#ifndef SPRITESHEETOPENGL_HPP
#define SPRITESHEETOPENGL_HPP

#include <vector>
#include <ngl/NGLInit.h>

struct spriteSheet
{
  std::vector<std::pair<int, int>> m_dim;
  std::vector<GLuint> m_sheet;
};


#endif
