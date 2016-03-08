#ifndef FILE_HPP
#define FILE_HPP

#include "universe.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

void writeVector(std::ostream &file, universe * u, int v);

void saveGame(universe * uni);

void writeVector(std::ostream &file, std::vector<enemy> * u);

void readVector(std::string str, universe * u);

void loadGame(universe * uni);

#endif
