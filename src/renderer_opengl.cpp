#include "SDL.h"

#include <array>
#include <string>
#include <utility>
#include <fstream>

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

  finalise();

  ngl::NGLInit::instance();

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
  createShaderProgram("flame", "explosionVertex", "flameFragment");
  createShaderProgram("smoke", "explosionVertex", "smokeFragment");
  createShaderProgram("shield", "MVPUVNVert", "shieldFragment");
  createShaderProgram("text", "MVPUVVert", "textureFragment");
  createShaderProgram("debug", "MVPVert", "debugFragment");

  m_shader->use("majorLazer");
  m_shader->setRegisteredUniform("resolution", ngl::Vec2( g_WIN_WIDTH, g_WIN_HEIGHT ));

  m_shader->use("background");
  m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
  m_shader->setRegisteredUniform("iterations", (g_GRAPHICAL_DETAIL + 1.0f) * 4.0f);

  loadAsset("COMMUNIST_1",         "commie_1");
  loadAsset("COMMUNIST_2",         "commie_2");

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

  loadFontSpriteSheet("pix", g_RESOURCE_LOC + "fonts/pix.TTF", 20);
  loadFontSpriteSheet("minimal", g_RESOURCE_LOC + "fonts/minimal.otf", 20);

  loadFontSpriteSheet("pix90", g_RESOURCE_LOC + "fonts/pix.TTF", 60);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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

  m_shield = new ngl::Obj(g_RESOURCE_LOC + "models/shield.obj");
  m_shield->createVAO();
}

void renderer_ngl::loadAsset(const std::string _key, const std::string _path)
{
  std::vector<ngl::Obj*> models;
  models.push_back( loadObj(_path, "") );
  models.push_back( loadObj(_path, "_static") );
  m_models.insert({_key, models});
}

ngl::Obj * renderer_ngl::loadObj(const std::string _path, const std::string _append)
{
  std::string full = _path + _append;
  if(std::ifstream(g_RESOURCE_LOC + "models/" + _path + "/" + full + ".obj"))
  {
    ngl::Obj * tempObj = new ngl::Obj(g_RESOURCE_LOC + "models/" + _path + "/" + full + ".obj", g_RESOURCE_LOC + "textures/" + _path + "/" + full + ".png");
    tempObj->createVAO();
    return tempObj;
  }
  return nullptr;
}

void renderer_ngl::drawAsset(const vec2 _p, const float _ang, const std::string _asset, const float _alpha)
{
  m_shader->setRegisteredUniform("alpha", _alpha);

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
  m_shader->setRegisteredUniform("alpha", 1.0f);

  m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, _p.m_z));
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

void renderer_ngl::drawShip(const vec3 _p, const float _ang, const std::string _asset, const std::array<float, 4> _lCol)
{
  m_shader->setRegisteredUniform("shootingLightCol", ngl::Vec4(_lCol[0], _lCol[1], _lCol[2], _lCol[3]));
  drawAsset(_p, _ang, _asset);
}

void renderer_ngl::addLine(const vec3 _start, const vec3 _end, const std::array<float, 4> _lCol)
{
  m_verts.push_back({_start.m_x, _start.m_y, _start.m_z});
  m_verts.push_back({_end.m_x, _end.m_y, _end.m_z});

  m_UVs.push_back({0.0f, 0.0f});
  m_UVs.push_back({1.0f, 1.0f});

  m_colours.push_back(_lCol);
  m_colours.push_back(_lCol);
}

