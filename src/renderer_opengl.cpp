#include "SDL.h"

#include <array>
#include <string>
#include <utility>
#include <fstream>

#include "camera.hpp"
#include "common.hpp"
#include "file.hpp"

#include "enemy.hpp"
#include "laser.hpp"
#include "missile.hpp"
#include "renderer_opengl.hpp"
#include "util.hpp"

#include <ngl/VAOPrimitives.h>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

#define XP_DISPLAY_DIVISOR 64.0f

renderer_ngl::renderer_ngl()
    :
      m_camera()
{
    m_focus = {-1, -1};

    init();

    if(g_WIN_WIDTH > 0 and g_WIN_HEIGHT > 0)
    {
        m_w = g_WIN_WIDTH;
        m_h = g_WIN_HEIGHT;
    }
    else
    {
        SDL_DisplayMode best;
        best.w = 0;
        best.h = 0;

        for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
        {
            SDL_DisplayMode temp;
            SDL_GetCurrentDisplayMode(i, &temp);
            if(temp.w * temp.h > best.w * best.h)
                best = temp;
        }

        g_WIN_WIDTH = best.w;
        g_WIN_HEIGHT = best.h;

        g_HALFWIN = {g_WIN_WIDTH / 2.0f, g_WIN_HEIGHT / 2.0f};

        m_w = g_WIN_WIDTH;
        m_h = g_WIN_HEIGHT;

        std::cout << "Screen dimensions detected: " << g_WIN_WIDTH << ", " << g_WIN_HEIGHT << '\n';
    }

    m_window = SDL_CreateWindow("Captain Fractal: Attack of the Space Communists",
                                0, 0,
                                m_w, m_h,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS /*| SDL_WINDOW_FULLSCREEN*/ );

    if(!m_window)
    {
        errorExit("Unable to create window");
    }

    //Setting up the GL attributes before they can be used
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    m_gl_context = SDL_GL_CreateContext( m_window );

    if(!m_gl_context)
    {
        errorExit("Unable to create GL context");
    }

    makeCurrent();
    SDL_GL_SetSwapInterval(1);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ngl::NGLInit::instance();

    m_dataFB.initialise( m_w, m_h );
    m_dataFB.addTexture("diffuse", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT0);
    m_dataFB.addTexture("normal", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT1);
    m_dataFB.addTexture("position", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT2);
    m_dataFB.addDepthAttachment("depth");
    if(!m_dataFB.checkComplete())
        std::cout << "Error! m_dataFB creation failed!\n";

    m_nonlitFB.initialise( m_w, m_h );
    m_nonlitFB.addTexture("background", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT0);
    m_nonlitFB.addTexture("effects", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT1);

    //Lit and nonlit FBOs share depth attachment.
    GLuint d = m_dataFB.get("depth");
    glBindRenderbuffer(GL_RENDERBUFFER, d);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, m_w, m_h);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, d);

    if(!m_nonlitFB.checkComplete())
        std::cout << "Error! m_nonlitFB creation failed!\n";

    for(auto &buffer : m_postFB)
    {
        buffer.initialise( m_w, m_h );
        buffer.addTexture("diffuse", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT0);
        if(!buffer.checkComplete())
            std::cout << "Error! m_postFB creation failed!\n";
    }

    m_smallBackgroundFB.initialise( m_w / AMBIENT_RESOLUTION_DIVIDER, m_w / AMBIENT_RESOLUTION_DIVIDER );
    m_smallBackgroundFB.addTexture("diffuse", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT0);
    if(!m_smallBackgroundFB.checkComplete())
        std::cout << "Error! m_smallBackgroundFB creation failed!\n";

    m_uiFB.initialise( m_w, m_h );
    m_uiFB.addTexture( "diffuse", GL_RGBA, GL_RGBA16F, GL_COLOR_ATTACHMENT0 );
    if(!m_uiFB.checkComplete())
        std::cout << "Error! m_uiFB creation failed!\n";

    glViewport(0, 0, m_w, m_h);

    m_view = ngl::lookAt(ngl::Vec3(0,0,1),
                         ngl::Vec3(0,0,0),
                         ngl::Vec3(0,1,0));


    float divz = 1 / g_ZOOM_LEVEL;
    m_project = ngl::ortho(
                -g_HALFWIN.m_x * divz + m_cameraShakeOffset.m_x,
                g_HALFWIN.m_x * divz + m_cameraShakeOffset.m_x,
                g_HALFWIN.m_y * divz + m_cameraShakeOffset.m_y,
                -g_HALFWIN.m_y * divz + m_cameraShakeOffset.m_y,
                -2048.0,
                2048.0
                );

    m_uiProject = ngl::ortho(
                0.0f,
                m_w,
                m_h,
                0.0f,
                -256.0,
                256.0
                );

    m_cameraShake = 0.0f;
    m_cameraShakeTargetOffset = {0.0f, 0.0f};
    m_cameraShakeOffset = {0.0f, 0.0f};

    //m_project = ngl::perspective(200.0f, float(g_WIN_WIDTH / g_WIN_HEIGHT), 0.01f, 200.0f);

    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    createShaderProgram("bufferCopy", "backgroundVertex", "bufferCopyFragment");
    createShaderProgram("bufferCopyHighlights", "backgroundVertex", "bufferCopyHighlightsFragment");
    createShaderProgram("bufferDirectionalBlur", "backgroundVertex", "bufferDirectionalBlurFragment");
    createShaderProgram("bufferBlur", "backgroundVertex", "bufferBlurFragment");
    createShaderProgram("bufferGaussianBlur", "backgroundVertex", "bufferGaussianBlurFragment");
    createShaderProgram("bufferBloom", "backgroundVertex", "bufferBloomFragment");
    createShaderProgram("bufferLight", "backgroundVertex", "bufferLightFragment");

    createShaderProgram("background", "backgroundVertex", "backgroundFragment");
    createShaderProgram("plain", "DiffuseVertex", "DiffuseFragment");
    createShaderProgram("xp", "MVPUVVert", "XPFragment");
    createShaderProgram("ship", "shipVertex", "shipFragment");
    createShaderProgram("explosion", "MVPUVNVertDistort", "explosionFrag");
    createShaderProgram("flame", "rectVert", "flameFragment");
    createShaderProgram("smoke", "rectVert", "smokeFragment");
    createShaderProgram("shield", "MVPUVNVert", "shieldFragment");
    createShaderProgram("text", "MVPUVVert", "textureFragment");
    createShaderProgram("debug", "MVPVert", "debugFragment");

    createShaderProgramVGF("sparks", "laserVertex", "lineToRectGeo", "sparksFragment");
    createShaderProgramVGF("laser", "laserVertex", "lineToRectGeo", "laserFragment");

    slib->use("bufferDirectionalBlur");
    slib->setRegisteredUniform("iResolution", ngl::Vec2( g_WIN_WIDTH, g_WIN_HEIGHT ));

    slib->use("background");
    slib->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
    slib->setRegisteredUniform("iterations", (g_GRAPHICAL_DETAIL + 2.0f) * 3.0f);

    slib->use("bufferCopy");
    slib->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));

    slib->use("bufferBlur");
    slib->setRegisteredUniform("bgl_dim", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));

    std::cout << "loading ships starting!\n";
    loadShips();
    std::cout << "loading ships complete!\n";

    loadAsset("RESOURCE_IRON_ROCK",  "resource_iron_rock");

    loadFontSpriteSheet("pix", g_GRAPHICAL_RESOURCE_LOC + "fonts/pix.TTF", 20);
    loadFontSpriteSheet("minimal", g_GRAPHICAL_RESOURCE_LOC + "fonts/minimal.otf", 20);
    loadFontSpriteSheet("pix90", g_GRAPHICAL_RESOURCE_LOC + "fonts/pix.TTF", 60);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenBuffers(1, &m_vertBuffer);
    glGenBuffers(1, &m_UVBuffer);
    glGenBuffers(1, &m_colourBuffer);
    glGenBuffers(1, &m_genericBuffer);

    m_vao = createVAO({ngl::Vec3(0.0f, 0.0f, 0.0f), ngl::Vec3(0.0f, 1.0f, 0.0f)});

    m_unit_square_vao = createVAO({
                                      ngl::Vec3(-0.5f, -0.5f, 0.0f),
                                      ngl::Vec3(-0.5f, 0.5f, 0.0f),
                                      ngl::Vec3(0.5f, 0.5f, 0.0f),
                                      ngl::Vec3(0.5f, -0.5f, 0.0f)
                                  },
    {
                                      ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                      ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                      ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                      ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f)
                                  },
    {
                                      ngl::Vec2(0.0, 0.0),
                                      ngl::Vec2(0.0, 1.0),
                                      ngl::Vec2(1.0, 1.0),
                                      ngl::Vec2(1.0, 0.0)
                                  }
                                  );

    m_spriteVAO = createVAO({
                                ngl::Vec3(-1.0f, -1.0f, 0.0f),
                                ngl::Vec3(-1.0f, 1.0f, 0.0f),
                                ngl::Vec3(1.0f, 1.0f, 0.0f),
                                ngl::Vec3(1.0f, -1.0f, 0.0f)
                            },
    {
                                ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f),
                                ngl::Vec4(0.0f, 0.0f, 0.0f, 1.0f)
                            },
    {
                                ngl::Vec2(0.0, 0.0),
                                ngl::Vec2(0.0, 1.0),
                                ngl::Vec2(1.0, 1.0),
                                ngl::Vec2(1.0, 0.0)
                            }
                            );

    std::cout << "creating screen quad!\n";
    m_screenQuadVAO = createVAO({
                                    ngl::Vec3(-1.0f, -1.0f, 0.5f),
                                    ngl::Vec3(-1.0f, 1.0f, 0.5f),
                                    ngl::Vec3(1.0f, 1.0f, 0.5f),
                                    ngl::Vec3(1.0f, -1.0f, 0.5f)
                                },
    {
                                    ngl::Vec2(-0.5f, -0.5f),
                                    ngl::Vec2(-0.5f, 0.5f),
                                    ngl::Vec2(0.5f, 0.5f),
                                    ngl::Vec2(0.5f, -0.5f)
                                }
                                );

    m_pointVAO = createVAO({ngl::Vec3(0.0f, 0.0f, 0.0f)});

    m_shield = new ngl::Obj(g_GRAPHICAL_RESOURCE_LOC + "models/shield.obj");
    m_shield->createVAO();

    resetLights();

    m_noise512 = loadTexture(g_GRAPHICAL_RESOURCE_LOC + "textures/util/noise512RGB.png", GL_RGB);

    ngl::VAOPrimitives * prim = ngl::VAOPrimitives::instance();
    prim->createSphere( "explosion", 1.0f, 16 );

    m_pointLights.reserve(MAX_LIGHTS);
    glGenBuffers(1, &m_lightbuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightbuffer);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(pointLight) * MAX_LIGHTS, &m_pointLights[0].m_pos.m_x, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    finalise(0.0f, vec2());

    std::cout << "g_HALFWIN is " << g_HALFWIN.m_x << ", " << g_HALFWIN.m_y << std::endl;
    std::cout << "Resolution: " << g_WIN_WIDTH << " x " << g_WIN_HEIGHT << std::endl;
}

