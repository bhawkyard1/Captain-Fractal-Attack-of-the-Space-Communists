#ifndef ANG3_HPP
#define ANG3_HPP

#include "mat3.hpp"
#include "vec3.hpp"

struct vec3;

//----------------------------------------------------------------------------------------------------------------------
/// \struct Container for a euler angle with pitch yaw and roll. Not finished.
//----------------------------------------------------------------------------------------------------------------------
struct ang3
{
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Rotation about z axis.
    //----------------------------------------------------------------------------------------------------------------------
    float m_pitch;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Rotation about y axis.
    //----------------------------------------------------------------------------------------------------------------------
    float m_yaw;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Rotation about z axis.
    //----------------------------------------------------------------------------------------------------------------------
    float m_roll;

    mat3 m_transform;
    bool m_recalc;

    ang3():
        m_pitch(0.0f),
        m_yaw(0.0f),
        m_roll(0.0f)
    {}

    ang3(const float _p, const float _y, const float _r) :
    m_pitch(_p),
    m_yaw(_y),
    m_roll(_r)
    {}

    ang3(const ang3& _rhs) = default;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Angle operators.
    //----------------------------------------------------------------------------------------------------------------------
    ang3& operator=(const ang3 &_rhs) { m_pitch = _rhs.m_pitch; m_yaw = _rhs.m_yaw; m_roll = _rhs.m_roll; return *this; }
    ang3& operator+=(ang3 _rhs) { m_pitch += _rhs.m_pitch; m_yaw += _rhs.m_yaw; m_roll += _rhs.m_roll; return *this; }
    ang3& operator-=(ang3 &_rhs) { m_pitch -= _rhs.m_pitch; m_yaw -= _rhs.m_yaw; m_roll -= _rhs.m_roll; return *this; }
    ang3& operator-=(float &_rhs) { m_pitch -= _rhs; m_yaw -= _rhs; m_roll -= _rhs; return *this; }

    void rotate(const float _p, const float _y, const float _r)
    {
        m_pitch = _p;
        m_yaw = _y;
        m_roll = _r;
        m_recalc = true;
    }

    void pitch(const float _v)
    {
        m_pitch = _v;
        m_recalc = true;
    }

    void yaw(const float _v)
    {
        m_yaw = _v;
        m_recalc = true;
    }

    void roll(const float _v)
    {
        m_roll = _v;
        m_recalc = true;
    }

    void calculateMatrix();
    vec3 forward();
    vec3 back();
    vec3 left();
    vec3 right();
    vec3 up();
    vec3 down();
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief More angle operators.
//----------------------------------------------------------------------------------------------------------------------
ang3 operator +(const ang3 &_lhs, const ang3 &_rhs);
ang3 operator +(const ang3 &_lhs, const float &_rhs);
ang3 operator +(const float &_lhs, const ang3 &_rhs);
ang3 operator -(const ang3 &_lhs, const ang3 &_rhs);
ang3 operator -(const ang3 &_lhs);
ang3 operator *(const ang3 &_lhs, const ang3 &_rhs);
ang3 operator *(const ang3 &_lhs, const float &_rhs);
ang3 operator *(const float &_lhs, const ang3 &_rhs);
ang3 operator /(const ang3 &_lhs, const ang3 &_rhs);
ang3 operator /(const ang3 &_lhs, const float &_rhs);
ang3 operator ^(const ang3 &_lhs, const float &_rhs);
bool operator ==(const ang3 &_lhs, const ang3 &_rhs);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts a 3d vector to an angle.
//----------------------------------------------------------------------------------------------------------------------
vec3 vec(const ang3 &_ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts an angle between degrees and radians.
//----------------------------------------------------------------------------------------------------------------------
ang3 deg(const ang3 &_ang);
ang3 rad(const ang3 &_ang);

#endif
