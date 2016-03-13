#include "SDL.h"

#include <array>
#include <string>

#include "common.hpp"

#if RENDER_MODE == 1

#include "renderer_opengl.hpp"
#include "util.hpp"

#include "enemy.hpp"
#include "laser.hpp"
#include "missile.hpp"

#include <ngl/VAOPrimitives.h>

renderer_ngl::renderer_ngl(int _w, int _h)
{
  init();
  m_w = _w;
  m_h = _h;

  m_window = SDL_CreateWindow("Ben is cool",
                              g_WIN_POS_X, g_WIN_POS_Y,
                              g_WIN_HEIGHT, g_WIN_WIDTH,
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

  m_gl_context = SDL_GL_CreateContext( m_window );

  if(!m_gl_context)
  {
    errorExit("Unable to create GL context");
  }

  makeCurrent();
  SDL_GL_SetSwapInterval(1);

  glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  swapWindow();

  ngl::NGLInit::instance();

  //loadTextures();
  m_project = ngl::perspective(45.0f,
                               float(_w / _h),
                               0.2f,
                               20.0f
                               );

  m_view = ngl::lookAt(ngl::Vec3(0,0,0),
                       ngl::Vec3(0,0,0),
                       ngl::Vec3(0,1,0));

  m_shader = ngl::ShaderLib::instance();
  std::cout << "p1" << std::endl;


  m_shader->createShaderProgram("background");
  m_shader->attachShader("backgroundVertex", ngl::ShaderType::VERTEX);
  m_shader->attachShader("backgroundFragment", ngl::ShaderType::FRAGMENT);

  m_shader->loadShaderSource("backgroundVertex", "shaders/backgroundVertex.glsl");
  m_shader->loadShaderSource("backgroundFragment", "shaders/backgroundFragment.glsl");

  m_shader->compileShader("backgroundVertex");
  m_shader->compileShader("backgroundFragment");

  m_shader->attachShaderToProgram("background", "backgroundVertex");
  m_shader->attachShaderToProgram("background", "backgroundFragment");

  m_shader->linkProgramObject("background");

  m_shader->createShaderProgram("plain");
  m_shader->attachShader("DiffuseVertex", ngl::ShaderType::VERTEX);
  m_shader->attachShader("DiffuseFragment", ngl::ShaderType::FRAGMENT);

  m_shader->loadShaderSource("DiffuseVertex", "shaders/DiffuseVertex.glsl");
  m_shader->loadShaderSource("DiffuseFragment", "shaders/DiffuseFragment.glsl");

  m_shader->compileShader("DiffuseVertex");
  m_shader->compileShader("DiffuseFragment");

  m_shader->attachShaderToProgram("plain", "DiffuseVertex");
  m_shader->attachShaderToProgram("plain", "DiffuseFragment");

  m_shader->linkProgramObject("plain");

  m_shader->use("background");
  std::cout << "p1" << std::endl;
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
  return 1;
}

void renderer_ngl::drawBackground(float _dt, vec2 _v)
{ 
  //std::cout << "uni vel! " << _v.m_x << ", " << _v.m_y << std::endl;
  ngl::Vec2 conv = ngl::Vec2(-_v.m_x, _v.m_y);
  //std::cout << "converted! " << conv.m_x << ", " << conv.m_y << std::endl;
  m_shader->use("background");
  m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
  m_shader->setRegisteredUniform("iGlobalTime", _dt);
  m_shader->setRegisteredUniform("zoom", g_ZOOM_LEVEL);
  m_shader->setRegisteredUniform("unidir", conv);

  drawRect({-1.0f, -1.0f}, {2.0f, 3.0f});
}


void renderer_ngl::drawTri(const vec2 _p, const float _d, const float _ang, const std::array<float, 4> _col)
{
  m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
  m_shader->setRegisteredUniform("zoom", g_ZOOM_LEVEL);
  std::array<ngl::Vec3, 3> tri = {
    ngl::Vec3(-(_d / 2), _d,  0.0f),
    ngl::Vec3(0.0f,            -_d,  0.0f),
    ngl::Vec3((_d / 2), _d,  0.0f)
  };

  for(auto &i : tri)
  {
    float tx = i.m_x * cos(rad(_ang)) + i.m_y * -sin(rad(_ang));
    float ty = i.m_x * sin(rad(_ang)) + i.m_y * cos(rad(_ang));
    i.m_x = tx + _p.m_x;
    i.m_y = ty + _p.m_y;
  }

  std::array<ngl::Vec4, 3> tricol;
  for(auto &i : tricol)
  {
    for(size_t j = 0; j < 4; ++j)
    {
      i[j] = _col[j];
    }
  }

  //Create a VAO
  glGenVertexArrays(1, &m_vao);
  //Make it active
  glBindVertexArray(m_vao);

  //Generate a VBO
  GLuint VBOvert;
  glGenBuffers(1, &VBOvert);
  glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
  //Copy vert data.
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(ngl::Vec3) * tri.size(),
               &tri[0].m_x,
      GL_STATIC_DRAW
      );

  //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);
  //glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, VBOvert);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

  //Generate a VBO
  /*GLuint VBOcol;
  glGenBuffers(1, &VBOcol);
  glBindBuffer(GL_ARRAY_BUFFER, VBOcol);
  //Copy vert data.
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(ngl::Vec4) * tricol.size(),
               &tricol[0].m_x,
      GL_STATIC_DRAW
      );

  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);
  glBindVertexArray(1);*/

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //glViewport(0, 0, m_w, m_h);
  //glBindVertexArray(m_vao);
  loadMatricesToShader();

  glDrawArraysEXT(GL_TRIANGLES, 0, 3);
  glDisableVertexAttribArray(0);
}