//This function loads all the ships in the game into a vector that we can copy from later.
void renderer_ngl::loadShips()
{
    std::ifstream data (g_RESOURCE_LOC + "ships/config.txt");
    std::string cur;

    int spec = 0;
    while(getlineSafe( data, cur ))
    {
        if(cur.length() == 0)
            continue;
        std::cout << "Loading ship from " << (g_RESOURCE_LOC + "ships/" + cur) << '\n';
        std::string asset = "";
        ship temp = loadShip(g_RESOURCE_LOC + "ships/" + cur, &asset, spec);
        loadAsset(temp.getIdentifier(), asset);
        g_ship_templates.push_back(temp);
        spec++;
    }
    /*for(int i = 0; i <= SHIPS_END; ++i)
        {
                ship insert = loadShip("");
                g_ship_templates.push_back(insert);
        }
        std::cout << "No of ship types: " << g_ship_templates.size() << std::endl;*/

}

void renderer_ngl::loadAsset(const std::string _key, const std::string _path)
{
    std::cout << "LOADING ASSET: " << _path << '\n';
    std::vector<ngl::Obj*> models;
    models.push_back( loadObj(_path, "") );
    models.push_back( loadObj(_path, "_static") );
    m_models.insert({_key, models});
    std::cout << "ASSET LOADED!\n";
}

ngl::Obj * renderer_ngl::loadObj(const std::string _path, const std::string _append)
{
    std::string full = _path + _append;
    if(std::ifstream(g_GRAPHICAL_RESOURCE_LOC + "models/" + _path + "/" + full + ".obj"))
    {
        ngl::Obj * tempObj = new ngl::Obj(g_GRAPHICAL_RESOURCE_LOC + "models/" + _path + "/" + full + ".obj", g_GRAPHICAL_RESOURCE_LOC + "textures/" + _path + "/" + full + ".png");
        tempObj->createVAO();
        return tempObj;
    }
    return nullptr;
}

