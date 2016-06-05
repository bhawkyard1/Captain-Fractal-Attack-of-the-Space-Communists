#ifndef WEAPONS_HPP
#define WEAPONS_HPP

#define WEAPS_W 10
#define WEAPS_H 18

#include "common.hpp"
#include "util.hpp"
#include <array>

//----------------------------------------------------------------------------------------------------------------------
/// \file weapons.hpp
/// \brief Weapon related types and stats live here.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// \brief This type represents the different attributes of a given weapon. They are analogous to the index containing weapon
/// data since they can be implicity converted to integers.
//----------------------------------------------------------------------------------------------------------------------
enum WEAPON_STAT {LASER_COUNT, SPREAD, DAMAGE, SPEED, COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE, ENERGY_COST, COOLDOWN, STOPPING_POWER};

//----------------------------------------------------------------------------------------------------------------------
/// \brief Enum representing the different weapon types that exist. Can be used to index into our global weapon stat array.
//----------------------------------------------------------------------------------------------------------------------
enum WEAPON_TYPE {
    WEAPON_PLAYER_RED, WEAPON_PLAYER_GREEN, WEAPON_PLAYER_BLUE,
    WEAPON_DEBUG_YELLOW,
    WEAPON_FED_1, WEAPON_FED_2, WEAPON_FED_3, WEAPON_FED_TURRET, WEAPON_FED_BOSS_1, WEAPON_FED_BOSS_2,
    WEAPON_WINGMAN_1, WEAPON_WINGMAN_2, WEAPON_WINGMAN_3,
    WEAPON_MINER_LASER,
    WEAPON_TURRET_LASER,
    WEAPON_PIRATE_1, WEAPON_PIRATE_2, WEAPON_PIRATE_3, WEAPON_PIRATE_BOSS_1,
    WEAPON_COMMUNIST_1, WEAPON_COMMUNIST_2, WEAPON_COMMUNIST_CAPITAL,
    WEAPON_ALLIANCE_1, WEAPON_ALLIANCE_2, WEAPON_ALLIANCE_3, WEAPON_ALLIANCE_BOSS,
    WEAPON_END
};


