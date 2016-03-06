#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include <vector>
#include <string>
#include "SDL.h"

struct sprite_sheet
{
  std::vector<SDL_Texture *>  m_sheet;
  void destroy();
};

#endif
