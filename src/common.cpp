#include <algorithm>
#include "common.hpp"

std::string g_RESOURCE_LOC = "../resources/";

double g_GLOBAL_TIME = 0.f;

int g_WIN_POS_X = 20, g_WIN_POS_Y = 20, g_WIN_HEIGHT = 1080, g_WIN_WIDTH = 1920, g_BACKGROUND_DOTS = 4096, g_DIFFICULTY = 1;
vec2 g_HALFWIN;
float g_MAX_DIM = 1920.0f;

bool g_DEV_MODE = true, g_GAME_OVER = false;

float g_TARG_ZOOM_LEVEL = 1.0f, g_ZOOM_LEVEL = 1.0f, g_PIXEL_UNIT_CONVERSION = 80.0f, g_TIME_SCALE = 1.0f;
float g_BG_DENSITY = 5.0f;

void loadConfig()
{
  std::ifstream config( g_RESOURCE_LOC + "config.txt" );
  std::string cur;

  while(getline( config, cur ))
  {
    if(cur.length() == 0) continue;

    std::vector<std::string> strings = split(cur, ' ');

    for(size_t i = 0; i < strings.size(); i++)
    {
      if(strings.at(i) == "res_x") g_WIN_WIDTH = stoi(strings.at(i+1), nullptr, 10);
      else if(strings.at(i) == "res_y") g_WIN_HEIGHT = stoi(strings.at(i+1), nullptr, 10);
      else if(strings.at(i) == "background_detail") g_BACKGROUND_DOTS = stoi(strings.at(i+1), nullptr, 10);
      else if(strings.at(i) == "devmode") g_DEV_MODE = static_cast<int>( stoi(strings.at(i+1), nullptr, 10) );
      else if(strings.at(i) == "g_DIFFICULTY") g_DIFFICULTY = stoi(strings.at(i+1), nullptr, 10);
    }
  }
  config.close();
  g_HALFWIN = {g_WIN_WIDTH / 2.0f, g_WIN_HEIGHT / 2.0f};
  g_MAX_DIM = std::max( g_WIN_WIDTH, g_WIN_HEIGHT );
  std::cout << "g_HALFWIN is " << g_HALFWIN.x << ", " << g_HALFWIN.y << std::endl;

  std::cout << "Resolution: " << g_WIN_WIDTH << " x " << g_WIN_HEIGHT << std::endl;
  std::cout << "g_DIFFICULTY: " << g_DIFFICULTY << std::endl;
  std::cout << "Background Particles: " << g_BACKGROUND_DOTS << std::endl;
  std::cout << "Devmode: " << g_DEV_MODE << std::endl;
}
