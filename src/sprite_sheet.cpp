#include "sprite_sheet.hpp"
#include <SDL_image.h>

void sprite_sheet::destroy()
{
  for(auto i = m_sheet.begin(); i != m_sheet.end(); ++i) SDL_DestroyTexture(*i);
}
