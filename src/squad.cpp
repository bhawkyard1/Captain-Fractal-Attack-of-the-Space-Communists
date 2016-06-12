
#include "squad.hpp"
#include "util.hpp"

squad createSquad(aiTeam _t)
{
    squad r;
    r.m_team = _t;
    r.m_regroupDist = randNum(800.0f, 1200.0f);
    r.m_size = 0;
    r.m_max_size = rand() % 20 + 40;
    return r;
}