void renderer_ngl::drawRect(const vec2 _p, const vec2 _d)
{
  m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
  std::array<ngl::Vec3, 4> quad = {
    ngl::Vec3(_p.m_x,           _p.m_y,           0.0f),
    ngl::Vec3(_p.m_x + _d.m_x,  _p.m_y,           0.0f),
    ngl::Vec3(_p.m_x + _d.m_x,  _p.m_y + _d.m_y,  0.0f),
    ngl::Vec3(_p.m_x,           _p.m_y + _d.m_y,  0.0f)
  };

  //Create a VAO
  glGenVertexArrays(1, &m_vao);
  //Make it active
  glBindVertexArray(m_vao);

  //Generate a VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //Copy dat data
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(ngl::Vec3) * quad.size(),
               &quad[0].m_x,
      GL_STATIC_DRAW
      );

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_w, m_h);
  glBindVertexArray(m_vao);
  //loadMatricesToShader();
  glDrawArraysEXT(GL_TRIANGLE_FAN, 0, 4);
}

void renderer_ngl::drawLine(
    const vec2 _start,
    const vec2 _end
    )
{
  m_shader->setRegisteredUniform("iResolution", ngl::Vec2(static_cast<float>(g_WIN_WIDTH), static_cast<float>(g_WIN_HEIGHT)));
  std::array<ngl::Vec3, 2> line = {
    ngl::Vec3(_start.m_x,_start.m_y,0.0f),
    ngl::Vec3(_end.m_x,  _end.m_y,  0.0f)
  };

  //Create a VAO
  glGenVertexArrays(1, &m_vao);
  //Make it active
  glBindVertexArray(m_vao);

  //Generate a VBO
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //Copy dat data
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(ngl::Vec3) * line.size(),
               &line[0].m_x,
      GL_STATIC_DRAW
      );

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);

  //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glViewport(0, 0, m_w, m_h);
  glBindVertexArray(m_vao);
  //loadMatricesToShader();
  glDrawArraysEXT(GL_LINES, 0, 2);
}

void renderer_ngl::loadMatricesToShader()
{
  ngl::ShaderLib * shader = ngl::ShaderLib::instance();
  ngl::Mat4 MVP = m_transform.getMatrix() * m_view * m_project;
  m_shader->setRegisteredUniform("MVP", MVP);
}

void renderer_ngl::errorExit(const std::string &_msg)
{
  std::cerr << &_msg << " " << SDL_GetError() << std::endl;
  SDL_Quit();
  exit(EXIT_FAILURE);
}

#endif