void renderer_ngl::drawAsset(const vec2 _p, const float _ang, const std::string _asset, const float _alpha)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    slib->setRegisteredUniform("alpha", _alpha);

    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));
    m_transform.setRotation(90.0f, 0.0f, 180.0f + _ang);
    loadMatricesToShader();

    size_t modlen = m_models[_asset].size();
    for(size_t i = 0; i < modlen - 1; ++i)
    {
        if(m_models[_asset][i] != nullptr) m_models[_asset][i]->draw();
    }

    m_transform.setRotation(90.0f, 0.0f, 0.0f);
    loadMatricesToShader();

    if(m_models[_asset][modlen - 1] != nullptr) m_models[_asset][modlen - 1]->draw();

    m_transform.reset();
}

void renderer_ngl::drawAsset(const vec3 _p, const float _ang, const std::string _asset)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    m_transform.reset();

    slib->setRegisteredUniform("alpha", 1.0f);

    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, _p.m_z));
    m_transform.setRotation(90.0f, 0.0f, 180.0f + _ang);
    slib->setRegisteredUniform("M", m_transform.getMatrix());
    loadMatricesToShader();

    size_t modlen = m_models[_asset].size();
    for(size_t i = 0; i < modlen - 1; ++i)
    {
        if(m_models[_asset][i] != nullptr) m_models[_asset][i]->draw();
    }

    m_transform.setRotation(90.0f, 0.0f, 0.0f);
    slib->setRegisteredUniform("transform", m_transform.getMatrix());
    loadMatricesToShader();

    if(m_models[_asset][modlen - 1] != nullptr) m_models[_asset][modlen - 1]->draw();

    m_transform.reset();
}

void renderer_ngl::addLine(const vec3 _start, const vec3 _end, const float _width, const std::array<float, 4> _lCol)
{
    m_verts.push_back(_start);
    m_verts.push_back(_end);

    m_UVs.push_back(vec2(_width, 0.0f));
    m_UVs.push_back(vec2(0.0f, 0.0f));

    m_colours.push_back(_lCol);
    m_colours.push_back(_lCol);
}

void renderer_ngl::drawLasers(const float _globalTime)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    slib->use("laser");
    slib->setRegisteredUniform("iGlobalTime", _globalTime);

    drawLines(5.0f);
}

void renderer_ngl::drawLines(const float _width)
{
    if(m_verts.size() == 0 or m_colours.size() == 0) return;

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec3) * m_verts.size(),
                 &m_verts[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );


    glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec4) * m_colours.size(),
                 &m_colours[0][0],
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, 0 );

    //IN THIS CASE, UV CONTROLS THE LINE WIDTH, WITH UV[0].x BEING THE WIDTH
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec2) * m_UVs.size(),
                 &m_UVs[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glLineWidth(_width * g_ZOOM_LEVEL);
    loadMatricesToShader();
    glDrawArraysEXT(GL_LINES, 0, m_verts.size());

    glBindVertexArray(0);

    glLineWidth(1.0f);
}

/*void renderer_ngl::drawLaser(const vec2 _start, const vec2 _end, const std::array<float, 4> _lCol)
{
        vec2 fstart = {_start.m_x, -_start.m_y};
        vec2 fend = {_end.m_x, -_end.m_y};
        fstart *= g_ZOOM_LEVEL;
        fend *= g_ZOOM_LEVEL;
        fstart += g_HALFWIN + m_cameraShakeOffset;
        fend += g_HALFWIN + m_cameraShakeOffset;

        slib->setRegisteredUniform("fstart", ngl::Vec2(fstart.m_x, fstart.m_y));
        slib->setRegisteredUniform("fend", ngl::Vec2(fend.m_x, fend.m_y));

        //std::cout << "TRANSLATE " << fstart.m_x << ", " << fstart.m_y << std::endl;

        glBindVertexArray(m_vao);

        std::array<ngl::Vec3, 2> line = {
                ngl::Vec3(_start.m_x, _start.m_y,10.0f),
                ngl::Vec3(_end.m_x,  _end.m_y,  10.0f)
        };

        std::array<ngl::Vec4, 2> vertCol = {
                ngl::Vec4(_lCol[0], _lCol[1], _lCol[2], 0.0f),
                ngl::Vec4(_lCol[0], _lCol[1], _lCol[2], 1.0f)
        };

        //Generate a VBO
        //GLuint vertBuffer;
        //glGenBuffers(1, &vertBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                                 sizeof(ngl::Vec3) * line.size(),
                                 &line[0].m_x,
                        GL_STATIC_DRAW
                        );

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );


        //GLuint colourBuffer;
        //glGenBuffers(1, &colourBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
        glBufferData(GL_ARRAY_BUFFER,
                                 sizeof(ngl::Vec4) * vertCol.size(),
                                 &vertCol[0].m_x,
                        GL_STATIC_DRAW
                        );

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
        glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, 0 );

        glLineWidth(5.0f * g_ZOOM_LEVEL);
        loadMatricesToShader();
        glDrawArraysEXT(GL_LINES, 0, 2);

        glBindVertexArray(0);

        glLineWidth(1.0f);
}*/

void renderer_ngl::drawShield(const vec3 _p, const float _r, const float _dt, const std::array<float, 4> _col)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));
    m_transform.setRotation(90.0f, 0.0f, 0.0f);
    m_transform.setScale(_r, _r, _r);

    slib->use("shield");
    slib->setRegisteredUniform("iGlobalTime", _dt);
    slib->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));
    loadMatricesToShader();

    m_shield->draw();

    m_transform.reset();
}

renderer_ngl::~renderer_ngl()
{
    SDL_DestroyWindow( m_window );

    for(auto &i : m_models)
    {
        for(auto &j : i.second)
        {
            delete j;
            j = nullptr;
        }
        i.second.clear();
    }

    delete m_shield;
    m_shield = nullptr;
}

int renderer_ngl::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        errorExit("SDL initialisation failed");
    }

    if(TTF_Init() != 0)
    {
        errorExit("SDL TTF initialisation failed");
    }
    return 1;
}

