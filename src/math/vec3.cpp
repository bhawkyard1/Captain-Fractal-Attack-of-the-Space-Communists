#include "math/vec3.hpp"

vec3 operator +(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x + rhs.m_x;
    ret.m_y = lhs.m_y + rhs.m_y;
    ret.m_z = lhs.m_z + rhs.m_z;
    return ret;
}

vec3 operator +(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x + rhs;
    ret.m_y = lhs.m_y + rhs;
    ret.m_z = lhs.m_z + rhs;
    return ret;
}

vec3 operator +(const float &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.m_x = lhs + rhs.m_x;
    ret.m_y = lhs + rhs.m_y;
    ret.m_z = lhs + rhs.m_z;
    return ret;
}

vec3 operator -(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x - rhs.m_x;
    ret.m_y = lhs.m_y - rhs.m_y;
    ret.m_z = lhs.m_z - rhs.m_z;
    return ret;
}

vec3 operator -(const vec3 &lhs)
{
    vec3 ret;
    ret.m_x = -lhs.m_x;
    ret.m_y = -lhs.m_y;
    ret.m_z = -lhs.m_z;
    return ret;
}

vec3 operator *(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x * rhs.m_x;
    ret.m_y = lhs.m_y * rhs.m_y;
    ret.m_z = lhs.m_z * rhs.m_z;
    return ret;
}

vec3 operator *(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x * rhs;
    ret.m_y = lhs.m_y * rhs;
    ret.m_z = lhs.m_z * rhs;
    return ret;
}

vec3 operator *(const float &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.m_x = lhs * rhs.m_x;
    ret.m_y = lhs * rhs.m_y;
    ret.m_z = lhs * rhs.m_z;
    return ret;
}

vec3 operator /(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x / rhs.m_x;
    ret.m_y = lhs.m_y / rhs.m_y;
    ret.m_z = lhs.m_z / rhs.m_z;
    return ret;
}

vec3 operator /(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.m_x = lhs.m_x / rhs;
    ret.m_y = lhs.m_y / rhs;
    ret.m_z = lhs.m_z / rhs;
    return ret;
}

vec3 operator ^(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.m_x = pow(lhs.m_x,rhs);
    ret.m_y = pow(lhs.m_y,rhs);
    ret.m_z = pow(lhs.m_z,rhs);
    return ret;
}

bool operator ==(const vec3 &lhs, const vec3 &rhs)
{
    return (lhs.m_x == rhs.m_x) and (lhs.m_y == rhs.m_y) and (lhs.m_z == rhs.m_z);
}

bool operator !=(const vec3 &lhs, const vec3 &rhs)
{
    return (lhs.m_x != rhs.m_x) or (lhs.m_y != rhs.m_y) or (lhs.m_z != rhs.m_z);
}

float sum(vec3 _vec)
{
    return _vec.m_x + _vec.m_y + _vec.m_z;
}

float asum(vec3 _vec)
{
    return fabs(_vec.m_x) + fabs(_vec.m_y) + fabs(_vec.m_z);
}

float mag(vec3 vec)
{
    return sqrt(vec.m_x * vec.m_x + vec.m_y * vec.m_y + vec.m_z * vec.m_z);
}

float magns(vec3 vec)
{
    return vec.m_x*vec.m_x+vec.m_y*vec.m_y+vec.m_z*vec.m_z;
}

float invMag(vec3 vec)
{
    return fastInvSqrt(vec.m_x*vec.m_x+vec.m_y*vec.m_y+vec.m_z*vec.m_z);
}

vec3 unit(vec3 vec)
{
    float m = invMag(vec);

    if(m != 0) return vec * m;
    return {0,0,0};
}

vec3 clamp(const vec3 _in, vec3 _min, vec3 _max)
{
    vec3 ret;
    ret.m_x = clamp(_in.m_x, _min.m_x, _max.m_x);
    ret.m_y = clamp(_in.m_y, _min.m_y, _max.m_y);
    ret.m_z = clamp(_in.m_z, _min.m_z, _max.m_z);

    return ret;
}

float dot(vec3 A, vec3 B)
{
    return sum(A * B);
}

float udot(vec3 A, vec3 B)
{
    return sum(unit(A) * unit(B));
}

vec3 crossProd(vec3 A, vec3 B)
{
    return {A.m_y * B.m_z - A.m_z * B.m_y, A.m_z * B.m_x - A.m_x * B.m_z, A.m_x * B.m_y - A.m_y * B.m_x};
}

vec3 closest(vec3 p1, vec3 m1, vec3 p2)
{
    //Ensure m1 is a unit vector.
    float m = mag(m1);
    if(m == 0.0f) return p1;
    else m1 /= m;

    return p1 + m1 * dot( p2 - p1, m1 );
}

vec3 tovec3(vec2 vec)
{
    return {vec.m_x, vec.m_y, 0.0f};
}

vec2 tovec2(vec3 vec)
{
    return {vec.m_x, vec.m_y};
}
