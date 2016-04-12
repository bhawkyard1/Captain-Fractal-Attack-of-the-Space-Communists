#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <math.h>

#define V_PI 3.14159265359
struct vec2;
struct vec3;

struct vec2
{
  float m_x, m_y;

  vec2& operator=(const vec2 &_rhs) { m_x = _rhs.m_x; m_y = _rhs.m_y; return *this; }
  vec2 operator+=(vec2 _rhs) { m_x += _rhs.m_x; m_y += _rhs.m_y; return *this; }
  vec2 operator-=(vec2 _rhs) { m_x -= _rhs.m_x; m_y -= _rhs.m_y; return *this; }
  vec2& operator-=(float &_rhs) { m_x -= _rhs; m_y -= _rhs; return *this; }
  vec2& operator*=(const float &_rhs) { m_x *= _rhs; m_y *= _rhs; return *this; }
  vec2& operator/=(float _rhs) { m_x /= _rhs; m_y /= _rhs; return *this; }
};

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

struct vec3
{
  float m_x, m_y, m_z;

  vec3& operator=(const vec3 &_rhs) { m_x = _rhs.m_x; m_y = _rhs.m_y; m_z = _rhs.m_z; return *this; }
  vec3& operator+=(vec3 _rhs) { m_x += _rhs.m_x; m_y += _rhs.m_y; m_z += _rhs.m_z; return *this; }
  vec3& operator-=(vec3 _rhs) { m_x -= _rhs.m_x; m_y -= _rhs.m_y; m_z -= _rhs.m_z; return *this; }
  vec3& operator-=(float &_rhs) { m_x -= _rhs; m_y -= _rhs; m_z -= _rhs; return *this; }
  vec3& operator*=(const float &_rhs) { m_x *= _rhs; m_y *= _rhs; m_z *= _rhs; return *this; }
  vec3& operator/=(float &_rhs) { m_x /= _rhs; m_y /= _rhs; m_z /= _rhs; return *this; }
};


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
float sum(vec2 vec);
float sum(vec3 vec);

//CROSS PRODUCT

float crossProd(const vec2 A, const vec2 B);
vec2 crossProd(const vec2 A, const float B);
vec2 crossProd(const float A, const vec2 B);
vec3 crossProd(const vec3 A, const vec3 B);
float ang(const vec2 vec);
vec2 ang(const vec3 vec);
float mag(const vec2 vec);
float mag(const vec3 vec);
float magns(const vec2 vec);
float magns(const vec3 vec);
float invMag(const vec2 vec);
float invMag(const vec3 vec);
vec2 unit(const vec2 vec);
vec3 unit(const vec3 vec);
vec2 normal(const vec2 vec);

//DOT PRODUCT

float dotProd1(const vec2 A, const vec2 B);
float dotProd2(const vec2 A, const vec2 B);
float dotProd1(const vec3 A, const vec3 B);
float dotProd2(const vec3 A, const vec3 B);

//Given a point p1 and a gradients m1, and the general point p2, will find the point on p1 + t(m1) closest to p2.
vec2 closest(const vec2 p1, const vec2 m1, const vec2 p2);
vec3 closest(const vec3 p1, const vec3 m1, const vec3 p2);
vec3 tovec3(const vec2 vec);
vec2 tovec2(const vec3 vec);
float angle(const vec2 vec);
float deg(const float ang);
float rad(const float ang);
vec2 vec(const float ang);

struct ang3
{
  float m_pitch, m_yaw, m_roll;

  ang3& operator=(const ang3 &_rhs) { m_pitch = _rhs.m_pitch; m_yaw = _rhs.m_yaw; m_roll = _rhs.m_roll; return *this; }
  ang3& operator+=(ang3 _rhs) { m_pitch += _rhs.m_pitch; m_yaw += _rhs.m_yaw; m_roll += _rhs.m_roll; return *this; }
  ang3& operator-=(ang3 &_rhs) { m_pitch -= _rhs.m_pitch; m_yaw -= _rhs.m_yaw; m_roll -= _rhs.m_roll; return *this; }
  ang3& operator-=(float &_rhs) { m_pitch -= _rhs; m_yaw -= _rhs; m_roll -= _rhs; return *this; }
};

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
vec3 vec(const ang3 ang);
ang3 deg(const ang3 ang);
ang3 rad(const ang3 ang);

#endif
