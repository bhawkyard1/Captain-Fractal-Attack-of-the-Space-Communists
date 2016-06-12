#ifndef FACTION_CPP
#define FACTION_CPP

#include <iostream>

#include "faction.hpp"
#include "ship.hpp"
#include "util.hpp"

faction::faction(std::string _name, std::array<float, 4> _col, aiTeam _team, shipBounds _fighters, shipBounds _utility, shipBounds _structures, bool _organised)
{
    m_identifier = _name;

    m_colour = _col;

    m_team = _team;

    m_relations = {DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL};

    m_combatShips = _fighters;
    m_utilityShips = _utility;
    m_structures = _structures;

    m_wealth = 0.0f;

    int range = _structures.second + 1 - _fighters.first;
    m_reserves.assign( range, 0 );
    m_deploy.assign( range, 0 );
    m_active.assign( range, 0 );

    m_aggression = randNum(0.0f, 1.0f);

    m_economy = randNum(0.00015f, 0.0001f);

    m_organised = _organised;
}

void faction::update(const float _dt, size_t _totalShips)
{
    if(!m_organised) return;

    _totalShips += 2;

    m_oldWealth = m_wealth;

    //std::cout << "  " << m_aggression << '\n';
    if(m_wealth > 0.0f) m_wealth += m_wealth * _dt * m_economy;
    else if(!(rand() & 128)) m_wealth += m_economy * _dt;

    float targetShips = _totalShips * m_aggression;

    //As aggression gets lower, a faction must be wealthier to spawn reserves.
    float wealthDT = (m_wealth - m_oldWealth) * _dt;

    //std::cout << "aggression " << m_aggression << '\n';
    //If the faction is too poor / not aggressive enough / already has enough ships, invest money.
    if( (rand() % 512) or wealthDT < -m_oldWealth / (10000.0f * m_aggression) )
    {
        //std::cout << "  investing\n";
        m_economy *= 1 + _dt * 0.000025f;
    }
    //If the faction is wealthy enough, purchase ships.
    else if(m_wealth > 0.0f)
    {
        //std::cout << "buying\n";
        addReserve();
    }

    size_t numReserves = 0;
    for(auto &i : m_reserves) numReserves += i;
    //std::cout << "  reserves : " << numReserves << " active : " << sumVec( m_active ) << '\n';

    //Deploy ships if there are too few in the field, enough in the reserves, and aggression is high enough.
    int prob = 512 / m_aggression / g_DIFFICULTY;
    if(!(rand() % prob) and sumVec( m_active ) < targetShips and sumVec( m_reserves ) > (targetShips - sumVec( m_active )) / 4)
    {
        int num = static_cast<int>(_totalShips * m_aggression);
        deploy( randNum( num / 2, num ) );
    }

    //debug("upkeep");
    float upkeep = 0.0f;
    for(auto i = m_combatShips.first; i <= m_combatShips.second; ++i)
    {
        upkeep += _dt * m_reserves.at(i - m_combatShips.first) * calcAICost(i) / 256.0f;
        upkeep += _dt * m_active.at(i - m_combatShips.first) * calcAICost(i) / 256.0f;
    }
    m_wealth -= upkeep;
    //debug("upkeep end");

    //Look at dt in wealth. If it is negative, below 1% of wealth multiplied by aggression and rand() hit, remove a ship.
    if(wealthDT < -m_wealth / (10000.0f * m_aggression) and rand() % 128)
    {
        for(auto &i : m_reserves)
        {
            if(i > 0)
            {
                --i;
                break;
            }
        }
    }

    m_aggression *= 0.999999f;
    if(!rand()) m_aggression = randNum(0.0f, 1.0f);
    //debug("update end");
}

/*void faction::addReserve()
{
    float seed = randNum(0.0f, 1.0f);

    //Distribution from 0 to 1.
    float func = pow( seed, 14 );

    //Clamp values so the faction can only purchase ships it can afford.
    int max = -1;

    std::vector<float> cost;
    for(ship_spec i = m_combatShips.first; i <= m_combatShips.second; ++i)
    {
        if(calcAICost(i) < m_wealth)
        {
            max++;
            cost.push_back( calcAICost(i) );
        }
        else
            break;
    }
    ////std::cout << "max calculated " << max << " vs min " << m_combatShips.first << '\n';

    if(max == -1) return;

    int range = max + static_cast<int>(m_combatShips.first);

    ////std::cout << "ADDING RESERVE: " << range << " * " << func << " = " << range * func << '\n';
    int offset = clamp( static_cast<int>(round( range * func )), 0, max );

    m_reserves.at(offset)++;
    m_wealth -= cost.at(offset);
}*/

void faction::addReserve()
{
    float maxCost = 0.0f;
    std::vector<float> rcosts;
    std::vector<float> costs;
    for(ship_spec i = m_combatShips.first; i < m_combatShips.second; ++i)
    {
        float cost = calcAICost(i);
        if(cost < m_wealth)
        {
            float rcost = 1.0f / cost;
            rcosts.push_back( rcost + maxCost );
            costs.push_back(cost);
            maxCost += rcost;
        }
        else
            break;
    }
    ////std::cout << "max calculated " << max << " vs min " << m_combatShips.first << '\n';

    if(rcosts.size() == 0) return;

    float rand = randNum(0.0f, maxCost);

    int offset = 0;
    for(size_t i = 0; i < rcosts.size(); ++i)
    {
        if(rand > rcosts[i])
            offset = i;
        else
            break;
    }

    m_reserves.at(offset)++;
    m_wealth -= costs.at(offset);
}

void faction::deploy(size_t _num)
{
    //debug( "    DEPLOY" );
    for(size_t i = 0; i < m_reserves.size(); ++i)
    {
        size_t pop = std::min(static_cast<size_t>(randNum(static_cast<int>(0), static_cast<int>(m_reserves[i] * m_aggression))), m_reserves.at(i));

        m_reserves.at(i) -= pop;
        m_deploy.at(i) += pop;
    }
}

void faction::unitDestroyed(const ship_spec _spec)
{
    addActive(_spec, -1);
    m_aggression += calcAICost(_spec) * 0.001f;
}

void faction::unitWithdrawn(const ship_spec _spec)
{
    addActive(_spec, -1);
    m_reserves[_spec - m_combatShips.first]++;
}

void faction::addAggression(const float _mult)
{
    m_aggression *= 1.0f + 0.01f * _mult;
}

void faction::addActive(const ship_spec _i, const int _v)
{
    debug("as");
    int index = _i - m_combatShips.first;
    int val = m_active[index] + _v;
    val = std::max(val, 0);

    m_active.at(index) = val;
    debug("ae");
}


#endif
