#ifndef FILE_HPP
#define FILE_HPP

#include "universe.hpp"
#include <string>
#include <fstream>
#include <sstream>

void writeVector(std::ostream &file, universe * u, int v);

void saveGame(universe * uni);

void writeVector(std::ostream &file, universe * u, int v);

void readVector(std::string str, universe * u, int v);

void loadGame(universe * uni);

#endif
