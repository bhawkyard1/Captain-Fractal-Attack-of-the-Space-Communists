#ifndef RENDERER_OPENGL_HPP
#define RENDERER_OPENGL_HPP

#include <string>
#include <unordered_map>
#include <array>
#include <vector>

#include "vectors.hpp"

#include "common.hpp"
#include "missile.hpp"
#include "enemy.hpp"
#include "ship.hpp"
#include "laser.hpp"
#include "player.hpp"
#include "faction.hpp"
#include "sprite_sheet_opengl.hpp"

#if RENDER_MODE == 1

#include <ngl/NGLInit.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>
#include <ngl/Obj.h>

//#include <ngl/Text.h>

class renderer_ngl
{
  //The window we'll be rendering to
  SDL_Window * m_window = NULL;
  //SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;

  std::unordered_map<std::string, spriteSheet> m_letters;

  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
  ngl::Mat4 m_uiProject;
  ngl::Mat4 m_VP;
  ngl::Transformation m_transform;
  ngl::ShaderLib * m_shader;

  std::unordered_map<std::string, std::vector<ngl::Obj*>> m_models;
  ngl::Obj * m_shield;

  ngl::Obj * m_test_ship;
  ngl::Texture m_test_texture;
  GLuint m_test_texture_id;

  SDL_GLContext m_gl_context;

  GLuint m_pointVAO;
  GLuint m_screenQuadVAO;
  GLuint m_unit_square_vao;
  GLuint m_spriteVAO;
  GLuint m_vao;

  GLuint m_vertBuffer;
  GLuint m_UVBuffer;
  GLuint m_colourBuffer;

  std::vector<vec3> m_verts;
  std::vector<std::array<float, 4>> m_colours;
  std::vector<vec2> m_UVs;

  void loadMatricesToShader();
  void loadTransformToShader();

  float m_cameraShake;
  vec2 m_cameraShakeTargetOffset;
  vec2 m_cameraShakeOffset;
public:
  renderer_ngl(int _w, int _h);
  ~renderer_ngl();

  int init();

  void createShaderProgram(const std::string _name, const std::string _vert, const std::string _frag);
  void update(const float _dt);

  void setShader(const std::string _shader) {m_shader->use(_shader);}

  GLuint createVAO(std::vector<ngl::Vec3> _verts);
  GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols);
  GLuint createVAO(std::vector<ngl::Vec3> _verts, std::vector<ngl::Vec4> _cols, std::vector<ngl::Vec2> _UVs);

  void clearVectors();

  void drawBackground(float _dt, vec2 _v, vec2 _p);

  void drawButton(const vec2 _p, const vec2 _d, const float _ang, std::array<float, 4> _col);

  void addRect(const vec2 _p, const vec2 _d, const float _ang, const std::array<float, 4> _col);
  void drawRects(const bool _ws);

  void drawRect(const vec2 _p, const vec2 _d, const float _ang, const bool _ws);
  void drawSmoke(const vec2 _pos, const vec2 _d, const float _dt, const std::array<float, 4> _col);
  void drawCircle(const vec2 _p, const float _d, const bool _ws);
  void makeCurrent() const { SDL_GL_MakeCurrent(m_window, m_gl_context); }
  void swapWindow() const { SDL_GL_SwapWindow(m_window); }
  void clear() const {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  void errorExit(const std::string &_msg);

  ngl::Obj * loadObj(const std::string _path, const std::string _append);
  void loadAsset(const std::string _key, const std::string _path);
  void drawAsset(const vec2 _p, const float _ang, const std::string _asset);
  void drawAsset(const vec2 _p, const float _ang, const std::string _asset, const float _alpha);

  void drawShield(const vec2 _p, const float _r, const float _dt, const float _alpha, const std::array<float, 4> _col);
  void drawShip(const vec2 _p, const float _ang, const std::string _asset, const std::array<float, 4> _lCol);
  //void drawLaser(const vec2 _start, const vec2 _end, const std::array<float, 4> _lCol);
  void drawLasers();
  void drawLines(float _width);

  void drawExplosion(const vec2 _pos, const float _d, const std::array<float, 4> _col);
  void drawExplosion(const vec2 _pos, const vec2 _d, const std::array<float, 4> _col);
  void drawFlames(const vec2 _pos, const vec2 _d, float _ang, std::array<float, 4> _col, const float _t, const float _speed);

  void useShader(const std::string _sh) {m_shader->use(_sh);}
  /*void loadTextures() {return;}*/

  void loadFontSpriteSheet(std::string name, std::string path, int size);
  /*void loadSpriteSheet() {return;}
  void loadTexture(std::string _key, std::string _path, SDL_BlendMode _b) {return;}
  void loadTextureSet(std::string _key, std::string _set) {return;}
*/
  void setBlendMode (SDL_BlendMode _b) {return;}
  void drawTextureSet(std::string key, vec2 pos, float orient, std::array<float, 4> alphaMod) {return;}
  void drawTexture(std::string key, size_t index, vec2 pos, float orient, std::array<float, 4> col) {return;}
  void drawText(std::string text, std::string font, vec2 pos, const bool _w, const float _s);

  void addLine(const vec2 _start, const vec2 _end, const std::array<float, 4> _lCol);
  //void drawLine(const vec2 _start, const vec2 _end, const std::array<float,4> _lCol);

  void queryTexture(std::string identifier, int index, int * w, int * h) {*w = 32.0f; *h = 32.0f;}
  //float getTextureRadius(ship_spec _type) {return 32.0f;}
  float getTextureRadius(ship_spec _type) {return g_texture_keys[(_type)].m_radius;}

  //UI drawing
  void drawMap(std::vector<missile> * _mp, std::vector<enemy> *_ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp);
  void statusBars(player * ply);
  void drawWeaponStats(player * ply) {return;}

  void finalise() {swapWindow();}

  SDL_Surface * getSurface(std::string path);
  void addShake(float _s);

  GLuint SDLSurfaceToGLTexture(SDL_Surface * _s);

  void enableDepthSorting();
  void disableDepthSorting();
};

#endif

#endif

