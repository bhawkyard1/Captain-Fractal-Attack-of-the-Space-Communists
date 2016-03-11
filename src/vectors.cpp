#include "vectors.hpp"
#include "util.hpp"

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

float sum(vec2 vec)
{
    return vec.m_x + vec.m_y;
}

float sum(vec3 vec)
{
    return vec.m_x + vec.m_y + vec.m_z;
}

//CROSS PRODUCT

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

vec3 crossProd(vec3 A, vec3 B)
{
    return {A.m_y * B.m_z - A.m_z * B.m_y, A.m_z * B.m_x - A.m_x * B.m_z, A.m_x * B.m_y - A.m_y * B.m_x};
}

float ang(vec2 vec)
{
    return atan2(vec.m_x,vec.m_y);
}

vec2 ang(vec3 vec)
{
    return {static_cast<float> (atan2(vec.m_y,vec.m_x)),static_cast<float> (atan2(vec.m_z,vec.m_x))};
}

float mag(vec2 vec)
{
    return sqrt(vec.m_x*vec.m_x+vec.m_y*vec.m_y);
}

float mag(vec3 vec)
{
    return sqrt(vec.m_x*vec.m_x+vec.m_y*vec.m_y+vec.m_z*vec.m_z);
}

float magns(vec2 vec)
{
    return vec.m_x*vec.m_x+vec.m_y*vec.m_y;
}

float magns(vec3 vec)
{
    return vec.m_x*vec.m_x+vec.m_y*vec.m_y+vec.m_z*vec.m_z;
}

float invMag(vec2 _vec)
{
    return fastInvSqrt(_vec.m_x*_vec.m_x+_vec.m_y*_vec.m_y);
}

float invMag(vec3 vec)
{
    return fastInvSqrt(vec.m_x*vec.m_x+vec.m_y*vec.m_y+vec.m_z*vec.m_z);
}

vec2 unit(vec2 vec)
{
    float m = invMag(vec);

    if(m != 0) return vec * m;
    return {0,0};
}

vec3 unit(vec3 vec)
{
    float m = invMag(vec);

    if(m != 0) return vec * m;
    return {0,0,0};
}

vec2 normal(vec2 vec)
{
    return {-vec.m_y,vec.m_x};
}

//DOT PRODUCT

float dotProd1(vec2 A, vec2 B)
{
    return sum(A * B);
}

float dotProd2(vec2 A, vec2 B)
{
    return sum(unit(A) * unit(B));
}

float dotProd1(vec3 A, vec3 B)
{
    return sum(A * B);
}

float dotProd2(vec3 A, vec3 B)
{
    return sum(unit(A) * unit(B));
}

//Given a point p1 and a gradients m1, and the general point p2, will find the point on p1 + t(m1) closest to p2.
vec2 closest(vec2 p1, vec2 m1, vec2 p2)
{
    float m = mag(m1);
    if(m == 0.0f) return p1;
    else m1 /= m;
    return p1 + m1 * dotProd1( p2 - p1, m1 );
}

vec3 tovec3(vec2 vec)
{
    return {vec.m_x, vec.m_y, 0};
}

vec2 tovec2(vec3 vec)
{
    return {vec.m_x, vec.m_y};
}


float angle(vec2 vec)
{
    return atan2( vec.m_y, vec.m_x );
}

float deg(float ang)
{
    return ang * ( 180 / V_PI );
}

float rad(float ang)
{
    return ang * ( V_PI / 180 );
}

vec2 vec(float ang)
{
    return { static_cast<float>(-cos(rad(-ang))), static_cast<float>(sin(rad(-ang))) };
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
