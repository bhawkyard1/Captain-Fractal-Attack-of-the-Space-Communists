#ifndef COMMON_HPP
#define COMMON_HPP

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "vectors.hpp"

#define RENDER_MODE 0

//void loadTextures();
void loadConfig();

enum game_mode {MODE_MENU, MODE_TUTORIAL, MODE_GAME, MODE_QUIT};
extern game_mode g_GAME_STATE;

extern std::string g_RESOURCE_LOC;

extern double g_GLOBAL_TIME;

std::vector<std::string> split(std::string, char);

extern int g_WIN_POS_X;
extern int g_WIN_POS_Y;
extern int g_WIN_HEIGHT;
extern int g_WIN_WIDTH;

extern int g_BACKGROUND_DOTS;

extern int g_DIFFICULTY;

extern vec2 g_HALFWIN;
extern float g_MAX_DIM;

extern bool g_DEV_MODE;
extern bool g_GAME_OVER;

extern float g_TARG_ZOOM_LEVEL;
extern float g_ZOOM_LEVEL;
extern float g_PIXEL_UNIT_CONVERSION;
extern float g_TIME_SCALE;
extern float g_BG_DENSITY;

#endif
