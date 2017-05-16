#include "math/ang3.hpp"

vec3 ang3::forward()
{
    if(m_recalc)
        calculateMatrix();
}

vec3 ang3::back()
{
    if(m_recalc)
        calculateMatrix();
}

vec3 ang3::left()
{
    if(m_recalc)
        calculateMatrix();
}

vec3 ang3::right()
{
    if(m_recalc)
        calculateMatrix();
}

vec3 ang3::up()
{
    if(m_recalc)
        calculateMatrix();
}

vec3 ang3::down()
{
    if(m_recalc)
        calculateMatrix();
}

ang3 operator +(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch + rhs.m_pitch;
    ret.m_yaw = lhs.m_yaw + rhs.m_yaw;
    ret.m_roll = lhs.m_roll + rhs.m_roll;
    return ret;
}

ang3 operator +(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch + rhs;
    ret.m_yaw = lhs.m_yaw + rhs;
    ret.m_roll = lhs.m_roll + rhs;
    return ret;
}

ang3 operator +(const float &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs + rhs.m_pitch;
    ret.m_yaw = lhs + rhs.m_yaw;
    ret.m_roll = lhs + rhs.m_roll;
    return ret;
}

ang3 operator -(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch - rhs.m_pitch;
    ret.m_yaw = lhs.m_yaw - rhs.m_yaw;
    ret.m_roll = lhs.m_roll - rhs.m_roll;
    return ret;
}

ang3 operator -(const ang3 &lhs)
{
    ang3 ret;
    ret.m_pitch = -lhs.m_pitch;
    ret.m_yaw = -lhs.m_yaw;
    ret.m_roll = -lhs.m_roll;
    return ret;
}

ang3 operator *(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch * rhs.m_pitch;
    ret.m_yaw = lhs.m_yaw * rhs.m_yaw;
    ret.m_roll = lhs.m_roll * rhs.m_roll;
    return ret;
}

ang3 operator *(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch * rhs;
    ret.m_yaw = lhs.m_yaw * rhs;
    ret.m_roll = lhs.m_roll * rhs;
    return ret;
}

ang3 operator *(const float &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs * rhs.m_pitch;
    ret.m_yaw = lhs * rhs.m_yaw;
    ret.m_roll = lhs * rhs.m_roll;
    return ret;
}

ang3 operator /(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch / rhs.m_pitch;
    ret.m_yaw = lhs.m_yaw / rhs.m_yaw;
    ret.m_roll = lhs.m_roll / rhs.m_roll;
    return ret;
}

ang3 operator /(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.m_pitch = lhs.m_pitch / rhs;
    ret.m_yaw = lhs.m_yaw / rhs;
    ret.m_roll = lhs.m_roll / rhs;
    return ret;
}

ang3 operator ^(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.m_pitch = pow(lhs.m_pitch, rhs);
    ret.m_yaw = pow(lhs.m_yaw, rhs);
    ret.m_roll = pow(lhs.m_roll, rhs);
    return ret;
}

bool operator ==(const ang3 &lhs, const ang3 &rhs)
{
    return (lhs.m_pitch == rhs.m_pitch) and (lhs.m_yaw == rhs.m_yaw) and (lhs.m_roll == rhs.m_roll);
}

void ang3::calculateMatrix()
{
    float p = rad(m_pitch);
    float y = rad(m_yaw);
    float r = rad(m_roll);

    mat3 pm;
    mat3 ym;
    mat3 rm;

    //Clockwise rotation about z
    pm.set(0, -cosf(p));
    pm.set(1, sinf(p));
    pm.set(3, -sinf(p));
    pm.set(4, -cosf(p));

    //Clockwise rotation about y
    ym.set(0, -cosf(y));
    ym.set(2, -sinf(y));
    ym.set(6, sinf(y));
    ym.set(8, -cosf(y));

    //Clockwise rotation about x
    rm.set(4, -cosf(r));
    rm.set(5, sinf(r));
    rm.set(7, -sinf(r));
    rm.set(8, -cosf(r));

    m_transform = pm * ym * rm;
}

vec3 vec(ang3 ang)
{
    return { static_cast<float> (atan2(ang.m_yaw, ang.m_pitch)), static_cast<float> (atan2(ang.m_roll, ang.m_pitch)), 0.0f };
}

ang3 deg(ang3 ang)
{
    return ang * ( 180 / V_PI );
}

ang3 rad(ang3 ang)
{
    return ang * ( V_PI / 180 );
}
