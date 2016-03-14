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
#include "faction.hpp"

class renderer
{
	//The window we'll be rendering to
  SDL_Window * m_window = NULL;
  SDL_Renderer * m_renderer = NULL;
  int m_w;
  int m_h;
  std::unordered_map<std::string, std::vector<SDL_Texture*>> m_textures;
  std::unordered_map<std::string, sprite_sheet> m_letters;
public:
  renderer(int _w, int _h);
  ~renderer();
  int init();
  void loadTextures();

  void loadFontSpriteSheet(std::string _name, std::string _path, int _size);
  void loadSpriteSheet();
  void loadTexture(std::string _key, std::string _path, SDL_BlendMode _b);
  void loadTextureSet(std::string _key, std::string _set);

  void setBlendMode (SDL_BlendMode _b) {SDL_SetRenderDrawBlendMode(m_renderer, _b);}
  void clear();
  void drawTextureSet(std::string _key, vec2 _pos, float _orient, std::array<float, 4> _alphaMod);
  void drawTexture(std::string _key, size_t _index, vec2 _pos, float _orient, std::array<float, 4> _col);
  void drawText(std::string _text, std::string _font, vec2 _pos, bool _ss, const float _mul);
  void drawLine(vec2 _start, vec2 _end, std::array<float,4> _col );
  void drawLine(vec2 _start, vec2 _end, std::array<int,4> _col);
  void drawLineGr(vec2, vec2, std::array<float, 4> _scol, std::array<float, 4> _ecol);
  void drawRect(vec2 _p, vec2 _d, std::array<int, 4> col, bool wire);
  void drawCircle(int _x, int _y, int _radius, std::array<float, 4> _col);
  void drawCircleUI(int _x, int _y, int _radius, std::array<int, 4> _col);
  void drawText(std::string _text);

  void queryTexture(std::string _identifier, int _index, int * _w, int * _h);
  float getTextureRadius(std::string _identifier);

  //UI drawing
  void drawMap(std::vector<missile> * _mp, std::vector<enemy> * _ep, std::vector<ship> * _ap, std::vector<laser> * _lp, std::vector<faction> * _fp);
  void statusBars(player * _ply);
  void drawWeaponStats(player * _ply);

  void finalise();

  SDL_Surface * getSurface(std::string _path);
};

#endif
