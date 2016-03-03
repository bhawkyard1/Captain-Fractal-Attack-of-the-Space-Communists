#ifndef PFX_HPP
#define PFX_HPP

#include <vector>
#include "base.hpp"
#include <string>

#include "SDL2/SDL_image.h"

class pfx
{
  std::vector<base> particles;
  std::vector<int> alphas;
  float glowA;
  std::string m_identifier;

	int col[3];
	
	vec2 pos, vel, wvel;
	bool active;
public:
  pfx(vec2,vec2,vec2,size_t,float,std::string identifier);
	void update(float);
	bool done() {return !active;}
	void setWVel(vec2 v) {wvel = v;}
  int * getCol() {return col;}
  int getCol(int i) {return col[i];}
  float getAlpha() {return glowA;}
  float getAlpha(int index) {return alphas.at(index);}
  std::string getIdentifier() {return m_identifier;}
  std::vector<base> * getParticles() {return &particles;}
  vec2 getPos() {return pos;}
};

#endif
