#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <map>

class renderer
{
	//The window we'll be rendering to
  SDL_Window * m_indow = NULL;
  SDL_Renderer * m_renderer = NULL;
  int m_w, m_h;
public:
  renderer(int,int);
  void init();
};

#endif

