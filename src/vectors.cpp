#include "vectors.hpp"

vec2 operator +(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.x = lhs.x + rhs.x;
    ret.y = lhs.y + rhs.y;
    return ret;
}

vec2 operator +(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.x = lhs.x + rhs;
    ret.y = lhs.y + rhs;
    return ret;
}

vec2 operator +(const float &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.x = lhs + rhs.x;
    ret.y = lhs + rhs.y;
    return ret;
}

vec2 operator -(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.x = lhs.x - rhs.x;
    ret.y = lhs.y - rhs.y;
    return ret;
}

vec2 operator -(const vec2 &lhs)
{
    vec2 ret;
    ret.x = -lhs.x;
    ret.y = -lhs.y;
    return ret;
}

vec2 operator *(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.x = lhs.x * rhs.x;
    ret.y = lhs.y * rhs.y;
    return ret;
}

vec2 operator *(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.x = lhs.x * rhs;
    ret.y = lhs.y * rhs;
    return ret;
}

vec2 operator *(const float &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.x = lhs * rhs.x;
    ret.y = lhs * rhs.y;
    return ret;
}

vec2 operator /(const vec2 &lhs, const vec2 &rhs)
{
    vec2 ret;
    ret.x = lhs.x / rhs.x;
    ret.y = lhs.y / rhs.y;
    return ret;
}

vec2 operator /(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.x = lhs.x / rhs;
    ret.y = lhs.y / rhs;
    return ret;
}

vec2 operator ^(const vec2 &lhs, const float &rhs)
{
    vec2 ret;
    ret.x = pow(lhs.x,rhs);
    ret.y = pow(lhs.y,rhs);
    return ret;
}

