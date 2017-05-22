#include "math/ang3.hpp"


//delete
#include <iostream>

vec3 ang3::forward()
{
    if(m_recalc)
        calculateMatrix();
    return vec3(
                m_transform.get(2, 0),
                m_transform.get(2, 1),
                m_transform.get(2, 2)
                );
}

vec3 ang3::back()
{
    if(m_recalc)
        calculateMatrix();
    return vec3(
                -m_transform.get(1, 0),
                -m_transform.get(1, 1),
                -m_transform.get(1, 2)
                );
}

vec3 ang3::left()
{
    if(m_recalc)
        calculateMatrix();
    return vec3(
                -m_transform.get(0, 0),
                -m_transform.get(0, 1),
                -m_transform.get(0, 2)
                );
}

vec3 ang3::right()
{
    if(m_recalc)
        calculateMatrix();
    return vec3(
                m_transform.get(0, 0),
                m_transform.get(0, 1),
                m_transform.get(0, 2)
                );
}

vec3 ang3::up()
{
    if(m_recalc)
        calculateMatrix();
    return vec3(
                m_transform.get(1, 0),
                m_transform.get(1, 1),
                m_transform.get(1, 2)
                );
}

vec3 ang3::down()
{
    if(m_recalc)
        calculateMatrix();
    return vec3(
                -m_transform.get(1, 0),
                -m_transform.get(1, 1),
                -m_transform.get(1, 2)
                );
}

ang3 operator +(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret (
                lhs.getPitch() + rhs.getPitch(),
                lhs.getYaw() + rhs.getYaw(),
                lhs.getRoll() + rhs.getRoll()
                );
    return ret;
}

ang3 operator +(const ang3 &lhs, const float &rhs)
{
    ang3 ret (
                lhs.getPitch() + rhs,
                lhs.getYaw() + rhs,
                lhs.getRoll() + rhs
                );
    return ret;
}

ang3 operator +(const float &lhs, const ang3 &rhs)
{
    ang3 ret (
                lhs + rhs.getPitch(),
                lhs + rhs.getYaw(),
                lhs + rhs.getRoll()
                );
    return ret;
}

ang3 operator -(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret (
                lhs.getPitch() - rhs.getPitch(),
                lhs.getYaw() - rhs.getYaw(),
                lhs.getRoll() - rhs.getRoll()
                );
    return ret;
}

ang3 operator -(const ang3 &lhs)
{
    ang3 ret (
                -lhs.getPitch(),
                -lhs.getYaw(),
                -lhs.getRoll()
                );
    return ret;
}

ang3 operator *(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret (
                lhs.getPitch() * rhs.getPitch(),
                lhs.getYaw() * rhs.getYaw(),
                lhs.getRoll() * rhs.getRoll()
                );
    return ret;
}

ang3 operator *(const ang3 &lhs, const float &rhs)
{
    ang3 ret (
                lhs.getPitch() * rhs,
                lhs.getYaw() * rhs,
                lhs.getRoll() * rhs
                );
    return ret;
}

ang3 operator *(const float &lhs, const ang3 &rhs)
{
    ang3 ret (
                lhs * rhs.getPitch(),
                lhs * rhs.getYaw(),
                lhs * rhs.getRoll()
                );
    return ret;
}

ang3 operator /(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret (
                lhs.getPitch() / rhs.getPitch(),
                lhs.getYaw() / rhs.getYaw(),
                lhs.getRoll() / rhs.getRoll()
                );
    return ret;
}

ang3 operator /(const ang3 &lhs, const float &rhs)
{
    ang3 ret (
                lhs.getPitch() / rhs,
                lhs.getYaw() / rhs,
                lhs.getRoll() / rhs
                );
    return ret;
}

ang3 operator ^(const ang3 &lhs, const float &rhs)
{
    ang3 ret (
                pow(lhs.getPitch(), rhs),
                pow(lhs.getYaw(), rhs),
                pow(lhs.getRoll(), rhs)
                );
    return ret;
}

bool operator ==(const ang3 &lhs, const ang3 &rhs)
{
    return (lhs.getPitch() == rhs.getPitch()) and (lhs.getYaw() == rhs.getYaw()) and (lhs.getRoll() == rhs.getRoll());
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

    m_recalc = false;
}

ang3 deg(ang3 ang)
{
    return ang * ( 180 / V_PI );
}

ang3 rad(ang3 ang)
{
    return ang * ( V_PI / 180 );
}
