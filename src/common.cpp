#include "common.hpp"

SDL_Texture * SMOKE_TEXTURE;
SDL_Texture * XPLO_TEXT;

double g_GLOBAL_TIME = 0.f;

int WIN_POS_X = 20, WIN_POS_Y = 20, WIN_HEIGHT = 1080, WIN_WIDTH = 1920, BACKGROUND_DOTS = 4096, DIFFICULTY = 1;
vec2 HALFWIN;
float MAX_DIM = 1920.0f;

bool DEV_MODE = true, GAME_OVER = false;

float TARG_ZOOM_LEVEL = 1.0f, ZOOM_LEVEL = 1.0f, PIXEL_UNIT_CONVERSION = 80.0f, TIME_SCALE = 1.0f;
float BG_DENSITY = 5.0f;

void loadConfig()
{
  std::ifstream config("../resources/config.txt");
  std::string cur;

  while(getline( config, cur ))
  {
    if(cur.length() == 0) continue;

    std::vector<std::string> strings = split(cur, ' ');

    for(size_t i = 0; i < strings.size(); i++)
    {
      if(strings.at(i) == "res_x") WIN_WIDTH = stoi(strings.at(i+1), nullptr, 10);
      else if(strings.at(i) == "res_y") WIN_HEIGHT = stoi(strings.at(i+1), nullptr, 10);
      else if(strings.at(i) == "background_detail") BACKGROUND_DOTS = stoi(strings.at(i+1), nullptr, 10);
      else if(strings.at(i) == "devmode") DEV_MODE = static_cast<int>( stoi(strings.at(i+1), nullptr, 10) );
      else if(strings.at(i) == "difficulty") DIFFICULTY = stoi(strings.at(i+1), nullptr, 10);
    }
  }
  config.close();
  HALFWIN = {WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f};
  MAX_DIM = std::max( WIN_WIDTH, WIN_HEIGHT );

  std::cout << "Resolution: " << WIN_WIDTH << " x " << WIN_HEIGHT << std::endl;
  std::cout << "Difficulty: " << DIFFICULTY << std::endl;
  std::cout << "Background Particles: " << BACKGROUND_DOTS << std::endl;
  std::cout << "Devmode: " << DEV_MODE << std::endl;
}

void loadTextures()
{
  SDL_Surface * temp = IMG_Load("../resources/images/environment/smoke_1.png");
  SMOKE_TEXTURE = SDL_CreateTextureFromSurface(renderer, temp);
  SDL_FreeSurface(temp);

  SDL_Surface * temp2 = IMG_Load("../resources/images/environment/explosion.png");
  XPLO_TEXT = SDL_CreateTextureFromSurface(renderer, temp2);
  SDL_FreeSurface(temp2);
}
