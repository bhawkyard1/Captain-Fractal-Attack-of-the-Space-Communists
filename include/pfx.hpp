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
	void draw(float);
	bool done() {return !active;}
	void setWVel(vec2 v) {wvel = v;}
  float getAlpha() {return glowA;}
  std::string getIdentifier() {return m_identifier;}
};

#endif
