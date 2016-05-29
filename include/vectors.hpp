#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <math.h>

#define V_PI 3.14159265359
struct vec2;
struct vec3;

//----------------------------------------------------------------------------------------------------------------------
/// \file vector.hpp
/// \brief A small vector math library. This library was also used in my specialist project, and should not be marked.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game. Can also be found in my specialist project, and so should not be marked.
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// \struct A 2d vector, holds an x and y component, and has overloaded operators to make the syntax nicer.
//----------------------------------------------------------------------------------------------------------------------
struct vec2
{
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief X component.
    //----------------------------------------------------------------------------------------------------------------------
    float m_x;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Y component.
    //----------------------------------------------------------------------------------------------------------------------
    float m_y;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief In-place operators.
    //----------------------------------------------------------------------------------------------------------------------
    vec2& operator=(const vec2 &_rhs) { m_x = _rhs.m_x; m_y = _rhs.m_y; return *this; }
    vec2 operator+=(vec2 _rhs) { m_x += _rhs.m_x; m_y += _rhs.m_y; return *this; }
    vec2 operator-=(vec2 _rhs) { m_x -= _rhs.m_x; m_y -= _rhs.m_y; return *this; }
    vec2& operator-=(float &_rhs) { m_x -= _rhs; m_y -= _rhs; return *this; }
    vec2& operator*=(const float &_rhs) { m_x *= _rhs; m_y *= _rhs; return *this; }
    vec2& operator/=(float _rhs) { m_x /= _rhs; m_y /= _rhs; return *this; }

    vec2()
    {
        m_x = m_y = 0.0f;
    }

    vec2(const float _x, const float _y)
    {
        m_x = _x;
        m_y = _y;
    }
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief More operators.
//----------------------------------------------------------------------------------------------------------------------
vec2 operator +(const vec2 &_lhs, const vec2 &_rhs);
vec2 operator +(const vec2 &_lhs, const float &_rhs);
vec2 operator +(const float &_lhs, const vec2 &_rhs);
vec2 operator -(const vec2 &_lhs, const vec2 &_rhs);
vec2 operator -(const vec2 &_lhs);
vec2 operator *(const vec2 &_lhs, const vec2 &_rhs);
vec2 operator *(const vec2 &_lhs, const float &_rhs);
vec2 operator *(const float &_lhs, const vec2 &_rhs);
vec2 operator /(const vec2 &_lhs, const vec2 &_rhs);
vec2 operator /(const vec2 &_lhs, const float &_rhs);
vec2 operator ^(const vec2 &_lhs, const float &_rhs);
bool operator ==(const vec2 &_lhs, const vec2 &_rhs);
bool operator !=(const vec2 &_lhs, const vec2 &_rhs);

//----------------------------------------------------------------------------------------------------------------------
/// \struct A 3d vector, holds an x and y component, and has overloaded operators to make the syntax nicer.
//----------------------------------------------------------------------------------------------------------------------
struct vec3
{
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief X component.
    //----------------------------------------------------------------------------------------------------------------------
    float m_x;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Y component.
    //----------------------------------------------------------------------------------------------------------------------
    float m_y;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Z component.
    //----------------------------------------------------------------------------------------------------------------------
    float m_z;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief In-place operators.
    //----------------------------------------------------------------------------------------------------------------------
    vec3& operator=(const vec3 &_rhs) { m_x = _rhs.m_x; m_y = _rhs.m_y; m_z = _rhs.m_z; return *this; }
    vec3& operator+=(vec3 _rhs) { m_x += _rhs.m_x; m_y += _rhs.m_y; m_z += _rhs.m_z; return *this; }
    vec3& operator-=(vec3 _rhs) { m_x -= _rhs.m_x; m_y -= _rhs.m_y; m_z -= _rhs.m_z; return *this; }
    vec3& operator-=(float &_rhs) { m_x -= _rhs; m_y -= _rhs; m_z -= _rhs; return *this; }
    vec3& operator*=(const float &_rhs) { m_x *= _rhs; m_y *= _rhs; m_z *= _rhs; return *this; }
    vec3& operator/=(float &_rhs) { m_x /= _rhs; m_y /= _rhs; m_z /= _rhs; return *this; }

    vec3()
    {
        m_x = m_y = 0.0f;
    }

