#include "SDL.h"

#include <array>
#include <string>
#include <utility>

#include "common.hpp"

#if RENDER_MODE == 1

#include "renderer_opengl.hpp"
#include "util.hpp"

#include "enemy.hpp"
#include "laser.hpp"
#include "missile.hpp"

#include <ngl/VAOPrimitives.h>

#include <SDL2/SDL.h>
#include <SDL_ttf.h>

renderer_ngl::renderer_ngl(int _w, int _h)
{
    init();
    m_w = _w;
    m_h = _h;

    m_window = SDL_CreateWindow("Captain Fractal: Attack of the Space Communists",
                                g_WIN_POS_X, g_WIN_POS_Y,
                                g_WIN_WIDTH, g_WIN_HEIGHT,
                                SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );

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

    glViewport(0, 0, m_w, m_h);

    m_gl_context = SDL_GL_CreateContext( m_window );

    if(!m_gl_context)
    {
        errorExit("Unable to create GL context");
    }

    makeCurrent();
    SDL_GL_SetSwapInterval(1);

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //Don't draw further fragments over closer ones.
    //This messes with transparency using ortho projection, so it's out for now.
    //glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);

    swapWindow();

    ngl::NGLInit::instance();

    //loadTextures();

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
                -256.0,
                256.0
                );

    m_uiProject = ngl::ortho(
                0.0f,
                g_WIN_WIDTH,
                g_WIN_HEIGHT,
                0.0f,
                -256.0,
                256.0
                );

    m_cameraShake = 0.0f;
    m_cameraShakeTargetOffset = {0.0f, 0.0f};
    m_cameraShakeOffset = {0.0f, 0.0f};

    //m_project = ngl::perspective(200.0f, float(g_WIN_WIDTH / g_WIN_HEIGHT), 0.01f, 200.0f);

    m_shader = ngl::ShaderLib::instance();

    createShaderProgram("background", "backgroundVertex", "backgroundFragment");
    createShaderProgram("plain", "DiffuseVertex", "DiffuseFragment");
    createShaderProgram("ship", "shipVertex", "shipFragment");
    createShaderProgram("majorLazer", "laserVertex", "laserFragment");
    createShaderProgram("explosion", "explosionVertex", "explosionFragment");
    createShaderProgram("text", "MVPUVVert", "textureFragment");
    createShaderProgram("debug", "MVPVert", "debugFragment");

    m_shader->use("background");

    loadAsset("FEDERATION_MKI",      "enemy_1");
    loadAsset("FEDERATION_MKII",     "enemy_2");
    loadAsset("FEDERATION_MKIII",    "enemy_3");
    loadAsset("FEDERATION_MKIV",     "enemy_4");
    loadAsset("FEDERATION_GUNSHIP",  "enemy_5");

    loadAsset("PIRATE_GNAT",         "pirate_1");
    loadAsset("PIRATE_CRUISER",      "pirate_2");
    loadAsset("PIRATE_WRANGLER",     "pirate_3");
    loadAsset("PIRATE_MARAUDER",     "pirate_4");
    loadAsset("PIRATE_GUNSHIP",      "pirate_5");

    loadAsset("PLAYER_SHIP",         "player");
    loadAsset("PLAYER_HUNTER",       "wingman_1");
    loadAsset("PLAYER_DEFENDER",     "wingman_2");
    loadAsset("PLAYER_DESTROYER",    "wingman_3");
    loadAsset("PLAYER_MINER_DROID",  "miner_1");
    loadAsset("PLAYER_TURRET",       "turret_1");

    loadAsset("PLAYER_STATION",      "station_1");
    loadAsset("PLAYER_GRAVWELL",     "well_1");
    loadAsset("PLAYER_BARRACKS",     "barracks_1");

    loadAsset("ION_MISSILE_MKI",     "missile_1");

    loadAsset("ASTEROID_SMALL",      "asteroid_1");
    loadAsset("ASTEROID_MID",        "asteroid_2");
    loadAsset("ASTEROID_LARGE",      "asteroid_3");

    loadFontSpriteSheet("pix", g_RESOURCE_LOC + "fonts/pix.TTF", 30);
    loadFontSpriteSheet("minimal", g_RESOURCE_LOC + "fonts/minimal.otf", 35);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //////////////////////////
    /// \brief glGenVertexArrays

    /*glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);

  glBindVertexArray(0);*/

    glGenBuffers(1, &m_vertBuffer);
    glGenBuffers(1, &m_UVBuffer);
    glGenBuffers(1, &m_colourBuffer);

    m_vao = createVAO({ngl::Vec3(0.0f, 0.0f, 0.0f), ngl::Vec3(0.0f, 1.0f, 0.0f)});

    m_unit_square_vao = createVAO({
                                      ngl::Vec3(-0.5f, -0.5f, 0.5f),
                                      ngl::Vec3(-0.5f, 0.5f, 0.5f),
                                      ngl::Vec3(0.5f, 0.5f, 0.5f),
                                      ngl::Vec3(0.5f, -0.5f, 0.5f)
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
                                ngl::Vec3(-1.0f, -1.0f, 0.5f),
                                ngl::Vec3(-1.0f, 1.0f, 0.5f),
                                ngl::Vec3(1.0f, 1.0f, 0.5f),
                                ngl::Vec3(1.0f, -1.0f, 0.5f)
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

    m_screenQuadVAO = createVAO({
                                    ngl::Vec3(-1.0f, -1.0f, 0.5f),
                                    ngl::Vec3(-1.0f, 1.0f, 0.5f),
                                    ngl::Vec3(1.0f, 1.0f, 0.5f),
                                    ngl::Vec3(1.0f, -1.0f, 0.5f)
                                });

    m_pointVAO = createVAO({ngl::Vec3(0.0f, 0.0f, 0.0f)});

    ////////////////////////////////////////////////////////////////
}

void renderer_ngl::loadAsset(const std::string _key, const std::string _path)
{
    //Load object and texture.
    ngl::Obj * tempObj = new ngl::Obj(g_RESOURCE_LOC + "models/" + _path + ".obj", g_RESOURCE_LOC + "textures/" + _path + "/" + _path + ".png");
    tempObj->createVAO();

    m_models.insert({_key, tempObj});
}

void renderer_ngl::drawAsset(const vec2 _p, const float _ang, const std::string _asset, const float _alpha)
{
    m_shader->setRegisteredUniform("alpha", _alpha);

    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));
    m_transform.setRotation(90.0f, 0.0f, 180.0f + _ang);

    loadMatricesToShader();
    m_models[_asset]->draw();

    m_transform.reset();
}

