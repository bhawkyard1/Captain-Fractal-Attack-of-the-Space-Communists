#ifndef COMMON_HPP
#define COMMON_HPP

#ifdef _WIN32
#include <ciso646>
#endif

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <SDL.h>
#include <SDL2/SDL_image.h>

#include "vectors.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \brief 0 = Rendered using SDL, 1 = Rendered using NGL.
//----------------------------------------------------------------------------------------------------------------------
#define RENDER_MODE 1

//----------------------------------------------------------------------------------------------------------------------
/// \file common.hpp
/// \brief Keeps track of the global variables used in the game.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// \brief Identifies the game state, whether the user is at the main menu, playing a normal game, the tutorial, devmode sandbox, or wants to quit the game
//----------------------------------------------------------------------------------------------------------------------
enum game_mode {MODE_MENU, MODE_TUTORIAL, MODE_GAME, MODE_QUIT, MODE_SANDBOX};
extern game_mode g_GAME_STATE;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Locations of game resources, differs for NGL and SDL versions
//----------------------------------------------------------------------------------------------------------------------
extern std::string g_RESOURCE_LOC;

//----------------------------------------------------------------------------------------------------------------------
/// \brief The time elapsed since the game started
//----------------------------------------------------------------------------------------------------------------------
extern double g_GLOBAL_TIME;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Splits a string given a character delimiter
/// \param _str input string, _delim delimiting character
//----------------------------------------------------------------------------------------------------------------------
std::vector<std::string> split(const std::string _str, const char _delim);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Positions and dimensions of the window
//----------------------------------------------------------------------------------------------------------------------
extern int g_WIN_HEIGHT;
extern int g_WIN_WIDTH;
extern int g_WIN_POS_X;
extern int g_WIN_POS_Y;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Graphical fidelity
//----------------------------------------------------------------------------------------------------------------------
extern int g_GRAPHICAL_DETAIL;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Difficulty level of the game
//----------------------------------------------------------------------------------------------------------------------
extern int g_DIFFICULTY;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Half of the window size, as a vec2
//----------------------------------------------------------------------------------------------------------------------
extern vec2 g_HALFWIN;

extern vec2 g_VIEWPOS;

//----------------------------------------------------------------------------------------------------------------------
/// \brief The maximum dimension of the window, used in some places to cull objects which are too far away, in the SDL version
//----------------------------------------------------------------------------------------------------------------------
#ifdef RENDER_MODE == 0
extern float g_MAX_DIM;
#endif

//----------------------------------------------------------------------------------------------------------------------
/// \brief Whether the game is in developer mode (this can be changed in config.txt, in the resources folder)
//----------------------------------------------------------------------------------------------------------------------
extern bool g_DEV_MODE;

//----------------------------------------------------------------------------------------------------------------------
/// \brief This makes the game very very hard, but it is useful for testing the combat.
//----------------------------------------------------------------------------------------------------------------------
extern bool g_BEAST_MODE;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Whether the game is over, typically used to tell the game to ignore the player
//----------------------------------------------------------------------------------------------------------------------
extern bool g_GAME_OVER;

//----------------------------------------------------------------------------------------------------------------------
/// \brief The target zoom level
//----------------------------------------------------------------------------------------------------------------------
extern float g_TARG_ZOOM_LEVEL;

//----------------------------------------------------------------------------------------------------------------------
/// \brief The current zoom level
//----------------------------------------------------------------------------------------------------------------------
extern float g_ZOOM_LEVEL;

//----------------------------------------------------------------------------------------------------------------------
/// \brief A multiplier for calculations using dt
//----------------------------------------------------------------------------------------------------------------------
extern const float g_PIXEL_UNIT_CONVERSION;

//----------------------------------------------------------------------------------------------------------------------
/// \brief The time scale
//----------------------------------------------------------------------------------------------------------------------
extern float g_TIME_SCALE;

//----------------------------------------------------------------------------------------------------------------------
/// \brief Density of background detail
//----------------------------------------------------------------------------------------------------------------------
extern float g_BG_DENSITY;

#endif
