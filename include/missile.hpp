#include "src/ship.cpp"

class missile: public ship
{
	ship * target;
	bool det;
public:
	missile(vec2 p);
	void setTarget(ship * s) {target = s;}
	ship * getTarget() {return target;}
	void steering();
	bool detonate() {return det;}
};
