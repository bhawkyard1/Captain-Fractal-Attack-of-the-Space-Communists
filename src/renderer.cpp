#include "renderer.hpp"
#include "util.hpp"

renderer::renderer(int _w, int _h)
{
  init();

  m_w = _w;
  m_h = _h;

  m_window = SDL_CreateWindow("Elite: Dangerous v2.0",
                                          WIN_POS_X, WIN_POS_Y,
                                          WIN_HEIGHT, WIN_WIDTH,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED );

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
}

renderer::init()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }

  if(TTF_Init() != 0)
  {
    std::cerr << "TTF_Init() failed: " << TTF_GetError() << std::endl;
    SDL_Quit();
    return EXIT_FAILURE;
  }

  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
  {
    std::cerr << "Mix_OpenAudio() failed! " << SDL_GetError() << std::endl;
    //SDL_Quit();
    //return 1;
  }
}
