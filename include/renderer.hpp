#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include <SDL_image.h>

#include "sprite_sheet.hpp"
#include "vectors.hpp"

class renderer
{
	//The window we'll be rendering to
  SDL_Window * m_window = NULL;
  SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;
  std::unordered_map<std::string, std::vector<SDL_Texture*>> m_textures;
  std::unordered_map<std::string, sprite_sheet> m_letters;
public:
  renderer(int,int);
  ~renderer();
  void init();
  void loadTextures();

  void loadSpriteSheet();
  void loadTexture(std::string _key, std::string _path);
  void loadTextureSet(std::string _key, std::string _set);

  void clear();
  void drawTextureSet(std::string key, vec2 pos, float orient);
  void drawTexture(std::string key, size_t index, vec2 pos, float orient, float col[]);
  void drawLine(vec2 _start, vec2 _end, int _col[]);
  void drawLineGr(vec2, vec2, int scol[], int ecol[]);
  void drawCircle(int x, int y, int radius);
  void drawText(std::string text);

  //UI drawing
  void drawMap(std::vector<missile> * mp, std::vector<ship> * ap, std::vector<laser> * lp);
  void statusBars(player * ply);

  void finalise();
};

#endif