void renderer_ngl::drawAsset(const vec2 _p, const float _ang, const std::string _asset)
{
    m_shader->setRegisteredUniform("alpha", 1.0f);

    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));
    m_transform.setRotation(90.0f, 0.0f, 180.0f + _ang);

    loadMatricesToShader();
    m_models[_asset]->draw();

    m_transform.reset();
}

void renderer_ngl::drawShip(const vec2 _p, const float _ang, const std::string _asset, const std::array<float, 4> _lCol)
{
    m_shader->setRegisteredUniform("shootingLightCol", ngl::Vec4(_lCol[0], _lCol[1], _lCol[2], _lCol[3]));
    drawAsset(_p, _ang, _asset);
}

void renderer_ngl::drawLaser(const vec2 _start, const vec2 _end, const std::array<float, 4> _lCol)
{
    vec2 fstart = {_start.m_x, -_start.m_y};
    vec2 fend = {_end.m_x, -_end.m_y};
    fstart *= g_ZOOM_LEVEL;
    fend *= g_ZOOM_LEVEL;
    fstart += g_HALFWIN + m_cameraShakeOffset;
    fend += g_HALFWIN + m_cameraShakeOffset;

    m_shader->setRegisteredUniform("fstart", ngl::Vec2(fstart.m_x, fstart.m_y));
    m_shader->setRegisteredUniform("fend", ngl::Vec2(fend.m_x, fend.m_y));
    m_shader->setRegisteredUniform("resolution", ngl::Vec2( g_WIN_WIDTH, g_WIN_HEIGHT ));

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
}

renderer_ngl::~renderer_ngl()
{
    SDL_DestroyWindow( m_window );
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
    m_shader->createShaderProgram(_name);
    m_shader->attachShader(_vert, ngl::ShaderType::VERTEX);
    m_shader->attachShader(_frag, ngl::ShaderType::FRAGMENT);

    m_shader->loadShaderSource(_vert, "shaders/" + _vert + ".glsl");
    m_shader->loadShaderSource(_frag, "shaders/" + _frag + ".glsl");

    m_shader->compileShader(_vert);
    m_shader->compileShader(_frag);

    m_shader->attachShaderToProgram(_name, _vert);
    m_shader->attachShaderToProgram(_name, _frag);

    m_shader->linkProgramObject(_name);
}

