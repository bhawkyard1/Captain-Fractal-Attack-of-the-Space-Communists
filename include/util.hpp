#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>

#include <vector>
#include <math.h>
#include <limits>
#include <algorithm>
#include <sstream>
#include <time.h>
#include <array>

#include <ciso646>

#include <SDL.h>
#include "SDL2/SDL_ttf.h"

#include "math/vec3.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \brief Max and infinite values for floats. Useful when you want to loop through a bunch of values to find the highest/
/// lowest.
//----------------------------------------------------------------------------------------------------------------------
#define F_MAX std::numeric_limits<float>::max()
#define F_INF std::numeric_limits<float>::infinity()

//----------------------------------------------------------------------------------------------------------------------
/// \brief Max and infinite values for ints. Useful when you want to loop through a bunch of values to find the highest/
/// lowest.
//----------------------------------------------------------------------------------------------------------------------
#define I_MAX std::numeric_limits<int>::max()
#define I_INF std::numeric_limits<int>::max()

#define ST_MAX std::numeric_limits<size_t>::max()
#define ST_INF std::numeric_limits<size_t>::max()

//----------------------------------------------------------------------------------------------------------------------
/// \brief A value used for all calculations involving pi. NGL has its own, but this game can run without NGL.
//----------------------------------------------------------------------------------------------------------------------
#define UPI 3.14159265359

#define PRINT_DEBUG_MESSAGES 0

//----------------------------------------------------------------------------------------------------------------------
/// \file util.hpp
/// \brief A small file containing lots of useful values and functions, used across the project.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

int gcd(int _a, int _b);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Clamps a value of any type to two values of the same type.
/// \param _v input value
/// \param _m minimum threshold
/// \param _M maximum threshold
//----------------------------------------------------------------------------------------------------------------------
template<typename tt>
tt clamp(tt _v, tt _m, tt _M)
{
    if(_v < _m) return _m;
    else if(_v > _M) return _M;
    return _v;
}

template<typename tt>
tt clampOutside(tt _v, tt _m, tt _M)
{
    if(_v > _m and _v < _M)
    {
        if(abs(_v - _m) < abs(_v - _M)) return _m;
        else return _M;
    }
    return _v;
}

template<typename tt>
tt getClosest(tt _v, tt _m, tt _M)
{
    if(abs(_v - _m) < abs(_v - _M)) return _m;
    return _M;
}

