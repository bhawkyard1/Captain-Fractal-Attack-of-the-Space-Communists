#include "shapes.hpp"
#include <iostream>

bool lineIntersectCircle(vec2 _start, vec2 _end, vec2 _pos, float _radius)
{
  vec2 lineDir  = _end - _start;

  vec2 cp = closest(_start, lineDir, _pos);
  vec2 cd = cp - _pos;
  return ((_radius * _radius) > magns(cd)) and pointOnLine(_start, _end, cp);
}

bool pointOnLine(vec2 _start, vec2 _end, vec2 _point)
{
  //Do cross product of start->end vs start->point, then check dx, dy are smaller for start->point.
  vec2 startToPoint = _point - _start;
  vec2 startToEnd = _end - _start;
  float cp = crossProd(startToPoint, startToEnd);

  return (fabs(cp) <= 0.01f) and (abs(startToPoint.m_x) <= abs(startToEnd.m_x));
}

bool circleIntersectRect(vec2 _pos, float _r, vec2 _min, vec2 _dim)
{
  vec2 closest = _min;
  closest.m_x = clamp(_pos.m_x, _min.m_x, _min.m_x + _dim.m_x);
  closest.m_y = clamp(_pos.m_y, _min.m_y, _min.m_y + _dim.m_y);
  vec2 dc = _pos - closest;
  return magns(dc) < _r;
}

bool lineIntersectSphere(vec3 _start, vec3 _end, vec3 _pos, float _radius)
{
  /*vec3 lineDir  = unit(_end - _start);

  float det = sqr( dotProd1(lineDir, _start - _pos) ) - magns(_start - _pos) + sqr(_radius);*/

  //std::cout << det << std::endl;
  vec3 lineDir  = _end - _start;
  vec3 cp = closest(_start, lineDir, _pos);
  vec3 cd = cp - _pos;

  //std::cout << ((_radius * _radius) > magns(cd)) << ", " << pointOnLine(_start, _end, cp) << std::endl;

  return ((_radius * _radius) > magns(cd)) and pointOnLine(_start, _end, cp);

  //return det >= 0.0f and pointOnLine(_start, _end, _pos);
}

bool pointOnLine(vec3 _start, vec3 _end, vec3 _point)
{
  //Do cross product of start->end vs start->point, then check dx, dy are smaller for start->point.
  vec3 startToPoint = _point - _start;
  vec3 startToEnd = _end - _start;
  float dp = dotProd2(startToPoint, startToEnd);

  return (fabs(dp) > 0.98f and fabs(dp) < 1.02f) and (magns(_start) < magns(_point) and magns(_point) < magns(_end));
}
