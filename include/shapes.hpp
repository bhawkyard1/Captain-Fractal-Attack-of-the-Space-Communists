#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "util.hpp"
#include "math/vec2.hpp"
#include "math/vec3.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file shapes.hpp
/// \brief This file provides functions to detect collisions between geometric shapes.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns whether a given line intersects a given circle.
/// \param _start line start
/// \param _end line end
/// \param _pos circle position
/// \param _radius circle radius
//----------------------------------------------------------------------------------------------------------------------
bool lineIntersectCircle(vec2 _start, vec2 _end, vec2 _pos, float _radius);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns whether a given point lies on a line.
/// \param _start line start
/// \param _end line end
/// \param _point point to test
//----------------------------------------------------------------------------------------------------------------------
bool pointOnLine(vec2 _start, vec2 _end, vec2 _point);
bool pointOnLine(vec3 _start, vec3 _end, vec3 _point);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns whether a given circle intersects a given AABB.
/// \param _start line start,
/// \param _end line end
/// \param _min minimum point of the AABB
/// \param _dim dimensions of the AABB
//----------------------------------------------------------------------------------------------------------------------
bool circleIntersectRectRough(vec2 _pos, float _r, vec2 _min, vec2 _dim);
bool circleIntersectRect(vec2 _pos, float _r, vec2 _min, vec2 _dim);
bool circleIntersectCircle(vec2 _pos1, float _r1, vec2 _pos2, float _r2);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns whether a line intersects a given sphere.
/// \param _start line start
/// \param _end line end
/// \param _pos sphere position
/// \param _radius sphere radius
//----------------------------------------------------------------------------------------------------------------------
bool lineIntersectSphere(vec3 _start, vec3 _end, vec3 _pos, float _radius, vec3 *_contact);

bool sphereIntersectSphere(vec3 _pos1, float _r1, vec3 _pos2, float _r2);

template<class t>
void sphereSphereCollision(t * _a, t * _b)
{
    float dist = magns(_a->getPos() - _b->getPos());

    if(dist > sqr(_a->getRadius() + _b->getRadius())) return;

    dist = mag(_b->getPos() - _a->getPos());
    vec3 normal = (_b->getPos() - _a->getPos()) / dist;
    normal.m_z = 0.0f;

    //Spooky boolean math here we go
    float ainvmass = _a->getInertia();
    float binvmass = _b->getInertia();
    float cinvmass = ainvmass + binvmass;

    if(cinvmass == 0.0f) return;

    //Positional correction.
    float diff = (_a->getRadius() + _b->getRadius()) - dist;
    vec3 correction = diff * normal;
    correction.m_z = 0.0f;

    _a->addPos( -correction * (ainvmass / cinvmass) );
    _b->addPos( correction * (binvmass / cinvmass) );

    vec3 rv = _a->getVel() - _b->getVel();
    rv.m_z = 0.0f;
    float separation = dot(rv, normal);
    if(separation < 0.0f) return;

    float force = -1.6f * separation;
    force /= cinvmass;

    vec3 impulse = force * normal;
    impulse.m_z = 0.0f;

    _a->addVel( ainvmass * impulse );
    _b->addForce( -binvmass * impulse );
}

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
}

SDL_Rect maxRect(const std::vector<SDL_Rect> _rects);

#endif
