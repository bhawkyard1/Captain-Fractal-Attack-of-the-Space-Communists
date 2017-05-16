#include "math/vec2.hpp"

vec2 operator +(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x + rhs.m_x;
    ret.m_y = lhs.m_y + rhs.m_y;
    return ret;
}

vec2 operator +(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x + rhs;
    ret.m_y = lhs.m_y + rhs;
    return ret;
}

vec2 operator +(const float &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.m_x = lhs + rhs.m_x;
    ret.m_y = lhs + rhs.m_y;
    return ret;
}

vec2 operator -(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x - rhs.m_x;
    ret.m_y = lhs.m_y - rhs.m_y;
    return ret;
}

vec2 operator -(const vec2 &lhs)
{
    vec2 ret;
    ret.m_x = -lhs.m_x;
    ret.m_y = -lhs.m_y;
    return ret;
}

vec2 operator *(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x * rhs.m_x;
    ret.m_y = lhs.m_y * rhs.m_y;
    return ret;
}

vec2 operator *(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x * rhs;
    ret.m_y = lhs.m_y * rhs;
    return ret;
}

vec2 operator *(const float &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.m_x = lhs * rhs.m_x;
    ret.m_y = lhs * rhs.m_y;
    return ret;
}

vec2 operator /(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x / rhs.m_x;
    ret.m_y = lhs.m_y / rhs.m_y;
    return ret;
}

vec2 operator /(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.m_x = lhs.m_x / rhs;
    ret.m_y = lhs.m_y / rhs;
    return ret;
}

vec2 operator ^(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.m_x = pow(lhs.m_x,rhs);
    ret.m_y = pow(lhs.m_y,rhs);
    return ret;
}

bool operator ==(const vec2 &lhs, const vec2 &rhs)
{
    return (lhs.m_x == rhs.m_x) && (lhs.m_y == rhs.m_y);
}

bool operator !=(const vec2 &lhs, const vec2 &rhs)
{
    return !(lhs.m_x == rhs.m_x) || !(lhs.m_y == rhs.m_y);
}

float sum(vec2 _vec)
{
    return _vec.m_x + _vec.m_y;
}

float crossProd(vec2 A, vec2 B)
{
    return A.m_x * B.m_y - A.m_y * B.m_x;
}

vec2 crossProd(vec2 A, float B)
{
    return {B*A.m_y, -B*A.m_x};
}

vec2 crossProd(float A, vec2 B)
{
    return {-A*B.m_y, A*B.m_x};
}

float ang(vec2 vec)
{
    return atan2(vec.m_x,vec.m_y);
}

float mag(vec2 vec)
{
    return sqrt(vec.m_x*vec.m_x+vec.m_y*vec.m_y);
}

float magns(vec2 vec)
{
    return vec.m_x*vec.m_x+vec.m_y*vec.m_y;
}

float invMag(vec2 _vec)
{
    return fastInvSqrt(_vec.m_x*_vec.m_x+_vec.m_y*_vec.m_y);
}

vec2 unit(vec2 vec)
{
    float m = invMag(vec);

    if(m != 0) return vec * m;
    return {0,0};
}

vec2 clamp(vec2 _in, vec2 _min, vec2 _max)
{
    vec2 ret;
    ret.m_x = clamp(_in.m_x, _min.m_x, _max.m_x);
    ret.m_y = clamp(_in.m_y, _min.m_y, _max.m_y);

    return ret;
}

vec2 normal(vec2 vec)
{
    return {-vec.m_y,vec.m_x};
}

float dot(vec2 A, vec2 B)
{
    return sum(A * B);
}

float udot(vec2 A, vec2 B)
{
    return sum(unit(A) * unit(B));
}

float angle(vec2 vec)
{
    return atan2( vec.m_y, vec.m_x );
}

vec2 vec(float ang)
{
    return { static_cast<float>(-cos(rad(-ang))), static_cast<float>(sin(rad(-ang))) };
}

vec2 closest(vec2 p1, vec2 m1, vec2 p2)
{
    //Ensure m1 is a unit vector.
    float m = mag(m1);
    if(m == 0.0f) return p1;
    else m1 /= m;

    return p1 + m1 * dot( p2 - p1, m1 );
}


