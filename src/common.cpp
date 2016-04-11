#include <algorithm>
#include "common.hpp"

game_mode g_GAME_STATE = MODE_MENU;

#if RENDER_MODE == 0
std::string g_RESOURCE_LOC = "../resources/SDL2/";
#elif RENDER_MODE == 1
std::string g_RESOURCE_LOC = "../resources/NGL/";
#endif

double g_GLOBAL_TIME = 0.0f;
int g_WIN_POS_X = 20;
int g_WIN_POS_Y = 20;
int g_WIN_HEIGHT = 1080;
int g_WIN_WIDTH = 1920;
int g_GRAPHICAL_DETAIL = 2;
int g_DIFFICULTY = 1;
vec2 g_HALFWIN;
float g_MAX_DIM = 1920.0f;
bool g_DEV_MODE = true;
bool g_GAME_OVER = false;

float g_TARG_ZOOM_LEVEL = 1.0f;
float g_ZOOM_LEVEL = 1.0f;
const float g_PIXEL_UNIT_CONVERSION = 80.0f;
float g_TIME_SCALE = 1.0f;
float g_BG_DENSITY = 5.0f;
