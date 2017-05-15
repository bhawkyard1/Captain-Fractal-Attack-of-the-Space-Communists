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

vec3 base::forwards() const
{
    ang3 ang = rad(m_angle);
    return vec3(
                cos(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_pitch)
                );
}

vec3 base::backwards() const
{
    ang3 ang = rad(m_angle + ang3(0.0f, 180.0f, 0.0f));
    return vec3(
                cos(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_pitch)
                );
}

vec3 base::right() const
{
    ang3 ang = rad(m_angle + ang(0.0f, 90.0f, 0.0f));
    return vec3(
                cos(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_pitch)
                );
}

vec3 base::left() const
{
    ang3 ang = rad(m_angle + ang(0.0f, -90.0f, 0.0f));
    return vec3(
                cos(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_pitch)
                );
}

vec3 base::up() const
{
    ang3 ang = rad(m_angle + ang(90.0f, 0.0f, 0.0f));
    return vec3(
                cos(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_pitch)
                );
}

vec3 base::down() const
{
    ang3 ang = rad(m_angle + ang3(-90.0f, 0.0f, 0.0f));
    return vec3(
                cos(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_yaw) * cos(ang.m_pitch),
                sin(ang.m_pitch)
                );
}
