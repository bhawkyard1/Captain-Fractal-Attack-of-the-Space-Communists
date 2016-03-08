#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <unordered_map>

#include <vector>

#include <SDL_image.h>

#include "sprite_sheet.hpp"
#include "vectors.hpp"

#include "common.hpp"
#include "missile.hpp"
#include "enemy.hpp"
#include "ship.hpp"
#include "laser.hpp"
#include "player.hpp"

#include <ngl/Camera.h>
#include <ngl/Colour.h>
#include <ngl/Light.h>
#include <ngl/Transformation.h>
#include <ngl/Text.h>
#include <ngl/Transformation.h>
#include <ngl/ShaderLib.h>

class renderer
{
  //The window we'll be rendering to
  SDL_Window * m_window = NULL;
  //SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;
  /*std::unordered_map<std::string, std::vector<SDL_Texture*>> m_textures;
  std::unordered_map<std::string, sprite_sheet> m_letters;*/
  ngl::Mat4 m_view;
  ngl::Mat4 m_project;
  ngl::Transformation m_transform;
  ngl::ShaderLib * shader;
  void loadMatricesToShader();
public:
  renderer(int,int);
  ~renderer();

  int init();
  /*void loadTextures();

  void loadFontSpriteSheet(std::string name, std::string path, int size);
  void loadSpriteSheet();
  void loadTexture(std::string _key, std::string _path, SDL_BlendMode _b);
  void loadTextureSet(std::string _key, std::string _set);

  void setBlendMode (SDL_BlendMode _b) {SDL_SetRenderDrawBlendMode(m_renderer, _b);}
  void clear();
  void drawTextureSet(std::string key, vec2 pos, float orient, std::array<float, 4> alphaMod);
  void drawTexture(std::string key, size_t index, vec2 pos, float orient, std::array<float, 4> col);
  void drawText(std::string text, std::string font, vec2 pos);
  void drawLine(vec2 _start, vec2 _end, std::array<float,4> _col );
  void drawLine(vec2 _start, vec2 _end, std::array<int,4> _col);
  void drawLineGr(vec2, vec2, std::array<float, 4> scol, std::array<float, 4> ecol);
  void drawRect(vec2 _p, vec2 _d, std::array<int, 4> col, bool wire);
  void drawCircle(int x, int y, int radius, std::array<float, 4> _col);
  void drawCircleUI(int x, int y, int radius, std::array<float, 4> _col);
  void drawText(std::string text);

  void queryTexture(std::string identifier, int index, int * w, int * h);
  float getTextureRadius(std::string _identifier);

  //UI drawing
  void drawMap(std::vector<missile> * mp, std::vector<enemy> *ep, std::vector<ship> * ap, std::vector<laser> * lp);
  void statusBars(player * ply);
  void drawWeaponStats(player * ply);

  void finalise();

  SDL_Surface * getSurface(std::string path);*/
};

#endif