void renderer_ngl::update(const float _dt)
{
    m_cameraShake = clamp(m_cameraShake - _dt, 0.0f, 15.0f);

    //m_cameraShake = 15.0f;

    if(magns(m_cameraShakeTargetOffset - m_cameraShakeOffset) < 40.0f)
    {
        m_cameraShakeTargetOffset = randVec(m_cameraShake);
    }
    m_cameraShakeOffset += (m_cameraShakeTargetOffset - m_cameraShakeOffset) * clamp(_dt * 0.5f, 0.0f, 1.0f);

    g_ZOOM_LEVEL +=  m_cameraShake * 0.00003f;

    float divz = 1 / g_ZOOM_LEVEL;

    m_project = ngl::ortho(
                -g_HALFWIN.m_x * divz + m_cameraShakeOffset.m_x,
                g_HALFWIN.m_x * divz + m_cameraShakeOffset.m_x,
                g_HALFWIN.m_y * divz + m_cameraShakeOffset.m_y,
                -g_HALFWIN.m_y * divz + m_cameraShakeOffset.m_y,
                -256.0,
                256.0
                );

    m_VP = m_view * m_project;
}

void renderer_ngl::drawBackground(float _dt, vec2 _p, vec2 _v)
{ 
    _p += m_cameraShakeOffset;
    ngl::Vec2 convp = ngl::Vec2(-_p.m_x, _p.m_y);
    ngl::Vec2 convv = ngl::Vec2(-_v.m_x, _v.m_y);

    std::cout << "yo " << convv.m_x << ", " << convv.m_y << std::endl;
    m_shader->use("background");
    m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
    m_shader->setRegisteredUniform("iGlobalTime", _dt);
    m_shader->setRegisteredUniform("zoom", 0.06f / g_ZOOM_LEVEL);
    m_shader->setRegisteredUniform("unipos", convp);
    m_shader->setRegisteredUniform("univel", convv);

    glBindVertexArray(m_screenQuadVAO);
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

}


void renderer_ngl::drawTri(const vec2 _p, const float _d, const float _ang)
{
    m_shader->use("plain");
    //m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
    //m_shader->setRegisteredUniform("zoom", g_ZOOM_LEVEL);
    std::array<ngl::Vec4, 3> tri = {
        ngl::Vec4(-(_d / 2), _d,  0.0f, 1.0f),
        ngl::Vec4(0.0f,            -_d,  0.0f, 1.0f),
        ngl::Vec4((_d / 2), _d,  0.0f, 1.0f)
    };

    m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));
    m_transform.setRotation(0.0f, 0.0f, _ang);

    //Create a VAO
    /*glGenVertexArrays(1, &m_unit_laser_vao);
  //Make it active
  glBindVertexArray(m_unit_laser_vao);

  //Generate a VBO
  GLuint VBOvert;
  glGenBuffers(1, &VBOvert);
  glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
  //Copy vert data.
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(ngl::Vec4) * tri.size(),
               &tri[0].m_x,
      GL_STATIC_DRAW
      );

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

  glViewport(0, 0, m_w, m_h);
  loadMatricesToShader();

  glDrawArraysEXT(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);*/

    m_transform.reset();
}

GLuint renderer_ngl::createVAO(std::vector<ngl::Vec3> _verts)
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
                 sizeof(ngl::Vec4) * _UVs.size(),
                 &_UVs[0].m_x,
            GL_STATIC_DRAW
            );

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, UVBuffer);
    glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

    glBindVertexArray(0);

    return temp_vao;
}

std::vector<vec3> renderer_ngl::constructTri(const vec2 _p, const float _d, const float _ang)
{
    std::vector<vec3> tri = {
        {-(_d / 2), _d,  0.0f},
        {0.0f,            -_d,  0.0f},
        {(_d / 2), _d,  0.0f}
    };

    for(auto &i : tri)
    {
        float tx = i.m_x * cos(rad(_ang)) + i.m_y * -sin(rad(_ang));
        float ty = i.m_x * sin(rad(_ang)) + i.m_y * cos(rad(_ang));
        i.m_x = tx + _p.m_x;
        i.m_y = ty + _p.m_y;
    }

    return tri;
}

void renderer_ngl::drawButton(const vec2 _p, const vec2 _d, const float _ang, std::array<float, 4> _col)
{
    m_shader->use("debug");
    m_shader->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

    drawRect(_p, _d, _ang, false);
}

void renderer_ngl::drawRect(const vec2 _p, const vec2 _d, const float _ang, const bool _ws)
{
    m_transform.setRotation(0.0f, 0.0f, _ang);
    m_transform.setScale(_d.m_x, _d.m_y, 1.0f);
    m_transform.setPosition(_p.m_x, _p.m_y, -0.5f);

    if(_ws) loadMatricesToShader();
    else loadTransformToShader();

    glBindVertexArray(m_unit_square_vao);
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);

    m_transform.reset();
}