void renderer_ngl::createShaderProgram(const std::string _name, const std::string _vert, const std::string _frag)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    slib->createShaderProgram(_name);
    slib->attachShader(_vert, ngl::ShaderType::VERTEX);
    slib->attachShader(_frag, ngl::ShaderType::FRAGMENT);

    slib->loadShaderSource(_vert, "shaders/" + _vert + ".glsl");
    slib->loadShaderSource(_frag, "shaders/" + _frag + ".glsl");

    slib->compileShader(_vert);
    slib->compileShader(_frag);

    slib->attachShaderToProgram(_name, _vert);
    slib->attachShaderToProgram(_name, _frag);

    slib->linkProgramObject(_name);
}

void renderer_ngl::createShaderProgramVGF(const std::string _name, const std::string _vert, const std::string _geo, const std::string _frag)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    slib->createShaderProgram(_name);
    slib->attachShader(_vert, ngl::ShaderType::VERTEX);
    slib->attachShader(_geo, ngl::ShaderType::GEOMETRY);
    slib->attachShader(_frag, ngl::ShaderType::FRAGMENT);

    slib->loadShaderSource(_vert, "shaders/" + _vert + ".glsl");
    slib->loadShaderSource(_geo, "shaders/" + _geo + ".glsl");
    slib->loadShaderSource(_frag, "shaders/" + _frag + ".glsl");

    slib->compileShader(_vert);
    slib->compileShader(_geo);
    slib->compileShader(_frag);

    slib->attachShaderToProgram(_name, _vert);
    slib->attachShaderToProgram(_name, _geo);
    slib->attachShaderToProgram(_name, _frag);

    slib->linkProgramObject(_name);
}

void renderer_ngl::update(float _dt, base * _focus)
{
    clearVectors();

    m_camera.setVel(_focus->getVel());
    m_camera.setTPos( _focus->getPos() );
    m_camera.update(_dt);
    //m_camera.updatePos(_dt);
    vec3 offset = m_camera.getCamPos();

    float divz = 1 / g_ZOOM_LEVEL;

    m_project = ngl::ortho(
                -g_HALFWIN.m_x * divz + offset.m_x,
                g_HALFWIN.m_x * divz + offset.m_x,
                g_HALFWIN.m_y  * divz + offset.m_y,
                -g_HALFWIN.m_y * divz + offset.m_y,
                -2048.0,
                2048.0
                );

    m_VP = m_view * m_project;
}

void renderer_ngl::drawBackground(float _dt, vec2 _p, vec2 _v, std::array<float, 3> _cCol)
{ 
    m_nonlitFB.bind();
    m_nonlitFB.activeColourAttachments({GL_COLOR_ATTACHMENT0});

    _p += m_cameraShakeOffset;
    ngl::Vec2 convp = ngl::Vec2(-_p.m_x, _p.m_y);
    ngl::Vec2 convv = ngl::Vec2(-_v.m_x, _v.m_y);

    convp.m_y += g_WIN_HEIGHT * 0.125 * (1 / g_ZOOM_LEVEL);

    for(auto &i : _cCol) i /= 255.0f;

    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    //std::cout << "Col " << _cCol[0] << ", " << _cCol[1] << ", " << _cCol[2] << std::endl;
    slib->use("background");
    slib->setRegisteredUniform("iGlobalTime", _dt);
    slib->setRegisteredUniform("zoom", 0.06f / g_ZOOM_LEVEL);
    slib->setRegisteredUniform("unipos", convp);
    slib->setRegisteredUniform("univel", convv);
    slib->setRegisteredUniform("inColour", ngl::Vec3(_cCol[0], _cCol[1], _cCol[2]));

    glBindVertexArray(m_screenQuadVAO);
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
}

GLuint renderer_ngl::createVAO(std::vector<ngl::Vec3> _verts)
{
    std::cout << "pure verts\n";
    GLuint temp_vao;
    glGenVertexArrays(1, &temp_vao);
    glBindVertexArray(temp_vao);

    //Generate a VBO
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec3) * _verts.size(),
                 &_verts[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    return temp_vao;
}

GLuint renderer_ngl::createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols)
{
    GLuint temp_vao;
    glGenVertexArrays(1, &temp_vao);
    glBindVertexArray(temp_vao);

    //Generate a VBO
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec3) * _verts.size(),
                 &_verts[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    GLuint colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec4) * _cols.size(),
                 &_cols[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindVertexArray(0);

    return temp_vao;
}

GLuint renderer_ngl::createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec2> _UVs)
{
    std::cout << "verts and uvs\n";
    GLuint temp_vao;
    glGenVertexArrays(1, &temp_vao);
    glBindVertexArray(temp_vao);

    //Generate a VBO
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec3) * _verts.size(),
                 &_verts[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    GLuint UVBuffer;
    glGenBuffers(1, &UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec2) * _UVs.size(),
                 &_UVs[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindVertexArray(0);

    return temp_vao;
}

GLuint renderer_ngl::createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols, std::vector<ngl::Vec2> _UVs)
{
    GLuint temp_vao;
    glGenVertexArrays(1, &temp_vao);
    glBindVertexArray(temp_vao);

    //Generate a VBO
    GLuint vertBuffer;
    glGenBuffers(1, &vertBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec3) * _verts.size(),
                 &_verts[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertBuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    GLuint colourBuffer;
    glGenBuffers(1, &colourBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec4) * _cols.size(),
                 &_cols[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colourBuffer);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, 0 );

    GLuint UVBuffer;
    glGenBuffers(1, &UVBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec2) * _UVs.size(),
                 &_UVs[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindVertexArray(0);

    return temp_vao;
}

void renderer_ngl::drawbutton(const vec3 _p, const vec2 _d, const float _ang, std::array<float, 4> _col)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    slib->use("debug");
    slib->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

    drawRect(_p, _d, _ang, false);
}

void renderer_ngl::addRect(const vec3 _p, const vec2 _d, const float _ang, const std::array<float, 4> _col)
{
    std::array<vec3, 6> pos;

    //Scaling
    pos[0] = {-_d.m_x / 2.0f, -_d.m_y / 2.0f, -0.5f};
    pos[1] = {_d.m_x / 2.0f, -_d.m_y / 2.0f, -0.5f};
    pos[2] = {-_d.m_x / 2.0f, _d.m_y / 2.0f, -0.5f};

    pos[3] = {_d.m_x / 2.0f, _d.m_y / 2.0f, -0.5f};
    pos[4] = {-_d.m_x / 2.0f, _d.m_y / 2.0f, -0.5f};
    pos[5] = {_d.m_x / 2.0f, -_d.m_y / 2.0f, -0.5f};

    float s = sin(_ang);
    float c = cos(_ang);

    //Rotation
    for(auto &i : pos)
    {
        i = {
            i.m_x * c - i.m_y * s,
            i.m_x * s + i.m_y * c,
            i.m_z
        };
    }

    //Positioning
    for(auto &i : pos)
    {
        i += _p;
    }

    //UVs
    std::array<vec2, 6> uv;

    uv[0] = {0.0f, 0.0f};
    uv[1] = {1.0f, 0.0f};
    uv[2] = {0.0f, 1.0f};

    uv[3] = {1.0f, 1.0f};
    uv[4] = {0.0f, 1.0f};
    uv[5] = {1.0f, 0.0f};

    for(int i = 0; i < 6; ++i)
    {
        m_verts.push_back(pos[i]);
        m_UVs.push_back(uv[i]);
        m_colours.push_back(_col);
    }
}

