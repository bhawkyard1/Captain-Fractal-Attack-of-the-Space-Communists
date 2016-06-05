#ifndef FACTION_CPP
#define FACTION_CPP

#include <iostream>

#include "faction.hpp"
#include "ship.hpp"
#include "util.hpp"

faction::faction(std::string _name, std::array<float, 4> _col, aiTeam _team, ship_spec _low, ship_spec _high, bool _organised)
{
    m_identifier = _name;

    m_colour = _col;

    m_team = _team;

    m_relations = {DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL};

    m_bounds.first = _low;
    m_bounds.second = _high;

    m_wealth = 0.0f;

    m_reserves.assign( _high + 1 - _low, 0 );
    m_deploy.assign( _high + 1 - _low, 0 );

    m_aggression = randNum(0.0f, 1.0f);

    m_economy = randNum(0.00001f, 0.0001f);

    m_active = 0;

    m_organised = _organised;
}

faction::~faction()
{
    m_reserves.clear();
    m_deploy.clear();
}

void faction::update(const float _dt, const size_t _totalShips)
{
    if(!m_organised) return;

    m_oldWealth = m_wealth;

    //std::cout << "  " << m_aggression << '\n';
    if(m_wealth > 0.0f) m_wealth += m_wealth * _dt * m_economy;
    else if(!(rand() & 2048)) m_wealth += m_economy;

    float targetShips = _totalShips * m_aggression + 1;

    //As aggression gets lower, a faction must be wealthier to spawn reserves.
    float wealthDT = (m_wealth - m_oldWealth) * _dt;

    //If the faction is too poor / not aggressive enough / already has enough ships, invest money.
    if( (rand() % 256) or wealthDT < -(m_oldWealth / 1000.0f) * m_aggression )
    {
        m_economy *= 1 + _dt * 0.00001f;
        std::cout << "  investing\n";
    }
    //If the faction is wealthy enough, purchase ships.
    else
    {
        addReserve();
        std::cout << "  buying\n";
    }

    size_t numReserves = 0;
    for(auto &i : m_reserves) numReserves += i;
    std::cout << "  reserves : " << numReserves << " active : " << m_active << '\n';

    //Deploy ships if there are too few in the field, enough in the reserves, and aggression is high enough.
    if(m_active < targetShips and !(rand() % 2048))
    {
        deploy(randNum(1, static_cast<int>(_totalShips * m_aggression)));
        std::cout << "  deploying\n";
    }

    float upkeep = 0.0f;
    for(auto i = m_bounds.first; i <= m_bounds.second; ++i)
    {
        upkeep += _dt * m_reserves[i - m_bounds.first] * calcAICost(i) / 10.0f;
    }
    m_wealth -= upkeep;

    //Look at dt in wealth. If it is negative, below 1% of wealth multiplied by aggression and rand() hit, remove a ship.
    if(wealthDT < -(m_wealth / 1000.0f) * m_aggression and rand() % 128)
    {
        for(auto &i : m_reserves)
        {
            if(i > 0)
            {
                i -= 1;
                break;
            }
        }
    }

    m_aggression *= 0.999999f;
    if(!rand()) m_aggression = randNum(0.0f, 1.0f);
}

void faction::addReserve()
{
    float seed = randNum(0.0f, 1.0f);

    //Cubic distribution from 0 to 1.
    float func = pow(seed, 5);

    //Clamp values so the faction can only purchase ships it can afford.
    int max = -1;
    float cost = 0.0f;
    for(ship_spec i = m_bounds.first; i <= m_bounds.second; ++i)
    {
        if(calcAICost(i) < m_wealth)
        {
            max++;
            cost = calcAICost(i);
        }
    }

    if(max == -1) return;

    int range = max - static_cast<int>(m_bounds.first);

    int offset = clamp( static_cast<int>(round( range * func )), 0, max );

    m_reserves[ offset ]++;
    m_wealth -= cost;
}

void faction::deploy(size_t _num)
{
    for(size_t i = 0; i < m_reserves.size(); ++i)
    {
        float x = static_cast<float>(i) / static_cast<float>(m_reserves.size());
        float func = sqr( x - 1.0f );
        func = clamp( func * _num, 0.0f, static_cast<float>(m_reserves[i]) );

        size_t pop = clamp( static_cast<size_t>(round(func)), static_cast<size_t>(0), static_cast<size_t>(m_reserves[i]) );
        m_reserves[i] -= pop;
        m_deploy[i] += pop;
    }
    m_active += _num;
}

void faction::unitDestroyed(const ship_spec _spec)
{
    addActive(-1);
    m_aggression += calcAICost(_spec) * 0.1f;
}

void faction::unitWithdrawn(const ship_spec _spec)
{
    addActive(-1);
    m_reserves[_spec - m_bounds.first]++;
}

#endif
