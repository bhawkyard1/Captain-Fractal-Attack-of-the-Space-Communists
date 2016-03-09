#ifndef BASE_HPP
#define BASE_HPP

#include "vectors.hpp"
class base
{
	vec2 prevPos, pos, vel, wvel;
public:
	void setPos(vec2 val) {pos = val;}
    vec2 getPos() const {return pos;}
	
	vec2 getPPos() {return prevPos;}
	void setPPos(vec2 p) {prevPos = p;}
	
	void setVel(vec2 v) {vel = v;}
	void setWVel(vec2 wv) {wvel = wv;}
	void addVel(vec2 v) {vel += v;}
	
    vec2 getVel() const {return vel;}
	vec2 getWVel() {return wvel;}
	
	void updatePos(float dt);

  vec2 getInterpolatedPosition(float dt);
};

#endif