void renderer_ngl::addPoint(const vec3 _point)
{
    m_verts.push_back( _point );
}

void renderer_ngl::addColour(const std::array<float, 4> _col)
{
    m_colours.push_back( _col );
}

void renderer_ngl::drawRect(const vec3 _p, const vec2 _d, const float _ang, const bool _ws)
{
    glBindVertexArray(m_unit_square_vao);

    //m_transform.reset();
    m_transform.setRotation(0.0f, 0.0f, _ang);
    m_transform.setScale(_d.m_x, _d.m_y, 1.0f);
    m_transform.setPosition(_p.m_x, _p.m_y, -0.5f);

    if(_ws) loadMatricesToShader();
    else loadTransformToShader();

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    m_transform.reset();
}

void renderer_ngl::drawRects(const bool _ws)
{
    if(m_verts.size() == 0 or m_colours.size() == 0 or m_UVs.size() == 0) return;

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vec3) * m_verts.size(),
                 &m_verts[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );


    glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec4) * m_colours.size(),
                 &m_colours[0][0],
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_colourBuffer);
    glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, 0 );


    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(vec2) * m_UVs.size(),
                 &m_UVs[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    if(_ws) loadMatricesToShader();
    else loadTransformToShader();

    glDrawArraysEXT(GL_TRIANGLES, 0, m_verts.size());

    glBindVertexArray(0);
}

void renderer_ngl::drawXP(const float _xp)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    slib->use("xp");
    slib->setRegisteredUniform("xp", _xp / XP_DISPLAY_DIVISOR);

    drawRects(true);
}

void renderer_ngl::drawCircle(const vec3 _p, const float _d, const bool _ws)
{
    glPointSize(_d);

    glBindVertexArray(m_pointVAO);
    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));

    if(_ws) loadMatricesToShader();
    else loadTransformToShader();

    glDrawArraysEXT(GL_POINTS, 0, 1);
    glBindVertexArray(0);
    m_transform.reset();

    glPointSize(1.0f);
}

void renderer_ngl::drawExplosions()
{
    ngl::VAOPrimitives * prim = ngl::VAOPrimitives::instance();
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    for(size_t i = 0; i < m_verts.size(); ++i)
    {
        ngl::Vec3 pos = ngl::Vec3(m_verts[i].m_x, m_verts[i].m_y, m_verts[i].m_z);
        float s = pow(m_genericData[i][0], 4.0f) / 64.0f;
				float power = m_genericData[i][1];
        ngl::Vec4 col = ngl::Vec4( m_colours[i][0], m_colours[i][1], m_colours[i][2], m_colours[i][3] );

        m_transform.setPosition( pos );
        m_transform.setScale( ngl::Vec3(s, s, s) );
        m_transform.setRotation(0.0f, 0.0f, 0.0f);

				slib->setRegisteredUniform("power", power);
				slib->setRegisteredUniform("s", s);
        slib->setRegisteredUniform("colour", col);

        loadMatricesToShader();

        prim->draw("explosion");
    }

    /*ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    GLuint id = slib->getProgramID("explosion");
    bindTextureToSampler(id, m_noise512, "noiseTex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_noise512);

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);

    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_genericBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec4) * m_genericData.size(),
                 &m_genericData[0][0],
            GL_STATIC_DRAW
            );
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, m_genericBuffer);
    glVertexAttribPointer( 3, 4, GL_FLOAT, GL_FALSE, 0, 0 );

    drawRects(true);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);*/

    m_transform.reset();
}

void renderer_ngl::drawSmoke(const float _dt)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    slib->use("smoke");
    slib->setRegisteredUniform("iGlobalTime", _dt);
    drawRects(true);
}

void renderer_ngl::drawFlames(const vec3 _pos, const vec2 _d, float _ang, std::array<float, 4> _col, const float _t, const float _speed)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    slib->setRegisteredUniform("iGlobalTime", _t);
    slib->setRegisteredUniform("flameCol", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));
    slib->setRegisteredUniform("speed", _speed);

    glBindVertexArray(m_spriteVAO);
    //glBindVertexArray(m_unit_square_vao);

    m_transform.setScale(ngl::Vec3(_d.m_x, _d.m_y, 0.0f));
    m_transform.setRotation(ngl::Vec3(0.0f, 0.0f, _ang));
    m_transform.setPosition(ngl::Vec3(_pos.m_x, _pos.m_y, -1.0f));//_pos.m_z));

    loadMatricesToShader();
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    m_transform.reset();
}

void renderer_ngl::finalise(const float _t, const vec2 _vel)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    //Downsample background texture.
    m_smallBackgroundFB.bind();
    m_smallBackgroundFB.activeColourAttachments({GL_COLOR_ATTACHMENT0});
    glViewport(0, 0, m_w / AMBIENT_RESOLUTION_DIVIDER, m_h / AMBIENT_RESOLUTION_DIVIDER);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    slib->use("bufferCopy");

    GLuint id = slib->getProgramID("bufferCopy");

    m_nonlitFB.bindTexture(id, "background", "diffuse", 0);

    glBindVertexArray(m_screenQuadVAO);
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    //Clear back buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_w - 1, m_h - 1);
    glClear(GL_COLOR_BUFFER_BIT);

    drawCustomBuffers(_t, _vel);
    SDL_GL_SwapWindow(m_window);
}

void renderer_ngl::bindTextureToSampler(const GLint _shaderID, const GLuint _tex, const char * _uniform, int _target)
{
    GLint loc = glGetUniformLocation(_shaderID, _uniform);

    if(loc == -1)
    {
        std::cerr << "Uh oh! Invalid uniform location in renderer_ngl::bindTextureToSampler!! " << _uniform << '\n';
    }
    glUniform1i(loc, _target);
}