void renderer_ngl::drawLasers()
{
  m_shader->setRegisteredUniform("zoom", g_ZOOM_LEVEL);
  m_shader->setRegisteredUniform("halfwin", ngl::Vec2(g_HALFWIN.m_x, g_HALFWIN.m_y));
  m_shader->setRegisteredUniform("shake", ngl::Vec2(m_cameraShakeOffset.m_x, m_cameraShakeOffset.m_y));

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

    m_shader->setRegisteredUniform("fstart", ngl::Vec2(fstart.m_x, fstart.m_y));
    m_shader->setRegisteredUniform("fend", ngl::Vec2(fend.m_x, fend.m_y));

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

void renderer_ngl::drawShield(const vec3 _p, const float _r, const float _dt, const float _alpha, const std::array<float, 4> _col)
{
  m_transform.setPosition(ngl::Vec3(_p.m_x, _p.m_y, 0.0f));
  m_transform.setRotation(90.0f, 0.0f, 0.0f);
  m_transform.setScale(_r, _r, _r);

  m_shader->use("shield");
  m_shader->setRegisteredUniform("iGlobalTime", _dt);
  m_shader->setRegisteredUniform("alpha", _alpha);
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));
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
  clearVectors();

  m_cameraShake = clamp(m_cameraShake - _dt * 5.0f, 0.0f, 20.0f);

  //m_cameraShake = 15.0f;

  if(magns(m_cameraShakeTargetOffset - m_cameraShakeOffset) < 80.0f)
  {
    m_cameraShakeTargetOffset = randVec2(m_cameraShake);
  }
  m_cameraShakeOffset += (m_cameraShakeTargetOffset - m_cameraShakeOffset) * clamp(_dt * 5.0f, 0.0f, 1.0f);

  g_ZOOM_LEVEL +=  m_cameraShake * 0.00003f;

  float divz = 1 / g_ZOOM_LEVEL;

  m_project = ngl::ortho(
        -g_HALFWIN.m_x * divz + m_cameraShakeOffset.m_x,
        g_HALFWIN.m_x * divz + m_cameraShakeOffset.m_x,
        g_HALFWIN.m_y * divz + m_cameraShakeOffset.m_y,
        -g_HALFWIN.m_y * divz + m_cameraShakeOffset.m_y,
        -2048.0,
        2048.0
        );

  m_VP = m_view * m_project;
}

