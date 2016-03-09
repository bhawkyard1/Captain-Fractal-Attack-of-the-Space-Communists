#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "ship.hpp"
#include "enemy.hpp"

class missile: public ship
{
    aiTeam m_team;
    ship * m_target;
	bool det;
public:
    missile(vec2 p, float _r);
    void setTarget(ship * s) {m_target = s;}
    ship * getTarget() {return m_target;}
	void steering();
	bool detonate() {return det;}
    aiTeam getTeam() {return m_team;}
};

#endif
