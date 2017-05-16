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

bool circleIntersectRectRough(vec2 _pos, float _r, vec2 _min, vec2 _dim)
{
    vec2 max = _min + _dim;
    return
            _pos.m_x + _r > _min.m_x and
            _pos.m_x - _r < max.m_x and
            _pos.m_y + _r > _min.m_y and
            _pos.m_y - _r < max.m_y;
}

bool circleIntersectRect(vec2 _pos, float _r, vec2 _min, vec2 _dim)
{
    vec2 closest = _min;
    closest.m_x = clamp(_pos.m_x, _min.m_x, _min.m_x + _dim.m_x);
    closest.m_y = clamp(_pos.m_y, _min.m_y, _min.m_y + _dim.m_y);
    vec2 dc = _pos - closest;
    return magns(dc) < sqr(_r);
}

bool circleIntersectCircle(vec2 _pos1, float _r1, vec2 _pos2, float _r2)
{
    return magns(_pos1 - _pos2) < sqr(_r1 + _r2);
}

bool lineIntersectSphere(vec3 _start, vec3 _end, vec3 _pos, float _radius, vec3 * _contact)
{
    vec3 lineDir = _end - _start;
    vec3 cp = closest(_start, lineDir, _pos);
    vec3 clamped = clamp(cp, _start, _end);

    if(_contact != nullptr) *_contact = clamped;

    vec3 cd = clamped - _pos;

    return (sqr(_radius) > magns(cd));
}

bool sphereIntersectSphere(vec3 _pos1, float _r1, vec3 _pos2, float _r2)
{
	float d = magns(_pos2 - _pos1);
	return d < sqr(_r1 + _r2);
}

/*bool pointOnLine(vec3 _start, vec3 _end, vec3 _point)
{
    //Do cross product of start->end vs start->point, then check dx, dy are smaller for start->point.
    vec3 startToPoint = _point - _start;
    vec3 startToEnd = _end - _start;
    float dp = udot(startToPoint, startToEnd);

    return pointInBox(_point, _start, _end) and (fabs(dp) > 0.99f and fabs(dp) < 1.01f);
}*/

bool pointOnLine(vec3 _start, vec3 _end, vec3 _point)
{
    //Do cross product of start->end vs start->point, then check dx, dy are smaller for start->point.
    vec3 startToPoint = _point - _start;
    float startToPointLen = mag(startToPoint);
    startToPoint /= startToPointLen;

    vec3 startToEnd = unit(_end - _start);
    float startToEndLen = mag(startToEnd);
    startToEnd /= startToEndLen;

    float dp = dot(startToPoint, startToEnd);

    return dp > 0.9f and dp < 1.1f and startToPointLen < startToEndLen;
}

/*
template<class t>
SDL_Rect enclose(const std::vector<t> &_ents)
{
    if(_ents.size() == 0) return {0, 0, 0, 0};

    int maxX = -I_INF;
    int maxY = -I_INF;
    int minX = I_INF;
    int minY = I_INF;
    for(auto &i : _ents)
    {
        vec2 pos = tovec2(i.getPos());

        if(pos.m_x < minX) minX = pos.m_x;
        if(pos.m_x > maxX) maxX = pos.m_x;
        if(pos.m_y < minY) minY = pos.m_y;
        if(pos.m_y > maxY) maxY = pos.m_y;
    }

    return {minX, minY, maxX - minX, maxY - minY};
}*/

SDL_Rect maxRect(const std::vector<SDL_Rect> _rects)
{
    int maxX = -I_INF;
    int maxY = -I_INF;
    int minX = I_INF;
    int minY = I_INF;

    for(auto &i : _rects)
    {
        if(i.x < minX) minX = i.x;
        if(i.y < minY) minY = i.y;
        if(i.x + i.w > maxX) maxX = i.x + i.w;
        if(i.y + i.h > maxY) maxY = i.y + i.h;
    }

    return {minX, minY, maxX - minX, maxY - minY};
}
