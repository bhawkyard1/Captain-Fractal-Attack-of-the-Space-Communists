#ifndef CLASS_BASE
#define CLASS_BASE

#include "base.hpp"

void base::updatePos(float dt)
{
	prevPos = pos; 
	pos += (vel + wvel) * PIXEL_UNIT_CONVERSION * dt;
}

#endif