bool operator ==(const vec2 &lhs, const vec2 &rhs)
{
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator !=(const vec2 &lhs, const vec2 &rhs)
{
    return !(lhs.x == rhs.x) || !(lhs.y == rhs.y);
}


vec3 operator +(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.x = lhs.x + rhs.x;
    ret.y = lhs.y + rhs.y;
    ret.z = lhs.z + rhs.z;
    return ret;
}

vec3 operator +(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.x = lhs.x + rhs;
    ret.y = lhs.y + rhs;
    ret.z = lhs.z + rhs;
    return ret;
}

vec3 operator +(const float &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.x = lhs + rhs.x;
    ret.y = lhs + rhs.y;
    ret.z = lhs + rhs.z;
    return ret;
}

vec3 operator -(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.x = lhs.x - rhs.x;
    ret.y = lhs.y - rhs.y;
    ret.z = lhs.z - rhs.z;
    return ret;
}

vec3 operator -(const vec3 &lhs)
{
    vec3 ret;
    ret.x = -lhs.x;
    ret.y = -lhs.y;
    ret.z = -lhs.z;
    return ret;
}

vec3 operator *(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.x = lhs.x * rhs.x;
    ret.y = lhs.y * rhs.y;
  ret.z = lhs.z * rhs.z;
    return ret;
}

vec3 operator *(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.x = lhs.x * rhs;
    ret.y = lhs.y * rhs;
    ret.z = lhs.z * rhs;
    return ret;
}

vec3 operator *(const float &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.x = lhs * rhs.x;
    ret.y = lhs * rhs.y;
    ret.z = lhs * rhs.z;
    return ret;
}

vec3 operator /(const vec3 &lhs, const vec3 &rhs)
{
    vec3 ret;
    ret.x = lhs.x / rhs.x;
    ret.y = lhs.y / rhs.y;
    ret.z = lhs.z / rhs.z;
    return ret;
}

vec3 operator /(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.x = lhs.x / rhs;
    ret.y = lhs.y / rhs;
    ret.z = lhs.z / rhs;
    return ret;
}

vec3 operator ^(const vec3 &lhs, const float &rhs)
{
    vec3 ret;
    ret.x = pow(lhs.x,rhs);
    ret.y = pow(lhs.y,rhs);
    ret.z = pow(lhs.z,rhs);
    return ret;
}

bool operator ==(const vec3 &lhs, const vec3 &rhs)
{
    return (lhs.x == rhs.x) and (lhs.y == rhs.y) and (lhs.z == rhs.z);
}

float sum(vec2 vec)
{
  return vec.x + vec.y;
}

float sum(vec3 vec)
{
  return vec.x + vec.y + vec.z;
}

//CROSS PRODUCT

float crossProd(vec2 A, vec2 B)
{
  return A.x * B.y - A.y * B.x;
}

vec2 crossProd(vec2 A, float B)
{
  return {B*A.y, -B*A.x};
}

vec2 crossProd(float A, vec2 B)
{
  return {-A*B.y, A*B.x};
}

vec3 crossProd(vec3 A, vec3 B)
{
  return {A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x};
}

float ang(vec2 vec)
{
  return atan2(vec.x,vec.y);
}

vec2 ang(vec3 vec)
{
  return {static_cast<float> (atan2(vec.y,vec.x)),static_cast<float> (atan2(vec.z,vec.x))};
}

float mag(vec2 vec)
{
  return sqrt(vec.x*vec.x+vec.y*vec.y);
}

float mag(vec3 vec)
{
  return sqrt(vec.x*vec.x+vec.y*vec.y+vec.z*vec.z);
}

float magns(vec2 vec)
{
  return vec.x*vec.x+vec.y*vec.y;
}

float magns(vec3 vec)
{
  return vec.x*vec.x+vec.y*vec.y+vec.z*vec.z;
}

vec2 unit(vec2 vec)
{
  float m = mag(vec);

  if(m != 0) return vec/m;
  return {0,0};
}

vec3 unit(vec3 vec)
{
  float m = mag(vec);

  if(m != 0) return vec/m;
  return {0,0,0};
}

vec2 normal(vec2 vec)
{
  return {-vec.y,vec.x};
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
  return {vec.x, vec.y, 0};
}

vec2 tovec2(vec3 vec)
{
  return {vec.x, vec.y};
}


float angle(vec2 vec)
{
  return atan2( vec.y, vec.x );
}

float deg(float ang)
{
  return ang * ( 180 / PI );
}

float rad(float ang)
{
  return ang * ( PI / 180 );
}

vec2 vec(float ang)
{
  return { static_cast<float>(-cos(rad(-ang))), static_cast<float>(sin(rad(-ang))) };
}

ang3 operator +(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch + rhs.pitch;
    ret.yaw = lhs.yaw + rhs.yaw;
    ret.roll = lhs.roll + rhs.roll;
    return ret;
}

ang3 operator +(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch + rhs;
    ret.yaw = lhs.yaw + rhs;
    ret.roll = lhs.roll + rhs;
    return ret;
}

ang3 operator +(const float &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.pitch = lhs + rhs.pitch;
    ret.yaw = lhs + rhs.yaw;
    ret.roll = lhs + rhs.roll;
    return ret;
}

ang3 operator -(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch - rhs.pitch;
    ret.yaw = lhs.yaw - rhs.yaw;
    ret.roll = lhs.roll - rhs.roll;
    return ret;
}

ang3 operator -(const ang3 &lhs)
{
    ang3 ret;
    ret.pitch = -lhs.pitch;
    ret.yaw = -lhs.yaw;
    ret.roll = -lhs.roll;
    return ret;
}

ang3 operator *(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch * rhs.pitch;
    ret.yaw = lhs.yaw * rhs.yaw;
    ret.roll = lhs.roll * rhs.roll;
    return ret;
}

ang3 operator *(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch * rhs;
    ret.yaw = lhs.yaw * rhs;
    ret.roll = lhs.roll * rhs;
    return ret;
}

ang3 operator *(const float &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.pitch = lhs * rhs.pitch;
    ret.yaw = lhs * rhs.yaw;
    ret.roll = lhs * rhs.roll;
    return ret;
}

ang3 operator /(const ang3 &lhs, const ang3 &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch / rhs.pitch;
    ret.yaw = lhs.yaw / rhs.yaw;
    ret.roll = lhs.roll / rhs.roll;
    return ret;
}

ang3 operator /(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.pitch = lhs.pitch / rhs;
    ret.yaw = lhs.yaw / rhs;
    ret.roll = lhs.roll / rhs;
    return ret;
}

ang3 operator ^(const ang3 &lhs, const float &rhs)
{
    ang3 ret;
    ret.pitch = pow(lhs.pitch, rhs);
    ret.yaw = pow(lhs.yaw, rhs);
    ret.roll = pow(lhs.roll, rhs);
    return ret;
}

bool operator ==(const ang3 &lhs, const ang3 &rhs)
{
    return (lhs.pitch == rhs.pitch) and (lhs.yaw == rhs.yaw) and (lhs.roll == rhs.roll);
}

vec3 vec(ang3 ang)
{
  return { static_cast<float> (atan2(ang.yaw, ang.pitch)), static_cast<float> (atan2(ang.roll, ang.pitch)), 0.0f };
}

ang3 deg(ang3 ang)
{
  return ang * ( 180 / PI );
}

ang3 rad(ang3 ang)
{
  return ang * ( PI / 180 );
}
