#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ship.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file player.hpp
/// \brief This file contains the player class..
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class player
/// \brief Builds upon the ship class, but unlike the enemy class adds controls rather than AI.
//----------------------------------------------------------------------------------------------------------------------

class player: public ship
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// \brief ctor for the player
  /// \param _p position
  /// \param _r ship radius
  //----------------------------------------------------------------------------------------------------------------------
  player(vec3 _p);

  //----------------------------------------------------------------------------------------------------------------------
  /// \brief Updates the player, processes some user input
  //----------------------------------------------------------------------------------------------------------------------
  void ctrlUpdate();
};

#endif