void renderer_ngl::drawLine(
        const vec2 _start,
        const vec2 _end,
        const std::array<float, 4> _lCol
        )
{ 
    glBindVertexArray(m_vao);

    std::array<ngl::Vec3, 2> line = {
        ngl::Vec3(_start.m_x, _start.m_y,10.0f),
        ngl::Vec3(_end.m_x,  _end.m_y,  10.0f)
    };

    std::array<ngl::Vec4, 2> vertCol = {
        ngl::Vec4(_lCol[0] / 255.0f, _lCol[1] / 255.0f, _lCol[2] / 255.0f, _lCol[3] / 255.0f),
        ngl::Vec4(_lCol[0] / 255.0f, _lCol[1] / 255.0f, _lCol[2] / 255.0f, _lCol[3] / 255.0f)
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

    glLineWidth(2.0f * g_ZOOM_LEVEL);
    loadMatricesToShader();
    glDrawArraysEXT(GL_LINES, 0, 2);

    glBindVertexArray(0);

    glLineWidth(1.0f);
}

void renderer_ngl::drawExplosion(const vec2 _pos, const vec2 _d, const float _alpha)
{
    m_shader->use("explosion");
    m_shader->setRegisteredUniform("alpha", _alpha);

    glBindVertexArray(m_spriteVAO);

    //Verts
    /*std::array<ngl::Vec3, 4> verts = {
        ngl::Vec3(-_d, -_d),
        ngl::Vec3(-_d, _d),
        ngl::Vec3(_d, _d),
        ngl::Vec3(_d, -_d)
    };*/
    /*
    glBindBuffer(GL_ARRAY_BUFFER, m_vertBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec3) * verts.size(),
                 &verts[0].m_x,
            GL_STATIC_DRAW
            );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    */
    //UVs
    /*std::array<ngl::Vec2, 4> uvs = {
        ngl::Vec2(0.0, 0.0),
        ngl::Vec2(0.0, 1.0),
        ngl::Vec2(1.0, 1.0),
        ngl::Vec2(1.0, 0.0)
    };

    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(ngl::Vec2) * uvs.size(),
                 &uvs[0].m_x,
            GL_STATIC_DRAW
            );
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, m_UVBuffer);
    glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 0, 0 );*/

    //m_transform.setRotation(ngl::Vec3(0.0f, _ang, 0.0f));
    m_transform.setScale(ngl::Vec3(_d.m_x, _d.m_y, 0.0f));
    m_transform.setPosition(ngl::Vec3(_pos.m_x, _pos.m_y));

    loadMatricesToShader();
    glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);
    m_transform.reset();
}

void renderer_ngl::loadMatricesToShader()
{
    ngl::Mat4 MVP = m_transform.getMatrix() * m_VP;
    m_shader->setRegisteredUniform("MVP", MVP);
}

void renderer_ngl::loadTransformToShader()
{
    ngl::Mat4 MVP = m_transform.getMatrix() * m_uiProject;
    m_shader->setRegisteredUniform("MVP", MVP);
}

void renderer_ngl::errorExit(const std::string &_msg)
{
    std::cerr << &_msg << " " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(EXIT_FAILURE);
}

void renderer_ngl::addShake(float _s)
{
    m_cameraShake += _s;
    m_cameraShakeTargetOffset = randVec(m_cameraShake);
}

void renderer_ngl::statusBars(player * _ply)
{
    //health base
    std::array<float, 4> col = {0.4f, 0.08f, 0.08f, 1.0f};
    drawButton({0,0}, {256, 8}, 0.0f, col);

    //health
    col = {0.9f, 0.2f, 0.2f, 1.0f};
    drawButton({0,0}, {(_ply->getHealth() / _ply->getMaxHealth()) * 256, 8}, 0.0f, col);

    //shield base
    col = {0.1f, 0.1f, 0.4f, 1.0f};
    drawButton({0,16}, {256, 8}, 0.0f, col);

    //shield
    col = {0.2f, 0.2f, 0.9f, 1.0f};
    drawButton({0,16}, {(_ply->getShield() / _ply->getMaxShield()) * 256, 8}, 0.0f, col);

    //energy base
    col = {0.08f, 0.4f, 0.08f, 1.0f};
    drawButton({0,32}, {256, 8}, 0.0f, col);

    //energy
    col = {0.2f, 0.9f, 0.2f, 1.0f};
    drawButton({0,32}, {(_ply->getEnergy() / _ply->getMaxEnergy()) * 256, 8}, 0.0f, col);
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
        //SDL_Surface * surf = IMG_Load((g_RESOURCE_LOC + "textures/player/player.png").c_str());

        if(!surf)
        {
            std::cerr << "Font loading error! " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surf);
            return;
        }

        GLuint texture = SDLSurfaceToGLTexture(surf);

        sheet.m_dim.push_back(std::make_pair(surf->w, surf->h));

        //Clean up the surface.
        SDL_FreeSurface(surf);

        sheet.m_sheet.push_back(texture);
    }

    TTF_CloseFont(fnt);

    m_letters.insert({_name, sheet});
}



