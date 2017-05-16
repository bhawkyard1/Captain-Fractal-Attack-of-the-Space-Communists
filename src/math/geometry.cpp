#include "math/geometry.hpp"

float deg(float ang)
{
    return ang * ( 180 / V_PI );
}

float rad(float ang)
{
    return ang * ( V_PI / 180 );
}

float fastInvSqrt(float _val)
{
    //Fast inverse sqrt, discovered in quake 3 source, attributed to Greg Walsh.
    float half = 0.5f * _val;
    int i = *(int*)&_val; // get bits for floating value
    i = 0x5f3759df - (i >> 1); // gives initial guess y0
    _val = *(float*)&i; // convert bits back to float
    _val = _val * (1.5f - half * _val * _val); // Newton step, repeating increases accuracy
    return _val;
}

float clamp(float _v, float _m, float _M)
{
    if(_v < _m) return _m;
    else if(_v > _M) return _M;
    return _v;
}
