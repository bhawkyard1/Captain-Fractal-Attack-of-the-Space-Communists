#include "shapes.hpp"

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

bool circleIntersectCircle(vec2 _pos1, float _r1, vec2 _pos2, float _r2)
{
    return magns(_pos1 - _pos2) < sqr(_r1 + _r2);
}

bool lineIntersectSphere(vec3 _start, vec3 _end, vec3 _pos, float _radius)
{
    vec3 lineDir  = _end - _start;
    vec3 cp = closest(_start, lineDir, _pos);
    vec3 cd = cp - _pos;

    return (sqr(_radius) > magns(cd)) and pointOnLine(_start, _end, cp);
}

bool pointOnLine(vec3 _start, vec3 _end, vec3 _point)
{
    //Do cross product of start->end vs start->point, then check dx, dy are smaller for start->point.
    vec3 startToPoint = _point - _start;
    vec3 startToEnd = _end - _start;
    float dp = dotProdUnit(startToPoint, startToEnd);

    return (fabs(dp) > 0.99f and fabs(dp) < 1.01f) and pointInBox(_point, _start, _end);
}

SDL_Rect maxRect(const std::vector<SDL_Rect> _rects)
{
    SDL_Rect ret = {I_INF, I_INF, -I_INF, -I_INF};
    for(auto &i : _rects)
    {
        if(i.x < ret.x) ret.x = i.x;
        if(i.x + i.w > ret.w) ret.w = i.w;
        if(i.y < ret.y) ret.y = i.y;
        if(i.y + i.h > ret.h) ret.h = i.h;
    }
    ret.w -= ret.x;
    ret.h -= ret.y;

    return ret;
}
