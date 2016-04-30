#include "util.hpp"

float computeAngle(vec2 _v)
{
    return -deg(atan2(_v.m_x,_v.m_y));
}

vec2 computeVector(float _angle)
{
    return { static_cast<float>(-cos(rad(-_angle))), static_cast<float>(sin(rad(-_angle))) };
}

float shortestAngle(float _ang1, float _ang2)
{
    return fmod(_ang1 - _ang2 + 180, 360) - 180;
}

double dotProd(double x0, double y0, double x1, double y1)
{
    return acos((x0*x1+y0*y1)/(sqrt(x0*x0+y0*y0)*sqrt(x1*x1+y1*y1)));
}

float sqr(float _arg)
{
    return _arg*_arg;
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

bool pointInRect(vec2 _point, vec2 _pos, vec2 _dim)
{
    if(_point.m_x > _pos.m_x and _point.m_x < _pos.m_x + _dim.m_x and _point.m_y > _pos.m_y and _point.m_y < _pos.m_y + _dim.m_y) return true;
    return false;
}

vec2 randVec2(float _m, float _M)
{
    float a = randNum(0.0f, static_cast<float>(UPI));
    vec2 v = {static_cast<float>(cos(a)), static_cast<float>(sin(a))};
    return v * randNum(_m, _M);
}

vec2 randVec2(float _f)
{
    float a = randNum(0.0f, static_cast<float>(UPI));
    vec2 v = {static_cast<float>(cos(a)), static_cast<float>(sin(a))};
    return v * randNum(0.0f, _f);
}

vec2 randVec2(vec2 _min, vec2 _max)
{
    return {randNum(_min.m_x, _max.m_x), randNum(_min.m_y, _max.m_y)};
}

vec3 randVec3(float m, float M)
{
    vec3 v = {randNum(0.0f, 1.0f), randNum(0.0f, 1.0f), randNum(0.0f, 1.0f)};
    float vsum = sum(v);
    v /= vsum;
    return v * randNum(m, M);
}

vec3 randVec3(float f)
{
    vec3 v = {randNum(0.0f, 1.0f), randNum(0.0f, 1.0f), randNum(0.0f, 1.0f)};
    float vsum = sum(v);
    v /= vsum;
    return v * randNum(0.0f, f);
}

vec3 randVec3(vec3 min, vec3 max)
{
    return {randNum(min.m_x, max.m_x), randNum(min.m_y, max.m_y), randNum(min.m_z, max.m_z)};
}

std::vector<std::string> split(std::string _str, char _delim)
{
    std::vector<std::string> ret;

    std::stringstream ss(_str);
    std::string sub;

    while( getline( ss, sub, _delim ) )
    {
        ret.push_back(sub);
    }

    return ret;
}

vec2 front(float _ang)
{
    _ang -= rad(90.0f);
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

vec2 back(float _ang)
{
    _ang += rad(90.0f);
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

vec2 left(float _ang)
{
    _ang -= rad(180.0f);
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

vec2 right(float _ang)
{
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

std::array<float, 4> col255to1(const std::array<float, 4> _col)
{
    return {_col[0] / 255.0f, _col[1] / 255.0f, _col[2] / 255.0f, _col[3] / 255.0f};
}

std::array<float, 4> col255to1(const std::array<int, 4> _col)
{
    return {_col[0] / 255.0f, _col[1] / 255.0f, _col[2] / 255.0f, _col[3] / 255.0f};
}
