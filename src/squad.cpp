
#include "squad.hpp"
#include "util.hpp"

squad createSquad(aiTeam _t)
{
    squad r;
    r.m_team = _t;
    r.m_regroupDist = randNum(1500.0f, 3000.0f);
    r.m_size = 0;
    r.m_max_size = rand() % 5 + 20;
    return r;
}
