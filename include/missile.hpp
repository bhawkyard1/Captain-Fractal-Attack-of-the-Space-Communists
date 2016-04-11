#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "ship.hpp"
#include "enemy.hpp"

class missile: public ship
{
    aiTeam m_team;
    ship * m_target;
    bool m_det;
public:
    missile(const vec3 _p, const float _r);
    void setTarget(ship * _s) {m_target = _s;}
    ship * getTarget() const {return m_target;}
	void steering();
    bool detonate() const {return m_det;}
    aiTeam getTeam() const {return m_team;}
};

#endif