void renderer_ngl::drawCustomBuffers(const float _t, const vec2 _vel)
{
    //All depth stuff has been done already, we are just comping in this function.
    disableDepthTesting();
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    //Lighting setup.
    glBindBuffer(GL_UNIFORM_BUFFER, m_lightbuffer);
    GLvoid * dat = glMapBuffer(GL_UNIFORM_BUFFER, GL_WRITE_ONLY);
    memcpy(dat, &m_pointLights[0].m_pos.m_x, sizeof(pointLight) * std::min( m_pointLights.size(), static_cast<size_t>(MAX_LIGHTS)));
    glUnmapBuffer(GL_UNIFORM_BUFFER);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint id = slib->getProgramID("bufferLight");
    GLuint lightBlockIndex = glGetUniformBlockIndex( id, "lightBuffer" );
    GLuint index = 1;
    glBindBufferBase(GL_UNIFORM_BUFFER, index, m_lightbuffer);
    glUniformBlockBinding(id, lightBlockIndex, index);

    m_postFB[2].bind();
    m_postFB[2].activeColourAttachments();

    //Background draw
    slib->use("bufferDirectionalBlur");
    id = slib->getProgramID("bufferDirectionalBlur");

    m_nonlitFB.bindTexture(id, "background", "diffuse", 0);
    slib->setRegisteredUniform("vel", ngl::Vec2(_vel.m_x, _vel.m_y));
    slib->setRegisteredUniform("spd", mag(_vel));

    glBindVertexArray(m_screenQuadVAO);
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);

    //Lighting draw.
    slib->use("bufferLight");

    id = slib->getProgramID("bufferLight");

    m_dataFB.bindTexture(id, "diffuse", "diffuse", 0);
    m_dataFB.bindTexture(id, "normal", "normal", 1);
    m_dataFB.bindTexture(id, "position", "position", 2);
    m_smallBackgroundFB.bindTexture(id, "diffuse", "ambient", 3);

    slib->setRegisteredUniform("ambientSteps", ngl::Vec2(m_w / AMBIENT_RESOLUTION_DIVIDER, m_h / AMBIENT_RESOLUTION_DIVIDER));
    slib->setRegisteredUniform("inverseVP", (m_project * m_view).inverse());
    slib->setRegisteredUniform("iGlobalTime", _t);
    slib->setRegisteredUniform("lightMul", AMBIENT_RESOLUTION_DIVIDER / (256.0f * g_ZOOM_LEVEL));
    slib->setRegisteredUniform("ACTIVE_LIGHTS", static_cast<int>(std::min( m_pointLights.size(), static_cast<size_t>(MAX_LIGHTS) )));

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);

    //Effects draw.
    slib->use("bufferCopy");
    id = slib->getProgramID("bufferCopy");
    m_nonlitFB.bindTexture(id, "effects", "diffuse", 0);

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);

    //Post effect blur
    m_postFB[2].unbind();

    /*slib->use("bufferBlur");
    id = slib->getProgramID("bufferBlur");
    m_postFB[0].bindTexture(id, "diffuse", "diffuse", 0);

    int x = 0;
    int y = 0;
    SDL_GetMouseState(&x, &y);
    slib->setRegisteredUniform( "mousePos", ngl::Vec2(x / (float)m_w, (m_h - y) / (float)m_h));
    slib->setRegisteredUniform( "vel", mag(_vel) );
    */

    //Draw into our last posteffect buffer
    /*m_postFB[2].bind();
    m_postFB[0].bindTexture(id, "diffuse", "diffuse", 0);
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    m_postFB[2].unbind();*/

    //Copy that image into the first posteffect buffer.
    slib->use("bufferCopyHighlights");
    id = slib->getProgramID("bufferCopyHighlights");
    m_postFB[2].bindTexture(id, "diffuse", "diffuse", 0);
    m_postFB[0].bind();
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    m_postFB[0].unbind();

    //Ping-pong between first and second posteffect buffers.
    slib->use("bufferGaussianBlur");
    id = slib->getProgramID("bufferGaussianBlur");
    bool horizontal = true;
    int passes = 4;
    for(int i = 0; i < passes; ++i)
    {
        m_postFB[ static_cast<int>(horizontal) ].bind();
        slib->setRegisteredUniform("horizontal", horizontal);
        m_postFB[ static_cast<int>(!horizontal) ].bindTexture(id, "diffuse", "diffuse", 0);
        glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
        horizontal = !horizontal;
    }
    m_postFB[ static_cast<int>(horizontal) ].unbind();

    //Post effect bloom
    slib->use("bufferBloom");
    id = slib->getProgramID("bufferBloom");

    m_postFB[2].bindTexture(id, "diffuse", "diffuse", 0);
    m_postFB[static_cast<size_t>(horizontal)].bindTexture(id, "diffuse", "bloom", 1);

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);

    /*slib->use("bufferCopy");
    id = slib->getProgramID("bufferCopy");
    m_postFB[0].bindTexture(id, "diffuse", "diffuse", 0);

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);*/

    //Comp UI
    slib->use("bufferCopy");
    id = slib->getProgramID("bufferCopy");
    m_uiFB.bindTexture(id, "diffuse", "diffuse", 0);

    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);

    //Unbind VAO
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void renderer_ngl::loadMatricesToShader()
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    ngl::Mat4 MVP = m_transform.getMatrix() * m_VP;
    slib->setRegisteredUniform("MVP", MVP);
}

void renderer_ngl::loadTransformToShader()
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    ngl::Mat4 MVP = m_transform.getMatrix() * m_uiProject;
    slib->setRegisteredUniform("MVP", MVP);
}

