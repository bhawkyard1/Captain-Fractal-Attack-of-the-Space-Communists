#include "squad.hpp"
#include "util.hpp"

squad createSquad(aiTeam _t)
{
    squad r;
    r.m_team = _t;
    r.m_regroupDist = randNum(800.0f, 1200.0f);
    r.m_size = 0;
    r.m_strength = 0.0f;
    r.m_confidence = 0.0f;
    r.m_max_size = rand() % 20 + 40;
    r.m_squadGoal = GOAL_CONGREGATE;

    if(_t == TEAM_PLAYER) r.m_targetPos = vec3();
    else r.m_targetPos = tovec3( randVec2(2000.0f, 100000.0f) );
    r.m_pAveragePos = r.m_targetPos;

    r.m_averagePos = vec3();
    r.m_averageVel = vec3();

    return r;
}
