#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "ship.hpp"

class missile: public ship
{
	ship * target;
	bool det;
public:
    missile(vec2 p, float _r);
	void setTarget(ship * s) {target = s;}
	ship * getTarget() {return target;}
	void steering();
	bool detonate() {return det;}
};

#endif