void renderer_ngl::errorExit(const std::string &_msg)
{
    std::cerr << &_msg << " " << SDL_GetError() << '\n';
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void renderer_ngl::addShake(float _s)
{
    //m_cameraShake += _s;
    m_camera.addShake(_s);
    //m_cameraShakeTargetOffset = randVec2(m_cameraShake);
}

void renderer_ngl::statusBars(player * _ply)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    //health base
    std::array<float, 4> col = {{0.4f, 0.08f, 0.08f, 1.0f}};
    drawbutton({128.0f, 40.0f, 0.0f}, {256, 16}, 0.0f, col);

    //health
    float width = (_ply->getHealth() / _ply->getMaxHealth()) * 256;
    col = {{0.9f, 0.2f, 0.2f, 1.0f}};
    drawbutton({width / 2.0f, 40.0f, 0.0f}, {width, 16}, 0.0f, col);

    //shield base
    col = {{0.1f, 0.1f, 0.4f, 1.0f}};
    drawbutton({128.0f, 56.0f, 0.0f}, {256, 16}, 0.0f, col);

    width = (_ply->getShield() / _ply->getMaxShield()) * 256;
    //shield
    col = {{0.2f, 0.2f, 0.9f, 1.0f}};
    drawbutton({width / 2.0f, 56.0f, 0.0f}, {width, 16}, 0.0f, col);

    //energy base
    col = {{0.08f, 0.4f, 0.08f, 1.0f}};
    drawbutton({128.0f, 72.0f, 0.0f}, {256, 16}, 0.0f, col);

    width = (_ply->getEnergy() / _ply->getMaxEnergy()) * 256;
    //energy
    col = {{0.2f, 0.9f, 0.2f, 1.0f}};
    drawbutton({width / 2.0f, 72.0f, 0.0f}, {width, 16}, 0.0f, col);

    addRect({128.0f, 106.0f, 0.0f}, {256, 64}, 0.0f, col);
    slib->use("xp");
    slib->setRegisteredUniform("xp", _ply->getXP() / XP_DISPLAY_DIVISOR);
    drawRects(false);
    clearVectors();
}

void renderer_ngl::drawWeaponStats(player *_ply)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();
    vec2 dim = {g_WIN_WIDTH * 0.1f, g_WIN_HEIGHT * 0.2f};
    vec3 pos = {g_WIN_WIDTH - (dim.m_x / 2.0f), g_WIN_HEIGHT - 1.4f * (dim.m_y / 2.0f), 0.0f};

    std::array<float, 4> wc = _ply->getCurWeapCol();

    slib->use("debug");
    slib->setRegisteredUniform("inColour", ngl::Vec4(wc[0], wc[1], wc[2], 1.0f));
    drawRect(pos, dim, 0.0f, false);

    dim.m_x *= 0.75f;
    dim.m_y *= 0.75f;

    float fWIN_WIDTH = static_cast<float>(g_WIN_WIDTH), fWIN_HEIGHT = static_cast<float>(g_WIN_HEIGHT);
    std::string damageText = "Damage " + std::to_string(_ply->getCurWeapStat(DAMAGE));
    drawText(damageText, "pix", {fWIN_WIDTH - dim.m_x, fWIN_HEIGHT - 1.4f * dim.m_y}, false, 1.0f);

    std::string noText = "\nLasers " + std::to_string(static_cast<int>(_ply->getCurWeapStat(LASER_COUNT)));
    drawText(noText, "pix", {fWIN_WIDTH - dim.m_x, fWIN_HEIGHT - 1.2f * dim.m_y}, false, 1.0f);

    std::string spreadText = "\nSpread " + std::to_string(_ply->getCurWeapStat(SPREAD));
    drawText(spreadText, "pix", {fWIN_WIDTH - dim.m_x, fWIN_HEIGHT - 1.0f * dim.m_y}, false, 1.0f);

    std::string rateText = "\nRate ";
    float rate = 1.0f / _ply->getCurWeapStat(COOLDOWN);
    rateText += std::to_string( static_cast<int>( rate ) );
    drawText(rateText, "pix", {fWIN_WIDTH - dim.m_x, fWIN_HEIGHT - 0.8f * dim.m_y}, false, 1.0f);
}

void renderer_ngl::loadFontSpriteSheet(
        std::string _name,
        std::string _path,
        int _size
        )
{
    spriteSheet sheet;

    TTF_Font * fnt = TTF_OpenFont(_path.c_str(), _size);

    if(!fnt)
    {
        std::cerr << "Font loading error! " << SDL_GetError() << std::endl;
        TTF_CloseFont(fnt);
        return;
    }

    std::string ascii;
    for(int i = 0; i < 33; ++i) ascii += ' ';
    for(int i = 33; i < 127; ++i)
    {
        ascii += static_cast<char>(i);
    }
    ascii += "\0";

    for(size_t i = 0; i < ascii.length(); ++i)
    {
        //We need to first render to a surface as that's what TTF_RenderText
        //returns, then load that surface into a texture
        SDL_Surface * surf = TTF_RenderGlyph_Blended(fnt, ascii[i], {255,255,255});
        //SDL_Surface * surf = IMG_Load((g_GRAPHICAL_RESOURCE_LOC + "textures/player/player.png").c_str());

        if(!surf)
        {
            std::cerr << "Font loading error! " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surf);
            return;
        }

        GLuint texture = SDLSurfaceToGLTexture(surf, GL_RGBA);

        sheet.m_dim.push_back(std::make_pair(surf->w, surf->h));

        //Clean up the surface.
        SDL_FreeSurface(surf);

        sheet.m_sheet.push_back(texture);
    }

    TTF_CloseFont(fnt);

    m_letters.insert({_name, sheet});
}

GLuint renderer_ngl::loadTexture(const std::string &_path, const int _format)
{
    SDL_Surface * surf = IMG_Load(_path.c_str());
    GLuint tex;
    if(surf != nullptr)
        tex = SDLSurfaceToGLTexture(surf, _format);
    else
    {
        std::cerr << "Warning! Null surface. " << SDL_GetError() << '\n';
        return 0;
    }

    SDL_FreeSurface(surf);
    return tex;
}

GLuint renderer_ngl::SDLSurfaceToGLTexture(SDL_Surface * _s, int _format)
{
    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //int mode = GL_RGB;
    if(_s->format->BytesPerPixel == 4)
    {
        //mode = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, _format, _s->w, _s->h, 0, _format, GL_UNSIGNED_BYTE, _s->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void renderer_ngl::drawText(
        const std::string _text,
        const std::string _font,
        const vec2 _pos,
        const bool _ws,
        const float _mul
        )
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    spriteSheet * tmp = &m_letters[_font];

    slib->use("text");
    slib->setRegisteredUniform("inColour", ngl::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

    float x = _pos.m_x;
    float y = _pos.m_y;

    for(size_t i = 0; i < _text.length(); ++i)
    {
        //bindTextureToSampler(id, tmp->m_sheet[_text[i]], "diffuse", 0);
        //glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);

        float w = static_cast<float>(tmp->m_dim[_text[i]].first);
        float h = static_cast<float>(tmp->m_dim[_text[i]].second);

        w *= _mul;
        h *= _mul;
        //w = 8;

        drawRect({x, y, 0.0f}, {w, h}, 0.0f, _ws);
        x += w;
    }
}

void renderer_ngl::drawText(
        const std::string _text,
        const std::string _font,
        const vec2 _pos,
        const bool _ws,
        const float _mul,
        const std::array<float, 4> _col
        )
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    spriteSheet * tmp = &m_letters[_font];

    slib->use("text");
    slib->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

    float x = _pos.m_x;
    float y = _pos.m_y;

    for(size_t i = 0; i < _text.length(); ++i)
    {
        //std::cout << "doot " << tmp->m_sheet[_text[i]] << std::endl;
        //glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);
        glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);

        float w = static_cast<float>(tmp->m_dim[_text[i]].first);
        float h = static_cast<float>(tmp->m_dim[_text[i]].second);

        w *= _mul;
        h *= _mul;
        //w = 8;

        drawRect({x, y, 0.0f}, {w, h}, 0.0f, _ws);
        x += w;
    }
}

