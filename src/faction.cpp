#ifndef FACTION_CPP
#define FACTION_CPP

#include <iostream>

#include "enemy.hpp"
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

    for(auto &i : m_resourceDemand) i = randNum(-1000.0f, 1000.0f);
}

//What do we spend money on?
void faction::updateEconomy(const float _dt)
{
    if(!m_organised) return;

    if(!rand())
        for(auto &i : m_resourceDemand) i = randNum(-1000.0f, 1000.0f);

    m_oldWealth = m_wealth;

    if(m_wealth > 0.0f) m_wealth += m_wealth * _dt * m_economy;
    else if(!(rand() & 128)) m_wealth += m_economy * _dt;

    //As aggression gets lower, a faction must be wealthier to spawn reserves.
    float wealthDT = (m_wealth - m_oldWealth) * _dt;

    //std::cout << "aggression " << m_aggression << '\n';
    //If the faction is too poor / not aggressive enough / already has enough ships, invest money.
    if( (rand() % 128) or wealthDT < -m_oldWealth / (10000.0f * m_aggression) )
    {
        m_economy *= 1 + _dt * 0.000025f;
    }
    //If the faction is wealthy enough, purchase ships.
    else if(m_wealth > 0.0f)
    {
        addReserve();
    }

    size_t numReserves = 0;
    for(auto &i : m_reserves) numReserves += i;
    //std::cout << "  reserves : " << numReserves << " active : " << sumVec( m_active ) << '\n';

    float upkeep = 0.0f;
    for(auto i = m_combatShips.first; i <= m_combatShips.second; ++i)
    {
        upkeep += _dt * m_reserves.at(i - m_combatShips.first) * calcAICost(i) / 128.0f;
        upkeep += _dt * m_active.at(i - m_combatShips.first) * calcAICost(i) / 128.0f;
    }
    m_wealth -= upkeep;

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
}

//Do we need to field more units? Where?
void faction::updateDeployment(const float _dt, const std::vector<faction> &_rivals)
{
    //return;
    if(!m_organised) return;

    for(int i = m_squads.m_objects.size() - 1; i >= 0; --i)
    {
        if(m_squads[i].m_size <= 0) m_squads.free(i);
    }

    //Enemies, neutrals, and friends
    std::array<float, 3> powerBalance = {0.0f, 0.0f, 0.0f};
    for(size_t f = 0; f < _rivals.size(); ++f)
    {
        aiTeam curTeam = static_cast<aiTeam>(f);
        int index = 2;
        //If enemies
        if(_rivals[f].getRelations(m_team) < DIPLOMACY_NEUTRAL and getRelations(curTeam) < DIPLOMACY_NEUTRAL) index = 0;
        //If neutral
        else if(_rivals[f].getRelations(m_team) == DIPLOMACY_NEUTRAL and getRelations(curTeam) == DIPLOMACY_NEUTRAL) index = 1;

        for(size_t slot = 0; slot < _rivals[f].m_active.size(); ++slot)
        {
            powerBalance[index] += calcAIPower( _rivals[curTeam].getShittiestShip() + slot ) * _rivals[curTeam].m_active[slot];
        }
    }

    //Power of all ships in reserve. What is the point of deploying if the faction has shit reserve power?
    float reservesPower = 0.0f;
    for(size_t r = 0; r < m_reserves.size(); ++r) reservesPower += calcAIPower(getShittiestShip() + r) * m_reserves[r];

    float targetPower = powerBalance[0] * (m_aggression + 0.25f);
    //std::cout << "ENEMY POWER: " << powerBalance[0] << " RESERVES POWER: " << reservesPower << " TARGET POWER: " << targetPower << '\n';

    //Deploy ships if there are too few in the field, enough in the reserves, and aggression is high enough.
    int prob = 512 / m_aggression / g_DIFFICULTY;
    if(
            !(rand() % prob) and
            powerBalance[2] < targetPower and //More friendlies needed
            reservesPower > targetPower - powerBalance[2] //Enough reserves to make deploying worthwhile.
            )
    {
        int num = static_cast<int>(sumVec(m_reserves) * m_aggression);
        deploy( randNum( num / 2, num ) );
    }
}

