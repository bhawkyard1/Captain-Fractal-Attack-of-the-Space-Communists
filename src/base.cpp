#include "base.hpp"
#include "common.hpp"

void base::updatePos(float dt)
{
	prevPos = pos; 
	pos += (vel + wvel) * PIXEL_UNIT_CONVERSION * dt;
}

vec2 base::getInpterpolatedPosition(float dt)
{
  vec2 p = dt * getPos() + (1 - dt) * getPPos();
  p *= ZOOM_LEVEL;
  p += HALFWIN;

  return p;
}
