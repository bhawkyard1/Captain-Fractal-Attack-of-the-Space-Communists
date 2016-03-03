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

extern SDL_Texture * SMOKE_TEXTURE;
extern SDL_Texture * XPLO_TEXT;

extern double g_GLOBAL_TIME;

std::vector<std::string> split(std::string, char);

extern int WIN_POS_X, WIN_POS_Y, WIN_HEIGHT, WIN_WIDTH, BACKGROUND_DOTS, DIFFICULTY;
extern vec2 HALFWIN;
extern float MAX_DIM;

extern bool DEV_MODE, GAME_OVER;

extern float TARG_ZOOM_LEVEL, ZOOM_LEVEL, PIXEL_UNIT_CONVERSION, TIME_SCALE;
extern float BG_DENSITY;

#endif
