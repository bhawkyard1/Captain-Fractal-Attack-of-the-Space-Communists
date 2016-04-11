#ifndef LASER_HPP
#define LASER_HPP

#include "base.hpp"
#include "enemy.hpp"
#include "weapons.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// @file laser.hpp
/// @brief Contains the laser class.
/// @author Ben Hawkyard
/// @version 1.0
/// @date 11/04/16
/// Revision History :
/// This is the initial version.
/// @class laser
/// @brief Inherits from base, represents a laser and contains data such as faction, damage, colour etc.
//----------------------------------------------------------------------------------------------------------------------

class laser: public base
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief ctor for the laser class
  /// @param _p position, _v velocity, _ang angle, _data copied from weapons.hpp, _team the lasers' team
  //----------------------------------------------------------------------------------------------------------------------
  laser(vec2 _p, vec2 _v, float _ang, std::array<float, WEAPS_W> _data, aiTeam _team);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns damage
  //----------------------------------------------------------------------------------------------------------------------
  int getDmg() const {return m_dmg;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns stopping power
  //----------------------------------------------------------------------------------------------------------------------
  float getStop() const {return m_stop;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief updates the laser
  /// @param _dt time difference
  //----------------------------------------------------------------------------------------------------------------------
  void update(float _dt);

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the team
  //----------------------------------------------------------------------------------------------------------------------
  aiTeam getTeam() const  {return m_team;}

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief returns the colour of
  //----------------------------------------------------------------------------------------------------------------------
  std::array<float, 4> getCol() const {return {m_col[0], m_col[1], m_col[2], 255.0f};}
  float getCol(int _index) const {return m_col[_index];}
  float getPower() const {return m_power;}
  float getAng() const {return m_ang;}
private:
  int m_dmg;
  int m_spd;
  aiTeam m_team;
  float m_ang;
  std::array<float, 3> m_col;
  float m_power;
  float m_stop;
};

#endif