    vec3(const float _x, const float _y, const float _z)
    {
        m_x = _x;
        m_y = _y;
        m_z = _z;
    }
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief More operators.
//----------------------------------------------------------------------------------------------------------------------
vec3 operator +(const vec3 &_lhs, const vec3 &_rhs);
vec3 operator +(const vec3 &_lhs, const float &_rhs);
vec3 operator +(const float &_lhs, const vec3 &_rhs);
vec3 operator -(const vec3 &_lhs, const vec3 &_rhs);
vec3 operator -(const vec3 &_lhs);
vec3 operator *(const vec3 &_lhs, const vec3 &_rhs);
vec3 operator *(const vec3 &_lhs, const float &_rhs);
vec3 operator *(const float &_lhs, const vec3 &_rhs);
vec3 operator /(const vec3 &_lhs, const vec3 &_rhs);
vec3 operator /(const vec3 &_lhs, const float &_rhs);
vec3 operator ^(const vec3 &_lhs, const float &_rhs);
bool operator ==(const vec3 &_lhs, const vec3 &_rhs);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Sums the components of a vector.
//----------------------------------------------------------------------------------------------------------------------
float sum(vec2 _vec);
float sum(vec3 _vec);
float asum(vec3 _vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief 2d and 3d cross-products.
//----------------------------------------------------------------------------------------------------------------------
float crossProd(const vec2 A, const vec2 B);
vec2 crossProd(const vec2 A, const float B);
vec2 crossProd(const float A, const vec2 B);
vec3 crossProd(const vec3 A, const vec3 B);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts vectors to angles.
//----------------------------------------------------------------------------------------------------------------------
float ang(const vec2 vec);
vec2 ang(const vec3 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns the length of a vector. Where possible, use magns().
//----------------------------------------------------------------------------------------------------------------------
float mag(const vec2 vec);
float mag(const vec3 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns the squared length of a vector.
//----------------------------------------------------------------------------------------------------------------------
float magns(const vec2 vec);
float magns(const vec3 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns the reciprocal of the length of a vector.
//----------------------------------------------------------------------------------------------------------------------
float invMag(const vec2 vec);
float invMag(const vec3 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Normalises a given vector.
//----------------------------------------------------------------------------------------------------------------------
vec2 unit(const vec2 vec);
vec3 unit(const vec3 vec);

vec2 clamp(const vec2 _in, vec2 _min, vec2 _max);
vec3 clamp(const vec3 _in, vec3 _min, vec3 _max);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns the normal of a 2d vector.
//----------------------------------------------------------------------------------------------------------------------
vec2 normal(const vec2 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns the dot of two vectors.
//----------------------------------------------------------------------------------------------------------------------
float dotProd(const vec2 A, const vec2 B);
float dotProd(const vec3 A, const vec3 B);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns the dot of two vectors, which are first normalised.
//----------------------------------------------------------------------------------------------------------------------
float dotProdUnit(const vec2 A, const vec2 B);
float dotProdUnit(const vec3 A, const vec3 B);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given a point p1 and a gradients m1, and the general point p2, will find the point on p1 + t(m1) closest to p2.
/// \param _p1 line point, _m1 gradient, _p2 general point.
//----------------------------------------------------------------------------------------------------------------------
vec2 closest(const vec2 p1, const vec2 m1, const vec2 p2);
vec3 closest(const vec3 p1, const vec3 m1, const vec3 p2);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts a 2d vector to 3d and vice versa.
//----------------------------------------------------------------------------------------------------------------------
vec3 tovec3(const vec2 vec);
vec2 tovec2(const vec3 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts a 2d vector to an angle.
//----------------------------------------------------------------------------------------------------------------------
float angle(const vec2 vec);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts between degrees and radians.
//----------------------------------------------------------------------------------------------------------------------
float deg(const float ang);
float rad(const float ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts an angle to a 2d vector.
//----------------------------------------------------------------------------------------------------------------------
vec2 vec(const float ang);

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

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Angle operators.
    //----------------------------------------------------------------------------------------------------------------------
    ang3& operator=(const ang3 &_rhs) { m_pitch = _rhs.m_pitch; m_yaw = _rhs.m_yaw; m_roll = _rhs.m_roll; return *this; }
    ang3& operator+=(ang3 _rhs) { m_pitch += _rhs.m_pitch; m_yaw += _rhs.m_yaw; m_roll += _rhs.m_roll; return *this; }
    ang3& operator-=(ang3 &_rhs) { m_pitch -= _rhs.m_pitch; m_yaw -= _rhs.m_yaw; m_roll -= _rhs.m_roll; return *this; }
    ang3& operator-=(float &_rhs) { m_pitch -= _rhs; m_yaw -= _rhs; m_roll -= _rhs; return *this; }
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
vec3 vec(const ang3 ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts an angle between degrees and radians.
//----------------------------------------------------------------------------------------------------------------------
ang3 deg(const ang3 ang);
ang3 rad(const ang3 ang);

#endif
