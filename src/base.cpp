#include "base.hpp"
#include "common.hpp"

void base::updatePos(float dt)
{
	prevPos = pos; 
	pos += (vel + wvel) * PIXEL_UNIT_CONVERSION * dt;
}
