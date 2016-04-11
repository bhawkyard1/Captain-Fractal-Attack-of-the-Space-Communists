#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "util.hpp"
#include "vectors.hpp"

bool lineIntersectCircle(vec2 _start, vec2 _end, vec2 _pos, float _radius);
bool pointOnLine(vec2 _start, vec2 _end, vec2 _point);
bool circleIntersectRect(vec2 _pos, float _r, vec2 _min, vec2 _dim);

bool lineIntersectSphere(vec3 _start, vec3 _end, vec3 _pos, float _radius);
bool pointOnLine(vec3 _start, vec3 _end, vec3 _point);

#endif
