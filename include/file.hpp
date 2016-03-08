#ifndef FILE_HPP
#define FILE_HPP

#include "ship.hpp"
#include "enemy.hpp"
#include "universe.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <vector>

void writeVector(std::ostream &file, universe * u, int v);

void saveGame(universe * uni);

void writeVectorEnemy(std::ostream &file, std::vector<enemy> * u);
void writeVectorAsteroid(std::ostream &file, std::vector<ship> * u);

void readVectorEnemy(std::string str, universe * u);
void readVectorAsteroid(std::string str, universe * u);

void loadGame(universe * uni);

#endif
