#ifndef COMMON_HPP
#define COMMON_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "vectors.hpp"

//void loadTextures();
void loadConfig();

extern std::string g_RESOURCE_LOC;

extern double g_GLOBAL_TIME;

std::vector<std::string> split(std::string, char);

extern int g_WIN_POS_X, g_WIN_POS_Y, g_WIN_HEIGHT, g_WIN_WIDTH, g_BACKGROUND_DOTS, g_DIFFICULTY;
extern vec2 g_HALFWIN;
extern float g_MAX_DIM;

extern bool g_DEV_MODE, g_GAME_OVER;

extern float g_TARG_ZOOM_LEVEL, g_ZOOM_LEVEL, g_PIXEL_UNIT_CONVERSION, g_TIME_SCALE;
extern float g_BG_DENSITY;

#endif
