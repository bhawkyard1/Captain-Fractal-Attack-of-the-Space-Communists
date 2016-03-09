#ifndef BASE_HPP
#define BASE_HPP

#include "vectors.hpp"
class base
{
    vec2 m_prevPos;
    vec2 m_pos;
    vec2 m_vel;
    vec2 m_wvel;
public:
    void setPos(const vec2 _val) {m_pos = _val;}
    vec2 getPos() const {return m_pos;}

    vec2 getPPos() const {return m_prevPos;}
    void setPPos(const vec2 _p) {m_prevPos = _p;}

    void setVel(const vec2 _v) {m_vel = _v;}
    void setWVel(const vec2 _wv) {m_wvel = _wv;}
    void addVel(const vec2 _v) {m_vel += _v;}

    vec2 getVel() const {return m_vel;}
    vec2 getWVel() const {return m_wvel;}

    void updatePos(float _dt);

    vec2 getInterpolatedPosition(const float _dt);
};

#endif
