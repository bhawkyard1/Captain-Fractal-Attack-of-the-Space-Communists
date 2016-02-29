#ifndef SHIP_PRESETS_HPP
#define SHIP_PRESETS_HPP

#include <vector>
#include "ship.hpp"

extern std::vector<ship> GLOBAL_SHIPS;

//This function loads all the ships in the game into a global vector that we can copy from later.
void loadShips();

#endif
