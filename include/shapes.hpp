#ifndef SHAPES_HPP
#define SHAPES_HPP

#include "util.hpp"
#include "vectors.hpp"

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
bool circleIntersectRect(vec2 _pos, float _r, vec2 _min, vec2 _dim);

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns whether a line intersects a given sphere.
/// \param _start line start
/// \param _end line end
/// \param _pos sphere position
/// \param _radius sphere radius
//----------------------------------------------------------------------------------------------------------------------
bool lineIntersectSphere(vec3 _start, vec3 _end, vec3 _pos, float _radius);

#endif
