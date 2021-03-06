#ifndef BASE_HPP
#define BASE_HPP

#include "math/ang3.hpp"
#include "math/vec3.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file base.hpp
/// \brief This class acts as a base for most objects in game.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class base
/// \brief Contains position and velocity, and the functionality to update based on time difference.
//----------------------------------------------------------------------------------------------------------------------

class base
{
public:
    base();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for position
    //----------------------------------------------------------------------------------------------------------------------
    void setPos(const vec3 _val) {m_pos = _val;}
    void addPos(const vec3 _vec) {m_pos += _vec;}
    vec3 getPos() const {return m_pos;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for m_prevPos
    //----------------------------------------------------------------------------------------------------------------------
    vec3 getPPos() const {return m_prevPos;}
    void setPPos(const vec3 _p) {m_prevPos = _p;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Setters for velocity
    //----------------------------------------------------------------------------------------------------------------------
    void setVel(const vec3 _v) {m_vel = _v;}
    void setWVel(const vec3 _wv) {m_wvel = _wv;}
    void addVel(const vec3 _v) {m_vel += _v;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Setters for force
    //----------------------------------------------------------------------------------------------------------------------
    void setForce(const vec3 _v) {m_forces = _v;}
    void addForce(const vec3 _v) {m_forces += _v;}
    vec3 getForce() const {return m_forces;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters for velocity
    //----------------------------------------------------------------------------------------------------------------------
    vec3 getVel() const {return m_vel;}
    vec3 getWVel() const {return m_wvel;}

    void setAngle(const ang3 &_ang) {m_angle = _ang;}
    ang3 getAngle() const {return m_angle;}

    void setAngleVel(const ang3 &_angleVel) {m_angleVel = _angleVel;}
    ang3 getAngleVel() const {return m_angleVel;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the position of the object based on velocity, wvel, and time differece
    /// \param _dt the time difference since the last update.
    //----------------------------------------------------------------------------------------------------------------------
    void updateTransform(float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns a vec2 interpolated between current and previous position
    /// \param _dt interpolant
    //----------------------------------------------------------------------------------------------------------------------
    vec3 getIPos(const float _dt);

    vec3 forward();
    vec3 back();
    vec3 left();
    vec3 right();
    vec3 up();
    vec3 down();

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The last position of the object, used when drawing
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_prevPos;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The current position of the object
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_pos;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The current velocity of the object, relative to the world
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_vel;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The forces acting on this object
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_forces;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The current velocity of the object, relative to the player
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_wvel;

    ang3 m_angle;

    ang3 m_angleVel;
};

#endif
