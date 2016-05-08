#ifndef FILE_HPP
#define FILE_HPP

#include <fstream>
#include <string>
#include <sstream>
#include <vector>

#include "enemy.hpp"
#include "ship.hpp"
#include "ui/selection.hpp"
#include "universe.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file file.hpp
/// \brief Performs simple file operations. Mainly used for saving and loading the game.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is the initial version.
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// \brief Saves the game to a txt file
/// \param _uni reference to the universe
//----------------------------------------------------------------------------------------------------------------------
void saveGame(universe * _uni);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Writes essential data out to txt from the universes vector of enemies
/// \param _file the file to write to
/// \param _u a ref to the vector of enemies
//----------------------------------------------------------------------------------------------------------------------
void writeVectorEnemy(std::ostream &_file, std::vector<enemy> * _u);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Writes essential data out to txt from the universes vector of asteroids
/// \param _file the file to write to,
/// _u a ref to the vector of asteroids
//----------------------------------------------------------------------------------------------------------------------
void writeVectorAsteroid(std::ostream &_file, std::vector<ship> * _u);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Reads a string and creates enemies
/// \param _str the string
/// \param _u a ref to the universe
//----------------------------------------------------------------------------------------------------------------------
void readVectorEnemy(std::string _str, universe * _u);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Reads a string and creates asteroids
/// \param _str the string
/// \param _u a ref to the asteroids
//----------------------------------------------------------------------------------------------------------------------
void readVectorAsteroid(std::string _str, universe * _u);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Loads the game stored in save.txt
/// \param _uni ref to the universe
//----------------------------------------------------------------------------------------------------------------------
void loadGame(universe * _uni);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Called during initialisation, reads config.txt and sets important variables
//----------------------------------------------------------------------------------------------------------------------
void loadConfig();

//----------------------------------------------------------------------------------------------------------------------
/// \brief Sets a value in config.txt, used in the options menu
/// \param _entry the entry to set
/// \param _val the value to set _entry to
//----------------------------------------------------------------------------------------------------------------------
void setConfigValue(const std::string _entry, const int _val);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Loads a menu from a text file
/// \param _path path to the file
//----------------------------------------------------------------------------------------------------------------------
ui::selection loadSelection(const std::string _path);

#endif
