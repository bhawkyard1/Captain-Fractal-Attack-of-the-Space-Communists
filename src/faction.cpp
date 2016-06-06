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
    m_active.assign( _high + 1 - _low, 0 );

    m_aggression = randNum(0.0f, 1.0f);

    m_economy = randNum(0.00001f, 0.0001f);

    m_organised = _organised;
}

void faction::update(const float _dt, size_t _totalShips)
{
    if(!m_organised) return;

    _totalShips += 2;

    m_oldWealth = m_wealth;

    //std::cout << "  " << m_aggression << '\n';
    if(m_wealth > 0.0f) m_wealth += m_wealth * _dt * m_economy;
    else if(!(rand() & 2048)) m_wealth += m_economy;

    float targetShips = _totalShips * m_aggression;

    //As aggression gets lower, a faction must be wealthier to spawn reserves.
    float wealthDT = (m_wealth - m_oldWealth) * _dt;

    //If the faction is too poor / not aggressive enough / already has enough ships, invest money.
    if( (rand() % 256) or wealthDT < -(m_oldWealth / 1000.0f) * m_aggression )
    {
        std::cout << "  investing\n";
        m_economy *= 1 + _dt * 0.00001f;
    }
    //If the faction is wealthy enough, purchase ships.
    else if(m_wealth > 0.0f)
    {
        std::cout << "buying\n";
        addReserve();
    }

    size_t numReserves = 0;
    for(auto &i : m_reserves) numReserves += i;
    //std::cout << "  reserves : " << numReserves << " active : " << sumVec( m_active ) << '\n';

    //Deploy ships if there are too few in the field, enough in the reserves, and aggression is high enough.
    if(sumVec( m_active ) < targetShips and !(rand() % 2048))
    {
        //std::cout << "  deploying " << static_cast<int>(_totalShips * m_aggression) << std::endl;
        deploy( randNum( 1, static_cast<int>(_totalShips * m_aggression) ) );
    }

    //debug("upkeep");
    float upkeep = 0.0f;
    for(auto i = m_bounds.first; i <= m_bounds.second; ++i)
    {
        std::cout << m_reserves[i - m_bounds.first] << " ";
        upkeep += _dt * m_reserves[i - m_bounds.first] * calcAICost(i) / 20.0f;
    }
    std::cout << '\n';
    m_wealth -= upkeep;
    //debug("upkeep end");

    //Look at dt in wealth. If it is negative, below 1% of wealth multiplied by aggression and rand() hit, remove a ship.
    if(wealthDT < -(m_wealth / 1000.0f) * m_aggression and rand() % 128)
    {
        std::cout << "culling\n";
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

void faction::addReserve()
{
    float seed = randNum(0.0f, 1.0f);

    //Cubic distribution from 0 to 1.
    float func = pow(seed, 3);

    //Clamp values so the faction can only purchase ships it can afford.
    int max = -1;

    std::vector<float> cost;
    for(ship_spec i = m_bounds.first; i <= m_bounds.second; ++i)
    {
        if(calcAICost(i) < m_wealth)
        {
            max++;
            cost.push_back( calcAICost(i) );
        }
        else
            break;
    }

    if(max == -1) return;

    int range = max - static_cast<int>(m_bounds.first);

    int offset = clamp( static_cast<int>(floor( range * func )), 0, max );

    m_reserves[ offset ]++;
    m_wealth -= cost[ offset ];
}

void faction::deploy(size_t _num)
{
    //debug( "    DEPLOY" );
    for(size_t i = 0; i < m_reserves.size(); ++i)
    {
        float x = static_cast<float>(i) / static_cast<float>(m_reserves.size());
        float func = sqr( x - 1.0f );
        func = clamp( func * _num, 0.0f, static_cast<float>( m_reserves[i]) );
        size_t pop = std::min(static_cast<size_t>(round(func)), m_reserves[i]);

        m_reserves[i] -= pop;
        m_deploy[i] += pop;
    }
}

void faction::unitDestroyed(const ship_spec _spec)
{
    addActive(_spec, -1);
    m_aggression += calcAICost(_spec) * 0.1f;
}

void faction::unitWithdrawn(const ship_spec _spec)
{
    addActive(_spec, -1);
    m_reserves[_spec - m_bounds.first]++;
}

void faction::addAggression(const float _mult)
{
    m_aggression *= 1.0f + 0.01f * _mult;
}

void faction::addActive(const ship_spec _i, const int _v)
{
    int index = _i - m_bounds.first;
    if(m_active[index] > 0) m_active[index]--;
}


#endif