template<typename t>
t * getClosest(vec3 _pos, const std::vector<t> &_obj)
{
    vec3 ret = vec3();
    float dist = F_INF;
    for(auto &i : _obj)
    {
        float temp = magns(_pos - i.getPos());
        if(temp < dist)
        {
            dist = temp;
            ret = i.getPos();
        }
    }
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns a random number between two values.
/// \param _low lover limit
/// \param _high upper limit
//----------------------------------------------------------------------------------------------------------------------
template<class t>
t randNum(t _low, t _high)
{
    if(_low == _high) return _low;
    return static_cast <t> (rand()) / static_cast <t> (RAND_MAX/(_high-_low))+_low;
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief Gets a random entry from an vector.
/// \param _v container ref
//----------------------------------------------------------------------------------------------------------------------
template<typename tt>
tt getRandomEntry(std::vector<tt> * _ref)
{
    int ri = randNum(0, static_cast<int>(_ref->size()));
    return (*_ref)[ri];
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief A rolling clamp, a value which exceeds the max will roll back around to the minimum.
/// \param _v input value
/// \param _m minimum threshold
/// \param _M maximum threshold
//----------------------------------------------------------------------------------------------------------------------
float clampRoll(float _v, float _m, float _M);
int clampRoll(int _v, int _m, int _M);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns whether a given value is between two others.
/// \param _v input value
/// \param _m minimum threshold
/// \param _M maximum threshold
//----------------------------------------------------------------------------------------------------------------------
template<typename tt>
tt inRange(tt _v, tt _m, tt _M)
{
    if(_M < _m)
    {
        tt temp = _M;
        _M = _m;
        _m = temp;
    }
    return _v <= _M and _v >= _m;
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given two angles in degrees, this function will return the shortest angle between them, also in degrees.
/// \param _ang1 first angle
/// \param _ang2 second angle
//----------------------------------------------------------------------------------------------------------------------
float shortestAngle(float _ang1, float _ang2);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Squares the given argument. Often quicker than typing it twice.
/// \param _arg value to square.
//----------------------------------------------------------------------------------------------------------------------
float sqr(float _arg);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given a point and two vectors representing a rectangle, returns whether the point lies inside.
/// \param _point point to test
/// \param _pos corner of the rectangle
/// \param _dim rectangle dimensions
//----------------------------------------------------------------------------------------------------------------------
bool pointInRect(vec2 _point, vec2 _pos, vec2 _dim);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given a point and two vectors representing an cuboid, returns whether the point lies inside.
/// \param _point point to test
/// \param _pos cuboid of the rectangle
/// \param _dim cuboid dimensions
//----------------------------------------------------------------------------------------------------------------------
bool pointInBox(vec3 _point, vec3 _start, vec3 _end);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns a vec2 with a magnitude between two floats.
/// \param _m min length, _M max length.
//----------------------------------------------------------------------------------------------------------------------
vec2 randVec2(float _m, float _M);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns a point inside the circle with a radius f.
/// \param _f radius
//----------------------------------------------------------------------------------------------------------------------
vec2 randVec2(float _f);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns a point in the rectangle defined by a minimum and maximum point.
/// \param _min the minimum point
/// \param _max the maximum point
//----------------------------------------------------------------------------------------------------------------------
vec2 randVec2(vec2 _min, vec2 _max);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns a vec3 with a magnitude between two floats.
/// \param _m min length
/// \param _M max length.
//----------------------------------------------------------------------------------------------------------------------
vec3 randVec3(float m, float M);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns a point inside the sphere with a radius f.
/// \param _point point to test
/// \param _pos corner of the rectangle
/// \param _dim rectangle dimensions
//----------------------------------------------------------------------------------------------------------------------
vec3 randVec3(float _f);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given a point and two vectors representing a rectangle, returns whether the point lies inside.
/// \param _point point to test
/// \param _pos corner of the rectangle
/// \param _dim rectangle dimensions
//----------------------------------------------------------------------------------------------------------------------
vec3 randVec3(vec3 min, vec3 max);

vec3 randVec3OnLine(vec3 _min, vec3 _max);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Splits a given string into a vector of strings, according to a given delimeter.
/// \param _str given string
/// \param _delim delimiting character
//----------------------------------------------------------------------------------------------------------------------
std::vector<std::string> split(std::string _str, char _delim);

template<class t>
void moveEntry(std::vector<t> * _src, int _i, std::vector<t> * _dst)
{
    _dst->push_back( (*_src)[_i] );
    swapnpop(_src, _i);
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief To avoid shuffling all elements in a vector when removing one, we can simply swap the element to remove, and
/// the element at the end, before popping. Of course, this should only be used when the order of the vector is
/// unimportant. I came accross the idea online, but the implementation is my own.
/// \param _vec pointer to the vector
/// \param _i index to remove
//----------------------------------------------------------------------------------------------------------------------
template<class t>
void swapnpop(std::vector<t> * _vec, int _i)
{
    iter_swap( _vec->begin() + _i, _vec->end() - 1 );
    _vec->pop_back();
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given an angle, returns a vector pointing forwards.
/// \param _ang angle
//----------------------------------------------------------------------------------------------------------------------
vec2 front(float _ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given an angle, returns a vector pointing back.
/// \param _ang angle
//----------------------------------------------------------------------------------------------------------------------
vec2 back(float _ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given an angle, returns a vector pointing left.
/// \param _ang angle
//----------------------------------------------------------------------------------------------------------------------
vec2 left(float _ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given an angle, returns a vector pointing right.
/// \param _ang angle
//----------------------------------------------------------------------------------------------------------------------
vec2 right(float _ang);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given a colour whose components range from 0-255, convert it to 0-1.
/// \param _col colour to convert
//----------------------------------------------------------------------------------------------------------------------
std::array<float, 4> col255to1(std::array<float, 4> _col);
std::array<float, 4> col255to1(std::array<int, 4> _col);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Given a colour whose components range from 0-1, convert it to 0-255.
/// \param _col colour to convert
//----------------------------------------------------------------------------------------------------------------------
std::array<float, 4> col1to255(std::array<float, 4> _col);
std::array<float, 4> col1to255(std::array<int, 4> _col);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Gets the mouse position as a vec2.
//----------------------------------------------------------------------------------------------------------------------
vec2 getMousePos();

//----------------------------------------------------------------------------------------------------------------------
/// \brief Converts a given vec2 from screen to world space.
//----------------------------------------------------------------------------------------------------------------------
vec2 toWorldSpace(vec2 _in);
vec2 toScreenSpace(vec2 _in);

template<class t>
t sumVec(const std::vector<t> _vec)
{
    t ret = 0;
    for(auto &i : _vec)
    {
        ret += i;
    }
    return ret;
}

//----------------------------------------------------------------------------------------------------------------------
/// \brief Like getline but works for text files from windows and linux.
/// \param _path path to the file
//----------------------------------------------------------------------------------------------------------------------
std::istream& getlineSafe(std::istream& is, std::string& t);

void debug(const std::string _msg);

void clearTerminal();

std::vector<SDL_Event> getEvents();

bool prob(const int _r);

void errorExit(const std::string &_msg);

#endif
