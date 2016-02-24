#ifndef PFX_HPP
#define PFX_HPP

#include <vector>
#include "base.hpp"

#include "SDL2/SDL_image.h"

class pfx
{
  std::vector<base> particles;
  std::vector<int> alphas;
	
	SDL_Texture * glow;
	float glowA;
	
	int col[3];
	
	vec2 pos, vel, wvel;
	bool active;
public:
	pfx(vec2,vec2,vec2,size_t,float,SDL_Texture*);
	void update(float);
	void draw(float);
	bool done() {return !active;}
	void setWVel(vec2 v) {wvel = v;}
	
	void clear() {SDL_DestroyTexture(glow);}
};

#endif
