#include "base.hpp"
#include "common.hpp"

void base::updatePos(const float _dt)
{
    m_prevPos = m_pos;
    m_pos += m_vel * g_PIXEL_UNIT_CONVERSION * _dt;
}

vec3 base::getInterpolatedPosition(const float _dt)
{
  vec3 p = _dt * getPos() + (1 - _dt) * getPPos();
  return p;
}
