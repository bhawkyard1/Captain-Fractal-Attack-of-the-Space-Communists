#ifndef SQUAD_HPP
#define SQUAD_HPP

#include "enemy.hpp"
#include <vector>

struct squad
{
    int m_id;
    int m_size;
    int m_max_size;
    ai_team m_team;
    ai_goal m_squadGoal;
    vec2 m_centerPoint;
    float m_regroupDist = 2000.0f;
    float m_strength;
};

squad createSquad(ai_team _t);

#endif