void renderer_ngl::drawBackground(float _dt, vec2 _p, vec2 _v, std::array<float, 3> _cCol)
{ 
  _p += m_cameraShakeOffset;
  ngl::Vec2 convp = ngl::Vec2(-_p.m_x, _p.m_y);
  ngl::Vec2 convv = ngl::Vec2(-_v.m_x, _v.m_y);

  for(auto &i : _cCol) i /= 255.0f;

  //std::cout << "Col " << _cCol[0] << ", " << _cCol[1] << ", " << _cCol[2] << std::endl;
  m_shader->use("background");
  m_shader->setRegisteredUniform("iGlobalTime", _dt);
  m_shader->setRegisteredUniform("zoom", 0.06f / g_ZOOM_LEVEL);
  m_shader->setRegisteredUniform("unipos", convp);
  m_shader->setRegisteredUniform("univel", convv);
  m_shader->setRegisteredUniform("inColour", ngl::Vec3(_cCol[0], _cCol[1], _cCol[2]));

  glBindVertexArray(m_screenQuadVAO);
  glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
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

void renderer_ngl::drawButton(const vec3 _p, const vec2 _d, const float _ang, std::array<float, 4> _col)
{
  m_shader->use("debug");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

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

void renderer_ngl::drawRect(const vec3 _p, const vec2 _d, const float _ang, const bool _ws)
{
  glBindVertexArray(m_unit_square_vao);
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

/*
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
        ngl::Vec4(_lCol[0], _lCol[1], _lCol[2], _lCol[3] / 255.0f),
        ngl::Vec4(_lCol[0], _lCol[1], _lCol[2], _lCol[3] / 255.0f)
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
}*/

void renderer_ngl::drawExplosion(const vec3 _pos, const vec2 _d, const std::array<float, 4> _col)
{
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

  glBindVertexArray(m_spriteVAO);
  m_transform.setScale(ngl::Vec3(_d.m_x, _d.m_y, 0.0f));
  m_transform.setPosition(ngl::Vec3(_pos.m_x, _pos.m_y));

  loadMatricesToShader();
  glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
  m_transform.reset();
}

void renderer_ngl::drawExplosion(const vec3 _pos, const float _d, const std::array<float, 4> _col)
{
  drawExplosion(_pos, {_d, _d}, _col);
}

void renderer_ngl::drawSmoke(const vec3 _pos, const vec2 _d, const float _dt, const std::array<float, 4> _col)
{
  m_shader->use("smoke");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

  glBindVertexArray(m_spriteVAO);
  m_transform.setScale(ngl::Vec3(_d.m_x, _d.m_y, 0.0f));
  m_transform.setPosition(ngl::Vec3(_pos.m_x, _pos.m_y));

  loadMatricesToShader();
  glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
  glBindVertexArray(0);
  m_transform.reset();
}

void renderer_ngl::drawFlames(const vec3 _pos, const vec2 _d, float _ang, std::array<float, 4> _col, const float _t, const float _speed)
{
  m_shader->setRegisteredUniform("iGlobalTime", _t);
  m_shader->setRegisteredUniform("flameCol", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));
  m_shader->setRegisteredUniform("speed", _speed);

  glBindVertexArray(m_spriteVAO);

  m_transform.setScale(ngl::Vec3(_d.m_x, _d.m_y, 0.0f));
  m_transform.setRotation(ngl::Vec3(0.0f, 0.0f, _ang));
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
  m_cameraShakeTargetOffset = randVec2(m_cameraShake);
}

void renderer_ngl::statusBars(player * _ply)
{
  //health base
  std::array<float, 4> col = {0.4f, 0.08f, 0.08f, 1.0f};
  drawButton({128,40}, {256, 16}, 0.0f, col);

  //health
  col = {0.9f, 0.2f, 0.2f, 1.0f};
  drawButton({128,40}, {(_ply->getHealth() / _ply->getMaxHealth()) * 256, 16}, 0.0f, col);

  //shield base
  col = {0.1f, 0.1f, 0.4f, 1.0f};
  drawButton({128,56}, {256, 16}, 0.0f, col);

  //shield
  col = {0.2f, 0.2f, 0.9f, 1.0f};
  drawButton({128,56}, {(_ply->getShield() / _ply->getMaxShield()) * 256, 16}, 0.0f, col);

  //energy base
  col = {0.08f, 0.4f, 0.08f, 1.0f};
  drawButton({128,72}, {256, 16}, 0.0f, col);

  //energy
  col = {0.2f, 0.9f, 0.2f, 1.0f};
  drawButton({128,72}, {(_ply->getEnergy() / _ply->getMaxEnergy()) * 256, 16}, 0.0f, col);
}

void renderer_ngl::drawWeaponStats(player *_ply)
{
  vec2 dim = {g_WIN_WIDTH * 0.1f, g_WIN_HEIGHT * 0.2f};
  vec3 pos = {g_WIN_WIDTH - (dim.m_x / 2.0f), g_WIN_HEIGHT - 1.4f * (dim.m_y / 2.0f), 0.0f};

  std::array<float, 4> wc = _ply->getCurWeapCol();

  m_shader->use("debug");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(wc[0], wc[1], wc[2], 1.0f));
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

void renderer_ngl::drawText(std::string _text,
    std::string _font,
    vec2 _pos,
    bool _ws,
    const float _mul
    )
{
  spriteSheet * tmp = &m_letters[_font];

  m_shader->use("text");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(1.0f, 1.0f, 1.0f, 1.0f));

  float x = _pos.m_x;
  float y = _pos.m_y;

  for(int i = 0; i < _text.length(); ++i)
  {
    //std::cout << "doot " << tmp->m_sheet[_text[i]] << std::endl;
    //glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);
    glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);

    float w = static_cast<float>(tmp->m_dim[_text[i]].first);
    float h = static_cast<float>(tmp->m_dim[_text[i]].second);

    w *= _mul;
    h *= _mul;
    //w = 8;

    drawRect({x, y}, {w, h}, 0.0f, _ws);
    x += w;
  }
}