void renderer_ngl::enableDepthTesting()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void renderer_ngl::disableDepthTesting()
{
    glDisable(GL_DEPTH_TEST);
}

void renderer_ngl::drawMap(std::vector<missile> * _mp, std::vector<enemy> * _ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp, const bool _mode)
{
    ngl::ShaderLib * slib = ngl::ShaderLib::instance();

    slib->use("debug");
    slib->setRegisteredUniform("inColour", ngl::Vec4(0.5f, 0.5f, 0.5f, 0.4f));

    vec3 center;
    vec2 dim;
    float zoom;

    if(_mode)
    {
        center = tovec3(g_HALFWIN);
        center -= {0.0f, 100.0f, 0.0f};
        dim.m_x = std::min(g_WIN_WIDTH, g_WIN_HEIGHT) - 300.0f;
        dim.m_y = dim.m_x;
        zoom = g_ZOOM_LEVEL;
    }
    else if(!_mode)
    {
        center = {g_WIN_WIDTH - 128.0f, 128.0f, 0.0f};
        dim = {256.0f, 256.0f};
        zoom = 1.0f;
    }

    drawRect(center, dim, 0.0f, false);

    slib->setRegisteredUniform("inColour", ngl::Vec4(0.0f, 0.5f, 1.0f, 1.0f));
    drawCircle(center, 4.0f, false);

    slib->setRegisteredUniform("inColour", ngl::Vec4(0.5f, 0.5f, 0.5f, 1.0f));
    for(unsigned int i = 0; i < _lp->size(); i++)
    {
        vec3 lpp = _lp->at(i).getPos() - m_camera.getPos();

        float x = clamp(zoom * lpp.m_x / 156.0f + center.m_x, center.m_x - dim.m_x / 2.0f, center.m_x + dim.m_x / 2.0f);
        float y = clamp(zoom * lpp.m_y / 156.0f + center.m_y, center.m_y - dim.m_y / 2.0f, center.m_y + dim.m_y / 2.0f);

        drawCircle({x, y, 0.0f}, 1.0f, false);
    }

    slib->setRegisteredUniform("inColour", ngl::Vec4(1.0f, 0.0f, 0.1f, 1.0f));
    for(unsigned int i = 0; i < _mp->size(); i++)
    {
        vec3 mpp = _mp->at(i).getPos() - m_camera.getPos();

        float x = clamp(zoom * mpp.m_x / 156.0f + center.m_x, center.m_x - dim.m_x / 2.0f, center.m_x + dim.m_x / 2.0f);
        float y = clamp(zoom * mpp.m_y / 156.0f + center.m_y, center.m_y - dim.m_y / 2.0f, center.m_y + dim.m_y / 2.0f);

        drawCircle({x, y, 0.0f}, 1.0f, false);
    }

    slib->setRegisteredUniform("inColour", ngl::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
    for(unsigned int i = 0; i < _ap->size(); i++)
    {
        vec3 app = _ap->at(i).getPos() - m_camera.getPos();

        float x = clamp(zoom * app.m_x / 156.0f + center.m_x, center.m_x - dim.m_x / 2.0f, center.m_x + dim.m_x / 2.0f);
        float y = clamp(zoom * app.m_y / 156.0f + center.m_y, center.m_y - dim.m_y / 2.0f, center.m_y + dim.m_y / 2.0f);

        float radius = 1.0f;
        if(_ap->at(i).getClassification() == ASTEROID_MID) radius = 2.0f;
        else if(_ap->at(i).getClassification() == ASTEROID_LARGE) radius = 3.0f;

        drawCircle({x, y, 0.0f}, radius, false);
    }

    for(unsigned int i = 0; i < _ep->size(); i++)
    {
        vec3 epp = _ep->at(i).getPos() - m_camera.getPos();
        float radius = clamp( _ep->at(i).getRadius() / 16.0f,  1.0f,  5.0f );

        std::array<float, 4> col;
        col = col255to1(_fp->at(_ep->at(i).getTeam()).getColour());
        col[3] = 1.0f;
        slib->setRegisteredUniform("inColour", ngl::Vec4(col[0], col[1], col[2], col[3]));

        float x = clamp(zoom * epp.m_x / 156.0f + center.m_x, center.m_x - dim.m_x / 2.0f, center.m_x + dim.m_x / 2.0f);
        float y = clamp(zoom * epp.m_y / 156.0f + center.m_y, center.m_y - dim.m_y / 2.0f, center.m_y + dim.m_y / 2.0f);

        drawCircle({x, y, 0.0f}, radius, false);
    }
}

void renderer_ngl::clearVectors()
{
    m_verts.clear();
    m_colours.clear();
    m_UVs.clear();
    m_genericData.clear();
}

void renderer_ngl::clear()
{
    m_dataFB.bind();
    m_dataFB.clear();
    m_nonlitFB.bind();
    m_nonlitFB.clear();
    for(auto &buffer : m_postFB)
    {
        buffer.bind();
        buffer.clear();
    }
    m_smallBackgroundFB.bind();
    m_smallBackgroundFB.clear();
    m_uiFB.bind();
    m_uiFB.clear();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_w - 1, m_h - 1);
    glClear(GL_COLOR_BUFFER_BIT);
}

void renderer_ngl::drawingDeferredElements()
{
    m_dataFB.bind();
    m_dataFB.activeColourAttachments();
}

void renderer_ngl::drawingNonLitElements()
{
    m_nonlitFB.bind();
    m_nonlitFB.activeColourAttachments();
}

void renderer_ngl::drawingUI()
{
    m_uiFB.bind();
    m_uiFB.activeColourAttachments();
}

void renderer_ngl::activeColourAttachments(const std::vector<GLenum> _bufs)
{
    glDrawBuffers(_bufs.size(), &_bufs[0]);
}
