#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#define V_PI 3.14159265359

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts between degrees and radians.
//----------------------------------------------------------------------------------------------------------------------
float deg(const float ang);
float rad(const float ang);
float fastInvSqrt(float _val);
float clamp(float _v, float _m, float _M);

#endif
