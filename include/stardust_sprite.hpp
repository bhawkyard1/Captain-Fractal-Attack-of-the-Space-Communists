#ifndef STARDUST_SPRITE_HPP
#define STARDUST_SPRITE_HPP

#include <string>
#include "SDL2/SDL.h"
#include "stardust.hpp"

class stardust_sprite: public stardust
{
  std::string m_identifier;
  float m_ang, m_angVel;
  float m_dim;
public:
  stardust_sprite(std::string identifier, float *, int, int);
  stardust_sprite(std::string identifier, float, int, int);
  void spriteGen(float*, int, int);
  std::string getTex() {return m_identifier;}
	void updateSprite(float);
	void incrDim();
  float getDim() {return m_dim;}
  float getAng() {return m_ang;}
  std::string getIdentifier() {return m_identifier;}
};

#endif
