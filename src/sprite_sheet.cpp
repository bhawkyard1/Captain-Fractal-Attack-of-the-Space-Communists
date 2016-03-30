#include "sprite_sheet_opengl.hpp"
#include <SDL_image.h>

void spriteSheet::destroy()
{
  for(auto i = m_sheet.begin(); i != m_sheet.end(); ++i) SDL_DestroyTexture(*i);
}
