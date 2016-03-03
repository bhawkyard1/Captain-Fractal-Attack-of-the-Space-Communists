#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include <string>
#include "SDL.h"

struct sprite_sheet
{
  SDL_Texture * m_sheet;
  int m_w;
  int m_h;
  void destroy();
};

sprite_sheet create(std::string _path, int _w, int _h);

#endif