GLuint renderer_ngl::SDLSurfaceToGLTexture(SDL_Surface * _s)
{
    GLuint textureID;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //int mode = GL_RGB;
    if(_s->format->BytesPerPixel == 4)
    {
        //mode = GL_RGB;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _s->w, _s->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, _s->pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return textureID;
}

void renderer_ngl::drawText(
        std::string _text,
        std::string _font,
        vec2 _pos,
        bool _ws,
        const float _mul
        )
{
    spriteSheet * tmp = &m_letters[_font];

    m_shader->use("text");

    float x = _pos.m_x;
    float y = _pos.m_y;

    for(int i = 0; i < _text.length(); ++i)
    {
        //std::cout << "doot " << tmp->m_sheet[_text[i]] << std::endl;
        //glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);
        glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);

        float w = static_cast<float>(tmp->m_dim[i].first);
        float h = static_cast<float>(tmp->m_dim[i].second);
        w *= _mul;
        h *= _mul;
        //w = 8;

        drawRect({x, y}, {(float)w, (float)h}, 0.0f, _ws);
        x += w;
    }
}

void renderer_ngl::enableDepthSorting()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

void renderer_ngl::disableDepthSorting()
{
    glDisable(GL_DEPTH_TEST);
}

void renderer_ngl::drawMap(std::vector<missile> *mp, std::vector<enemy> *ep, std::vector<ship> *ap, std::vector<laser> *lp)
{
  /*SDL_Rect map;
  map.w = 256;
  map.h = 256;
  map.x = g_WIN_WIDTH - 256;
  map.y = 0;

  SDL_SetRenderDrawColor(m_renderer, 200, 200, 255, 100);
  SDL_RenderFillRect(m_renderer,&map);

  SDL_SetRenderDrawColor(m_renderer,0, 0, 0, 255);
  SDL_RenderDrawLine(m_renderer, g_WIN_WIDTH-128, 124, g_WIN_WIDTH-128, 132);
  SDL_RenderDrawLine(m_renderer, g_WIN_WIDTH-124, 128, g_WIN_WIDTH-132, 128);*/

  m_shader->use("debug");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(0.5f, 0.5f, 0.5f, 0.4f));
  drawRect({g_WIN_WIDTH - 256.0f, 0.0f}, {256.0f, 256.0f}, 0.0f, false);

  /*SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
  for(unsigned int i = 0; i < _lp->size(); i++)
  {
    vec2 lpp = _lp->at(i).getPos();

    double x = clamp(lpp.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    double y = clamp(lpp.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    SDL_RenderDrawPoint(m_renderer, x, y);
  }

  SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
  for(unsigned int i = 0; i < _mp->size(); i++)
  {
    vec2 mpp = _mp->at(i).getPos();

    double x = clamp(mpp.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    double y = clamp(mpp.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    SDL_RenderDrawPoint(m_renderer, x, y);
  }

  SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
  for(unsigned int i = 0; i < _ap->size(); i++)
  {
    vec2 app = _ap->at(i).getPos();

    double x = clamp(app.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    double y = clamp(app.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    int radius = 1;
    if(_ap->at(i).getClassification() == ASTEROID_MID) radius = 2;
    else if(_ap->at(i).getClassification() == ASTEROID_LARGE) radius = 3;

    drawCircleUI(x, y, radius, {200,200,200,255});
  }

  for(unsigned int i = 0; i < _ep->size(); i++)
  {
    vec2 epp = _ep->at(i).getPos();
    int radius = clamp( _ep->at(i).getRadius() / 16.0f,  1.0f,  5.0f );

    std::array<int, 4> col;
    col = _fp->at(_ep->at(i).getTeam()).m_colour;
    col[3] = 255;

    float x = clamp(epp.m_x / 156.0f + g_WIN_WIDTH - 128.0f, g_WIN_WIDTH - 256.0f, static_cast<float>(g_WIN_WIDTH));
    float y = clamp(epp.m_y / 156.0f + 128.0f, 0.0f, 256.0f);

    drawCircleUI(x,y,radius,col);
  }*/
}

#endif
