#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <map>
#include <vector>

#include <SDL_image.h>

#include "vectors.hpp"

class renderer
{
	//The window we'll be rendering to
  SDL_Window * m_indow = NULL;
  SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;
  std::unordered_map<string, std::vector<SDL_Texture*> m_textures;
public:
  renderer(int,int);
  void init();
  void loadTextures();
  void loadShipTextureSet(std::string set, std::string key);
  void drawTextureSet(std::string key, vec2 pos, float orient)
};

#endif
