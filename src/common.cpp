#include <algorithm>
#include "common.hpp"

game_mode g_GAME_STATE = MODE_GAME;

#if RENDER_MODE == 0
std::string g_RESOURCE_LOC = "../resources/";
#elif RENDER_MODE == 1
std::string g_RESOURCE_LOC = "../resources/";
#endif

double g_GLOBAL_TIME = 0.0f;

int g_WIN_POS_X = 20, g_WIN_POS_Y = 20, g_WIN_HEIGHT = 1080, g_WIN_WIDTH = 1920, g_BACKGROUND_DOTS = 4096, g_DIFFICULTY = 1;
vec2 g_HALFWIN;
float g_MAX_DIM = 1920.0f;

bool g_DEV_MODE = true, g_GAME_OVER = false;

float g_TARG_ZOOM_LEVEL = 1.0f, g_ZOOM_LEVEL = 1.0f, g_PIXEL_UNIT_CONVERSION = 80.0f, g_TIME_SCALE = 1.0f;
float g_BG_DENSITY = 5.0f;
