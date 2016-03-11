#ifndef STARDUST_SPRITE_HPP
#define STARDUST_SPRITE_HPP

#include <array>
#include <string>
#include "stardust.hpp"

class stardust_sprite: public stardust
{
  std::string m_identifier;
  float m_ang, m_angVel;
  float m_dim;
public:
  stardust_sprite(const std::string _identifier, const std::array<float, 3> _col, const int _w, const int _h);
  stardust_sprite(const std::string _identifier, const float _alph, const int _w, const int _h);
  void spriteGen(const std::array<float, 3> &_col, const int _w, const int _h);
  std::string getTex() const {return m_identifier;}
  void updateSprite(const float _dt);
	void incrDim();
  float getDim() const {return m_dim;}
  float getAng() const {return m_ang;}
  std::string getIdentifier() {return m_identifier;}
};

#endif
