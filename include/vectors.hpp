#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <math.h>

#define PI 3.14159265359

struct vec2
{
	float x, y;
	
	vec2& operator=(const vec2 &rhs) { x = rhs.x; y = rhs.y; return *this; }
	vec2 operator+=(vec2 rhs) { x += rhs.x; y += rhs.y; return *this; }
	vec2 operator-=(vec2 rhs) { x -= rhs.x; y -= rhs.y; return *this; }
	vec2& operator-=(float &rhs) { x -= rhs; y -= rhs; return *this; }
	vec2& operator*=(float &rhs) { x *= rhs; y *= rhs; return *this; }
	vec2& operator/=(float &rhs) { x /= rhs; y /= rhs; return *this; }
};

vec2 operator +(const vec2 &lhs, const vec2 &rhs);
vec2 operator +(const vec2 &lhs, const float &rhs);
vec2 operator +(const float &lhs, const vec2 &rhs);
vec2 operator -(const vec2 &lhs, const vec2 &rhs);
vec2 operator -(const vec2 &lhs);
vec2 operator *(const vec2 &lhs, const vec2 &rhs);
vec2 operator *(const vec2 &lhs, const float &rhs);
vec2 operator *(const float &lhs, const vec2 &rhs);
vec2 operator /(const vec2 &lhs, const vec2 &rhs);
vec2 operator /(const vec2 &lhs, const float &rhs);
vec2 operator ^(const vec2 &lhs, const float &rhs);
bool operator ==(const vec2 &lhs, const vec2 &rhs);
bool operator !=(const vec2 &lhs, const vec2 &rhs);

struct vec3
{
	float x, y, z;
	
	vec3& operator=(const vec3 &rhs) { x = rhs.x; y = rhs.y; z = rhs.z; return *this; }
	vec3& operator+=(vec3 rhs) { x += rhs.x; y += rhs.y; z += rhs.z; return *this; }
	vec3& operator-=(vec3 &rhs) { x -= rhs.x; y -= rhs.y; z -= rhs.z; return *this; }
	vec3& operator-=(float &rhs) { x -= rhs; y -= rhs; z -= rhs; return *this; }
	vec3& operator*=(float &rhs) { x *= rhs; y *= rhs; z *= rhs; return *this; }
	vec3& operator/=(float &rhs) { x /= rhs; y /= rhs; z /= rhs; return *this; }
};

	
vec3 operator +(const vec3 &lhs, const vec3 &rhs);
vec3 operator +(const vec3 &lhs, const float &rhs);
vec3 operator +(const float &lhs, const vec3 &rhs);
vec3 operator -(const vec3 &lhs, const vec3 &rhs);
vec3 operator -(const vec3 &lhs);
vec3 operator *(const vec3 &lhs, const vec3 &rhs);
vec3 operator *(const vec3 &lhs, const float &rhs);
vec3 operator *(const float &lhs, const vec3 &rhs);
vec3 operator /(const vec3 &lhs, const vec3 &rhs);
vec3 operator /(const vec3 &lhs, const float &rhs);
vec3 operator ^(const vec3 &lhs, const float &rhs);
bool operator ==(const vec3 &lhs, const vec3 &rhs);
float sum(vec2 vec);
float sum(vec3 vec);

//CROSS PRODUCT

float crossProd(vec2 A, vec2 B);
vec2 crossProd(vec2 A, float B);
vec2 crossProd(float A, vec2 B);
vec3 crossProd(vec3 A, vec3 B);
float ang(vec2 vec);
vec2 ang(vec3 vec);
float mag(vec2 vec);
float mag(vec3 vec);
float magns(vec2 vec);
float magns(vec3 vec);
float invMag(vec2 vec);
float invMag(vec3 vec);
vec2 unit(vec2 vec);
vec3 unit(vec3 vec);
vec2 normal(vec2 vec);

//DOT PRODUCT

float dotProd1(vec2 A, vec2 B);
float dotProd2(vec2 A, vec2 B);
float dotProd1(vec3 A, vec3 B);
float dotProd2(vec3 A, vec3 B);

//Given a point p1 and a gradients m1, and the general point p2, will find the point on p1 + t(m1) closest to p2.
vec2 closest(vec2 p1, vec2 m1, vec2 p2);
vec3 tovec3(vec2 vec);
vec2 tovec2(vec3 vec);
float angle(vec2 vec);
float deg(float ang);
float rad(float ang);
vec2 vec(float ang);

struct ang3
{
	float pitch, yaw, roll;
	
	ang3& operator=(const ang3 &rhs) { pitch = rhs.pitch; yaw = rhs.yaw; roll = rhs.roll; return *this; }
	ang3& operator+=(ang3 rhs) { pitch += rhs.pitch; yaw += rhs.yaw; roll += rhs.roll; return *this; }
	ang3& operator-=(ang3 &rhs) { pitch -= rhs.pitch; yaw -= rhs.yaw; roll -= rhs.roll; return *this; }
	ang3& operator-=(float &rhs) { pitch -= rhs; yaw -= rhs; roll -= rhs; return *this; }
};
	
ang3 operator +(const ang3 &lhs, const ang3 &rhs);
ang3 operator +(const ang3 &lhs, const float &rhs);
ang3 operator +(const float &lhs, const ang3 &rhs);
ang3 operator -(const ang3 &lhs, const ang3 &rhs);
ang3 operator -(const ang3 &lhs);
ang3 operator *(const ang3 &lhs, const ang3 &rhs);
ang3 operator *(const ang3 &lhs, const float &rhs);
ang3 operator *(const float &lhs, const ang3 &rhs);
ang3 operator /(const ang3 &lhs, const ang3 &rhs);
ang3 operator /(const ang3 &lhs, const float &rhs);
ang3 operator ^(const ang3 &lhs, const float &rhs);
bool operator ==(const ang3 &lhs, const ang3 &rhs);
vec3 vec(ang3 ang);
ang3 deg(ang3 ang);
ang3 rad(ang3 ang);

#endif
