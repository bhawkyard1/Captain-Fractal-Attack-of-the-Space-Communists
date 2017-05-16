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

vec3 base::getIPos(const float _dt)
{
    vec3 p = _dt * getPos() + (1 - _dt) * getPPos();
    return p;
}

vec3 base::forward()
{
    return m_angle.forward();
}

vec3 base::back()
{
    return m_angle.back();
}

vec3 base::right()
{
    return m_angle.right();
}

vec3 base::left()
{
   return m_angle.left();
}

vec3 base::up()
{
    return m_angle.up();
}

vec3 base::down()
{
    return m_angle.down();
}
