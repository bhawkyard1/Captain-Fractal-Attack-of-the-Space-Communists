#ifndef CLASS_PLAYER
#define CLASS_PLAYER

#include "player.hpp"

void player::ctrlUpdate()
{
	vec2 p = getPos();
	p *= ZOOM_LEVEL;
	p += HALFWIN;
	vec2 v = getVel();
	setPos({v.x, v.y});
	setPPos({v.x, v.y});
	int mx = 0, my = 0;
	SDL_GetMouseState(&mx,&my);
	setTAng(computeAngle({p.x - mx, p.y - my}));
}

#endif