//How should active squads behave?
void faction::updateTactics(const float _dt, const std::vector<faction> &_rivals, const std::vector<enemy> &_ships)
{
    bool b = (m_team == ALLIANCE);

    std::vector<squad> enemySquads;
    for(size_t f = 0; f < _rivals.size(); ++f)
    {
        if(&_rivals[f] == this) continue;
        aiTeam curTeam = _rivals[f].getTeam();
        if(_rivals[f].getRelations(m_team) < DIPLOMACY_NEUTRAL and getRelations(curTeam) < DIPLOMACY_NEUTRAL)
        {
            //if(b) std::cout << "ADDING SQUADS FROM TEAM " << _rivals[f].getIdentifier() << '\n';
            enemySquads.reserve(enemySquads.size() + _rivals[f].getSquads().size());
            //Append rival factions squads to enemy squads.
            enemySquads.insert(
                        enemySquads.end(),
                        _rivals[f].getSquads().begin(),
                        _rivals[f].getSquads().end()
                        );
        }
    }
    //std::cout << m_identifier << " applying tactics, enemy squads: " << enemySquads.size() << '\n';

    for(auto &s : m_squads.m_objects)
    {
        s.m_targetPos += m_wvel * g_PIXEL_UNIT_CONVERSION * _dt;

        bool done = false;
        float bestDistance = F_INF;

        //Don't do shit if everything is too spread out.
        if(s.m_averageDistance > sqr(s.m_regroupDist))
        {
            if(b)
            {
                std::cout << "enemy squads len " << enemySquads.size() << '\n';
            }
            //Targeting enemy squads.
            for(auto &target : enemySquads)
            {
                float dist = magns(s.m_averagePos - target.m_targetPos);
                if(
                        dist < bestDistance and
                        s.m_strength * m_aggression > target.m_strength
                        )
                {
                    if(b) std::cout << "  targeting squad at " << target.m_averagePos.m_x << ", " << target.m_averagePos.m_y << " belonging to faction " << target.m_team << " of size " << target.m_size << '\n';
                    bestDistance = dist;
                    s.m_targetPos = target.m_averagePos;
                    done = true;
                }
            }
            if(b) std::cout << "targeting enemies\n";
            if( done ) continue;

            //Targeting of player.
            float dist = magns(s.m_averagePos);
            if(
                    getRelations(TEAM_PLAYER) < DIPLOMACY_NEUTRAL and
                    dist < bestDistance
                    )
            {
                bestDistance = dist;
                s.m_targetPos = vec3();
                done = true;
            }
            else if(
                    getRelations(TEAM_PLAYER) == DIPLOMACY_SELF
                    and dist > sqr(1500.0f)
                    )
            {
                s.m_targetPos = vec3();
                done = true;
            }
            if(b) std::cout << "targeting player\n";
            if( done ) continue;

            //Reinforcing of squads within the SAME faction.
            for(auto &ally : m_squads.m_objects)
            {
                if(&s == &ally) continue;

                float dist = magns(s.m_averagePos - ally.m_targetPos);
                if(
                        dist < bestDistance and
                        ally.m_strength < s.m_strength
                        )
                {
                    bestDistance = dist;
                    s.m_targetPos = ally.m_averagePos;
                    done = true;
                }
            }
            if(b) std::cout << "reinforcing teammates\n";
            if( done ) continue;

            //Targeting of individual ships/structures (since not every ship is in a squad).
            for(auto &ship : _ships)
            {
                //Skip if this ship is in a squad.
                if(
                        ship.getSquadID().m_version == -1 and
                        _rivals[ship.getTeam()].getRelations(m_team) < DIPLOMACY_NEUTRAL and getRelations(ship.getTeam()) < DIPLOMACY_NEUTRAL
                        )
                {
                    s.m_targetPos = ship.getPos();
                    done = true;
                }
            }
            if(b) std::cout << "chasing down stragglers\n";
            if( done ) continue;
            //std::cout << "p5\n";
            //If no targets, withdraw.
            s.m_targetPos = unit(s.m_averagePos) * 200000.0f;
            if(b) std::cout << "withdrawing\n";
        }
    }
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
    rcosts.push_back(0.0f);
    std::vector<float> costs;
    for(ship_spec i = m_combatShips.first; i < m_combatShips.second; ++i)
    {
        float cost = pow(calcAICost(i), 0.8f);
        if(cost < m_wealth)
        {
            float rcost = 1.0f / cost;
            rcosts.push_back( rcost + maxCost );
            costs.push_back( cost );
            maxCost += rcost;
        }
        else
            break;
    }

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
    //std::cout << "type " << g_ship_templates[offset + m_combatShips.first].getIdentifier() << " offset " << offset << ", m_reserves size " << m_reserves.size() << '\n';
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
    int index = _i - m_combatShips.first;
    int val = m_active[index] + _v;
    val = std::max(val, 0);

    m_active.at(index) = val;
}

void faction::resetSquads()
{
    for(auto &s : m_squads.m_objects)
    {
        s.m_averagePos = vec3();
        s.m_averageVel = vec3();
    }
}

#endif
