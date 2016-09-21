#include "base.hpp"
#include "common.hpp"

base::base()
{
    m_forces = vec3();
}

void base::updatePos(const float _dt)
{
    m_prevPos = m_pos;
    m_pos += (m_vel + m_wvel) * g_PIXEL_UNIT_CONVERSION * _dt;
    m_vel += m_forces * g_PIXEL_UNIT_CONVERSION * _dt;
    m_forces = vec3();
}

vec3 base::getInterpolatedPosition(const float _dt)
{
  vec3 p = _dt * getPos() + (1 - _dt) * getPPos();
  return p;
}
