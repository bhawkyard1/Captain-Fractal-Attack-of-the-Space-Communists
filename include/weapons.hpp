#ifndef WEAPONS_HPP
#define WEAPONS_HPP

#define WEAPS_W 10
#define WEAPS_H 18

#include "common.hpp"
#include <array>

enum WEAPON_STAT {LASER_COUNT, SPREAD, DAMAGE, SPEED, COLOUR_RED, COLOUR_GREEN, COLOUR_BLUE, ENERGY_COST, COOLDOWN, STOPPING_POWER};

//Weapon definitions.
//0 Lasers per shot, 1 spread, 2 dmg, 3 speed, 4 red, 5 green, 6 blue, 7 energy cost, 8 cooldown, 9 stopping power.
static std::array<std::array<float,10>,18> weapons =
{{
     {{1.0f,	2.0f,               8.0f,       12.0f,		255.0f,	50.0f,	50.0f,	4.0f,   0.2f,   0.008f}},//Laser cannon	0
     {{12.0f,	5.0f,               3.0f,       8.0f,       50.0f,	255.0f,	65.0f,	10.0f,	0.5f,   0.01f}},//Shotgun			1
     {{1.0f,	1.0f,               2.0f,       30.0f,		40.0f,	75.0f,	255.0f,	1.0f,   0.05f,  0.001f}},//Blue laser		2
     {{1.0f,	1.0f,               2000.0f,    100.0f,		255.0f,	210.0f,	0.0f,   0.0f,   0.02f,  0.01f}},//Yellow laser	3
     {{1.0f,	3.0f/g_DIFFICULTY,    10.0f,       10.0f,		255.0f,	20.0f,	240.0f,	1.0f,   0.8f,   0.002f}},//AI laser 1		4
     {{3.0f,	9.0f/g_DIFFICULTY,	8.0f,       5.0f,       255.0f,	20.0f,	240.0f,	0.5f,   1.0f,   0.003f}},//AI laser 2		5
     {{1.0f,	2.5f/g_DIFFICULTY,	4.0f,       15.0f,		255.0f,	20.0f,	240.0f,	0.3f,   0.7f,   0.004f}},//AI laser 3		6
     {{30.0f,	2.0f/g_DIFFICULTY,	4.0f,       6.0f,       255.0f,	30.0f,	80.0f,	1.0f,   1.0f,   0.008f}},//Boss laser 1	7
     {{1.0f,	1.5f/g_DIFFICULTY,	2.0f,       20.0f,		255.0f,	30.0f,	80.0f,	1.0f,   0.05f,  0.009f}},//Boss laser 2	8
     {{2.0f,	0.8f/g_DIFFICULTY,	10.0f,		24.0f,		20.0f,	255.0f,	40.0f,	0.8f,   0.8f,   0.01f}},//Wingman laser	9
     {{1.0f,	1.0f/g_DIFFICULTY,	1.0f,       40.0f,		20.0f,	255.0f,	40.0f,	0.4f,   0.2f,   0.01f}},//Wingman laser	10
     {{1.0f,	0.8f/g_DIFFICULTY,	20.0f,		5.0f,       20.0f,	255.0f,	40.0f,	1.0f,   1.0f,   0.01f}},//Wingman laser	11
     {{1.0f,	0.1f/g_DIFFICULTY,	100.0f,		40.0f,		20.0f,	255.0f,	40.0f,	0.2f,   5.0f,   0.002f}},//Mining laser	12
     {{1.0f,	0.5f/g_DIFFICULTY,	6.0f,       20.0f,		20.0f,	255.0f,	40.0f,	1.0f,   0.9f,   0.03f}},//Turret laser	13
     {{1.0f,	0.5f/g_DIFFICULTY,	7.0f,       10.0f,		255.0f,	216.0f,	0.0f,   0.8f,   0.5f,   0.004f}},//Pirate laser 1	14
     {{1.0f,	0.5f/g_DIFFICULTY,	7.0f,       10.0f,		255.0f,	216.0f,	0.0f,   0.8f,   0.5f,   0.004f}},//Pirate laser 2	15
     {{1.0f,	0.5f/g_DIFFICULTY,	7.0f,       10.0f,		255.0f,	216.0f,	0.0f,   0.8f,   0.5f,   0.004f}},//Pirate laser 3	16
     {{3.0f,	0.5f/g_DIFFICULTY,	7.0f,       10.0f,		255.0f,	216.0f,	0.0f,   1.0f,   0.9f,   0.008f}},//P gunship laser	17
 }};

inline float getEnergyCost(int weap)
{
    return weapons[weap][7];
}

inline float getCooldown(int weap)
{
    return weapons[weap][8];
}

inline bool isOffScreen(vec2 pos, float dist)
{
    return (pos.x < -dist or pos.y < -dist or pos.x > dist or pos.y > dist);
}

#endif