void renderer_ngl::drawText(std::string _text,
    std::string _font,
    vec2 _pos,
    bool _ws,
    const float _mul,
    const std::array<float, 4> _col
    )
{
  spriteSheet * tmp = &m_letters[_font];

  m_shader->use("text");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(_col[0], _col[1], _col[2], _col[3]));

  float x = _pos.m_x;
  float y = _pos.m_y;

  for(int i = 0; i < _text.length(); ++i)
  {
    //std::cout << "doot " << tmp->m_sheet[_text[i]] << std::endl;
    //glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);
    glBindTexture(GL_TEXTURE_2D, tmp->m_sheet[_text[i]]);

    float w = static_cast<float>(tmp->m_dim[_text[i]].first);
    float h = static_cast<float>(tmp->m_dim[_text[i]].second);

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

void renderer_ngl::drawMap(std::vector<missile> * _mp, std::vector<enemy> * _ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp)
{
  m_shader->use("debug");
  m_shader->setRegisteredUniform("inColour", ngl::Vec4(0.5f, 0.5f, 0.5f, 0.4f));
  drawRect({g_WIN_WIDTH - 128.0f, 128.0f}, {256.0f, 256.0f}, 0.0f, false);

  m_shader->setRegisteredUniform("inColour", ngl::Vec4(0.0f, 1.0f, 0.0f, 1.0f));
  drawCircle({g_WIN_WIDTH - 128.0f, 128.0f}, 2.0f, false);

  m_shader->setRegisteredUniform("inColour", ngl::Vec4(0.1f, 0.4f, 1.0f, 1.0f));
  for(unsigned int i = 0; i < _lp->size(); i++)
  {
    vec3 lpp = _lp->at(i).getPos();

    float x = clamp(lpp.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    float y = clamp(lpp.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    drawCircle({x, y}, 1.0f, false);
  }

  m_shader->setRegisteredUniform("inColour", ngl::Vec4(1.0f, 0.0f, 0.1f, 1.0f));
  for(unsigned int i = 0; i < _mp->size(); i++)
  {
    vec3 mpp = _mp->at(i).getPos();

    float x = clamp(mpp.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    float y = clamp(mpp.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    drawCircle({x, y}, 1.0f, false);
  }

  m_shader->setRegisteredUniform("inColour", ngl::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
  for(unsigned int i = 0; i < _ap->size(); i++)
  {
    vec3 app = _ap->at(i).getPos();

    float x = clamp(app.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    float y = clamp(app.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    float radius = 1.0f;
    if(_ap->at(i).getClassification() == ASTEROID_MID) radius = 2.0f;
    else if(_ap->at(i).getClassification() == ASTEROID_LARGE) radius = 3.0f;

    drawCircle({x, y}, radius, false);
  }

  for(unsigned int i = 0; i < _ep->size(); i++)
  {
    vec3 epp = _ep->at(i).getPos();
    float radius = clamp( _ep->at(i).getRadius() / 16.0f,  1.0f,  5.0f );

    std::array<float, 4> col;
    col = col255to1(_fp->at(_ep->at(i).getTeam()).m_colour);
    col[3] = 1.0f;
    m_shader->setRegisteredUniform("inColour", ngl::Vec4(col[0], col[1], col[2], col[3]));

    float x = clamp(epp.m_x / 156.0f + g_WIN_WIDTH - 128.0f, g_WIN_WIDTH - 256.0f, static_cast<float>(g_WIN_WIDTH));
    float y = clamp(epp.m_y / 156.0f + 128.0f, 0.0f, 256.0f);

    drawCircle({x, y}, radius, false);
  }
}

void renderer_ngl::clearVectors()
{
  m_verts.clear();
  m_colours.clear();
  m_UVs.clear();
}

#endif
