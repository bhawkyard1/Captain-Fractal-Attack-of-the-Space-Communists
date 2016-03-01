#include <string>
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
}

void renderer::loadTextures()
{
  loadShipTextureSet("FEDERATION_MKI",      "enemy_1");
  loadShipTextureSet("FEDERATION_MKII",     "enemy_2");
  loadShipTextureSet("FEDERATION_MKIII",    "enemy_3");
  loadShipTextureSet("FEDERATION_MKIV",     "enemy_4");
  loadShipTextureSet("FEDERATION_GUNSHIP",  "enemy_5");

  loadShipTextureSet("PIRATE_GNAT",         "pirate_1");
  loadShipTextureSet("PIRATE_CRUISER",      "pirate_2");
  loadShipTextureSet("PIRATE_WRANGLER",     "pirate_3");
  loadShipTextureSet("PIRATE_MAURAUDER",    "pirate_4");
  loadShipTextureSet("PIRATE_GUNSHIP",      "pirate_5");

  loadShipTextureSet("PLAYER_SHIP",         "player");
  loadShipTextureSet("PLAYER_HUNTER",       "wingman_1");
  loadShipTextureSet("PLAYER_DEFENDER",     "wingman_2");
  loadShipTextureSet("PLAYER_DESTROYER",    "wingman_3");
  loadShipTextureSet("PLAYER_MINER_DROID",  "droid_1");
  loadShipTextureSet("PLAYER_TURRET",       "turret_1");
  loadShipTextureSet("PLAYER_STATION",      "station_1");
  loadShipTextureSet("PLAYER_GRAVWELL",     "well_1");
  loadShipTextureSet("PLAYER_BARRACKS",     "barracks_1");

  loadShipTextureSet("ION_MISSILE_MKI",     "missile");

  loadShipTextureSet("ASTEROID_SMALL",      "asteroid_1");
  loadShipTextureSet("ASTEROID_MID",        "asteroid_2");
  loadShipTextureSet("ASTEROID_LARGE",      "asteroid_3");
}

void renderer::loadShipTextureSet(std::string set, std::string key)
{
  vector<SDL_Surface*> temp_surf;
  temp_surf.push_back( IMG_Load("../resources/textures/" + set + ".png") );
  temp_surf.push_back( IMG_Load("../resources/textures/" + set + "_engines.png") );
  temp_surf.push_back( IMG_Load("../resources/textures/" + set + "_steering.png") );
  temp_surf.push_back( IMG_Load("../resources/textures/" + set + "_shoot.png") );
  temp_surf.push_back( IMG_Load("../resources/textures/" + set + "_shield.png") );
  ship_surf.push_back( IMG_Load("../resources/textures/" + set + "_static.png") );

  vector<SDL_Texture*> temp_tex;
  for(size_t i = 0; i < temp_surf.size(); ++i)
  {
    if(temp_surf.at(i) != nullptr)
    {
      temp_tex.push_back( SDL_CreateTextureFromSurface( m_renderer, temp_surf.at(i) ) );
      SDL_FreeSurface(temp_surf.at(i));
    }
    else
    {
      std::cout << SDL_GetError() << std::endl;
      temp_tex.push_back( nullptr );
    }
  }
  m_textures[key] = temp_tex;
}

void renderer::drawTextureSet(std::string key, vec2 pos, float orient)
{
  int w, h;
  SDL_QueryTexture(m_textures.at(key).at(0), NULL, NULL, &w, &h);

  pos *= ZOOM_LEVEL;
  pos += HALFWIN;

  w *= ZOOM_LEVEL;
  h *= ZOOM_LEVEL;

  SDL_Rect drawRect;
  drawRect.x = p.x - (w/2);
  drawRect.y = p.y - (h/2);
  drawRect.w = w;
  drawRect.h = h;

  SDL_SetTextureAlphaMod(m_textures.at(key).at(1), engineGlow);
  SDL_SetTextureAlphaMod(m_textures.at(key).at(2), steeringGlow);
  SDL_SetTextureAlphaMod(m_textures.at(key).at(3), drawShot);
  SDL_SetTextureAlphaMod(m_textures.at(key).at(4), shieldGlow);
  SDL_SetTextureColorMod(m_textures.at(key).at(3), weapons[curWeap][4], weapons[curWeap][5], weapons[curWeap][6]);

  for(int i = 0 i < 5 ++i) SDL_RenderCopyEx(m_renderer, m_textures.at(key).at(i), NULL, &drawRect, orient, NULL, SDL_FLIP_NONE);
  SDL_RenderCopyEx(m_renderer, m_textures.at(key).at(5), NULL, &drawRect, 0, NULL, SDL_FLIP_NONE);
}
