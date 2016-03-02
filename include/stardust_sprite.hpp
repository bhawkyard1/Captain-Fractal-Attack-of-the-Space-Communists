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
	stardust_sprite(SDL_Texture *, float *);
	stardust_sprite(SDL_Texture *, float);
	void spriteGen(float*);
	void draw(float);
	void drawDim(float);
  std::string getTex() {return m_identifier;}
	void updateSprite(float);
	void incrDim();
  float getDim() {return m_dim;}
  float getAng() {return m_ang;}
};

#endif