//----------------------------------------------------------------------------------------------------------------------
/// \brief Global array containing base stats of all weapons in-game. These are copied out to ships upon their construction.
//----------------------------------------------------------------------------------------------------------------------
static std::array<std::array<float,10>,27> g_weapons =
{{
     {{1.0f,	2.0f,                           8.0f,       12.0f,		255.0f,	50.0f,	50.0f,	4.0f,   0.2f,   0.008f}},//Laser cannon	0
     {{12.0f,	5.0f,                           3.0f,       8.0f,       50.0f,	255.0f,	65.0f,	10.0f,	0.5f,   0.01f}},//Shotgun			1
     {{1.0f,	1.0f,                           2.0f,       30.0f,		40.0f,	75.0f,	255.0f,	1.0f,   0.05f,  0.001f}},//Blue laser		2
     {{1.0f,	1.0f,                           2000.0f,    100.0f,		255.0f,	210.0f,	0.0f,   0.0f,   0.02f,  0.01f}},//Yellow laser	3
     {{1.0f,	3.0f/clamp(g_DIFFICULTY, 1, 9), 10.0f,      10.0f,		255.0f,	20.0f,	240.0f,	1.0f,   0.8f,   0.002f}},//AI laser 1		4
     {{3.0f,	9.0f/clamp(g_DIFFICULTY, 1, 9),	8.0f,       10.0f,      255.0f,	20.0f,	240.0f,	0.5f,   1.0f,   0.003f}},//AI laser 2		5
     {{1.0f,	2.5f/clamp(g_DIFFICULTY, 1, 9),	4.0f,       15.0f,		255.0f,	20.0f,	240.0f,	0.3f,   0.7f,   0.004f}},//AI laser 3		6
     {{1.0f,	3.0f/clamp(g_DIFFICULTY, 1, 9), 10.0f,      10.0f,		255.0f,	20.0f,	240.0f,	1.0f,   0.8f,   0.002f}},
     {{30.0f,	2.0f/clamp(g_DIFFICULTY, 1, 9),	4.0f,       6.0f,       255.0f,	30.0f,	80.0f,	1.0f,   1.0f,   0.008f}},//Boss laser 1	7
     {{1.0f,	1.5f/clamp(g_DIFFICULTY, 1, 9),	2.0f,       20.0f,		255.0f,	30.0f,	80.0f,	1.0f,   0.05f,  0.009f}},//Boss laser 2	8
     {{2.0f,	0.8f/clamp(g_DIFFICULTY, 1, 9),	10.0f,		24.0f,		20.0f,	255.0f,	40.0f,	0.8f,   0.8f,   0.01f}},//Wingman laser	9
     {{1.0f,	1.0f/clamp(g_DIFFICULTY, 1, 9),	1.0f,       40.0f,		20.0f,	255.0f,	40.0f,	0.4f,   0.2f,   0.01f}},//Wingman laser	10
     {{1.0f,	0.8f/clamp(g_DIFFICULTY, 1, 9),	20.0f,		5.0f,       20.0f,	255.0f,	40.0f,	1.0f,   1.0f,   0.01f}},//Wingman laser	11
     {{1.0f,	0.1f/clamp(g_DIFFICULTY, 1, 9),	100.0f,		40.0f,		20.0f,	255.0f,	40.0f,	0.2f,   5.0f,   0.002f}},//Mining laser	12
     {{1.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	6.0f,       20.0f,		20.0f,	255.0f,	40.0f,	1.0f,   0.9f,   0.03f}},//Turret laser	13
     {{1.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	4.0f,       10.0f,		255.0f,	216.0f,	0.0f,   0.8f,   0.5f,   0.004f}},//Pirate laser 1	14
     {{1.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	5.0f,       10.0f,		255.0f,	216.0f,	0.0f,   0.8f,   0.5f,   0.004f}},//Pirate laser 2	15
     {{1.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	6.0f,       10.0f,		255.0f,	216.0f,	0.0f,   0.8f,   0.5f,   0.004f}},//Pirate laser 3	16
     {{3.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	12.0f,       10.0f,		255.0f,	216.0f,	0.0f,   1.0f,   0.9f,   0.008f}},//P gunship laser	17
     {{1.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	4.0f,       15.0f,		255.0f,	0.0f,	0.0f,   1.0f,   0.9f,   0.004f}},//Commie laser 1 18
     {{2.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	5.0f,       10.0f,		255.0f,	0.0f,	0.0f,   1.0f,   0.7f,   0.004f}},//Commie laser 2 19
     {{15.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	20.0f,      10.0f,		255.0f,	0.0f,	0.0f,   1.0f,   0.5f,   0.004f}},//Commie laser capital 20
     {{1.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	8.0f,       18.0f,		0.0f,	200.0f,	255.0f, 3.0f,   0.9f,   0.008f}},//Alliance 1
     {{5.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	3.0f,       15.0f,		0.0f,	200.0f,	255.0f, 2.0f,   0.9f,   0.009f}},//Alliance 2
     {{2.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	2.0f,       24.0f,		0.0f,	200.0f,	255.0f, 0.8f,   0.1f,   0.002f}},//Alliance 3
     {{2.0f,	0.5f/clamp(g_DIFFICULTY, 1, 9),	20.0f,      50.0f,		0.0f,	200.0f,	255.0f, 0.8f,   0.1f,   0.3f}},//Alliance boss
     {{0.0f,    0.0f,                           0.0f,       0.0f,       0.0f,   0.0f,   0.0f,   0.0f,   0.0f,   0.0f}}   //Null 20
}};

inline float getEnergyCost(int weap)
{
    return g_weapons[weap][ENERGY_COST];
}

inline float getCooldown(int weap)
{
    return g_weapons[weap][COOLDOWN];
}

inline bool isOffScreen(const vec2 &pos, float dist)
{
    return (pos.m_x < -dist or pos.m_y < -dist or pos.m_x > dist or pos.m_y > dist);
}

inline bool isOffScreen(const vec3 &pos, float dist)
{
    return (pos.m_x < -dist or pos.m_y < -dist or pos.m_x > dist or pos.m_y > dist);
}

#endif
