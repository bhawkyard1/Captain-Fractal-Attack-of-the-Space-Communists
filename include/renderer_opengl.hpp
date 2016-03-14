#ifndef RENDERER_OPENGL_HPP
#define RENDERER_OPENGL_HPP

#include <string>
#include <unordered_map>

#include <vector>

#include "sprite_sheet.hpp"
#include "vectors.hpp"

#include "common.hpp"
#include "missile.hpp"
#include "enemy.hpp"
#include "ship.hpp"
#include "laser.hpp"
#include "player.hpp"

#if RENDER_MODE == 1

#include <ngl/NGLInit.h>
#include <ngl/VertexArrayObject.h>
#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>

class renderer_ngl
{
  //The window we'll be rendering to
  SDL_Window * m_window = NULL;
  //SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;
  std::unordered_map<std::string, std::vector<SDL_Texture*>> m_textures;
  std::unordered_map<std::string, sprite_sheet> m_letters;
  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
  ngl::Transformation m_transform;
  ngl::ShaderLib * m_shader;

  std::vector<ngl::Vec3> m_verts;
  std::vector<ngl::Colour> m_colours;

  SDL_GLContext m_gl_context;
  GLuint m_vao;
  void loadMatricesToShader();
public:
  renderer_ngl(int _w, int _h);
  ~renderer_ngl();

  int init();

  void update();

  void genVBO();
  void addVerts(const std::vector<vec3> _verts);
  void packVerts(const int _slot);
  void drawVerts(const GLenum _mode);

  void drawBackground(float _dt, vec2 _v);
  void drawRect(const vec2 _p, const vec2 _d);
  void drawTri(const vec2 _p, const float _d, const float _ang);
  std::vector<vec3> constructTri(const vec2 _p, const float _d, const float _ang);
  void drawLine(const vec2 _start, const vec2 _end);
  void makeCurrent() const { SDL_GL_MakeCurrent(m_window, m_gl_context); }
  void swapWindow() const { SDL_GL_SwapWindow(m_window); }
  void clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  void errorExit(const std::string &_msg);

  void useShader(const std::string _sh) {m_shader->use(_sh);}
  /*void loadTextures() {return;}

  void loadFontSpriteSheet(std::string name, std::string path, int size) {return;}
  void loadSpriteSheet() {return;}
  void loadTexture(std::string _key, std::string _path, SDL_BlendMode _b) {return;}
  void loadTextureSet(std::string _key, std::string _set) {return;}
*/
  void setBlendMode (SDL_BlendMode _b) {return;}
  void drawTextureSet(std::string key, vec2 pos, float orient, std::array<float, 4> alphaMod) {return;}
  void drawTexture(std::string key, size_t index, vec2 pos, float orient, std::array<float, 4> col) {return;}
  void drawText(std::string text, std::string font, vec2 pos) {return;}
  void drawLine(vec2 _start, vec2 _end, std::array<float,4> _col ) {return;}
  void drawLine(vec2 _start, vec2 _end, std::array<int,4> _col) {return;}
  void drawLineGr(vec2, vec2, std::array<float, 4> scol, std::array<float, 4> ecol) {return;}
  void drawCircle(int x, int y, int radius, std::array<float, 4> _col) {return;}
  void drawCircleUI(int x, int y, int radius, std::array<float, 4> _col) {return;}
  void drawText(const std::string _text, const std::string _font, const vec2 _pos, const bool _wpos) {return;}

  void queryTexture(std::string identifier, int index, int * w, int * h) {*w = 32.0f; *h = 32.0f;}
  float getTextureRadius(std::string _identifier) {return 32.0f;}
/*
  //UI drawing
  void drawMap(std::vector<missile> * mp, std::vector<enemy> *ep, std::vector<ship> * ap, std::vector<laser> * lp) {return;}
  void statusBars(player * ply) {return;}
  void drawWeaponStats(player * ply) {return;}*/

  void finalise() {swapWindow();}

  SDL_Surface * getSurface(std::string path);
};

#endif

#endif

