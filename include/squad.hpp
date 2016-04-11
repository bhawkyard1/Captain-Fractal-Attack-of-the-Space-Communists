#ifndef SQUAD_HPP
#define SQUAD_HPP

#include "enemy.hpp"
#include <vector>

struct squad
{
    int m_id;
    int m_size;
    int m_max_size;
    aiTeam m_team;
    aiGoal m_squadGoal;
    vec3 m_centerPoint;
    vec3 m_averageVel;
    float m_regroupDist = 2000.0f;
    float m_strength;
};

squad createSquad(aiTeam _t);

#endif
