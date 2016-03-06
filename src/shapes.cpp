#include "shapes.hpp"
#include <iostream>
bool lineIntersectCircle(vec2 _start, vec2 _end, vec2 _pos, float _radius)
{
  vec2 lineDir  = _end - _start;

  vec2 cp = closest(_start, lineDir, _pos);
  std::cout << "CLOSEST IS " << cp.x << ", " << cp.y << std::endl;
  vec2 cd = cp - _pos;
  return ((_radius * _radius) > magns(cd)) and pointOnLine(_start, _end, cp);
}

bool pointOnLine(vec2 _start, vec2 _end, vec2 _point)
{
  //Do cross product of start->end vs start->point, then check dx, dy are smaller for start->point.
  vec2 startToPoint = _point - _start;
  vec2 startToEnd = _end - _start;
  float cp = crossProd(startToPoint, startToEnd);
  return (cp == 0.0f) and (startToPoint.x < startToEnd.x);
}

bool circleIntersectRect(vec2 _pos, float _r, vec2 _min, vec2 _dim)
{
  vec2 closest = _min;
  closest.x = clamp(_pos.x, _min.x, _min.x + _dim.x);
  closest.y = clamp(_pos.y, _min.y, _min.y + _dim.y);
  vec2 dc = _pos - closest;
  return magns(dc) < _r;
}
