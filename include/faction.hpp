#ifndef FACTION_HPP
#define FACTION_HPP

#include <array>
#include <vector>

#include "aiTypes.hpp"
//#include "debris.hpp"
#include "ship.hpp"
#include "slotMap.hpp"
#include "squad.hpp"

class enemy;

//----------------------------------------------------------------------------------------------------------------------
/// \file faction.hpp
/// \brief Keeps track of the faction data, currently only colour but could be extended.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an barebones initial version.
/// \struct faction
/// \brief Barebones implementation of factions, currently only contains colour.
//----------------------------------------------------------------------------------------------------------------------

typedef std::pair<ship_spec, ship_spec> shipBounds;

struct faction
{
public:
    faction(std::string _name, std::array<float, 4> _col, aiTeam _team, shipBounds _fighters, shipBounds _utility, shipBounds _structures, bool _organised);
    void updateEconomy(const float _dt);
    void updateDeployment(const float _dt, const std::vector<faction> &_rivals);
    void updateTactics(const float _dt, const std::vector<faction> &_rivals, const std::vector<enemy> &_ships);

    void addReserve();
    void deploy(size_t _num);

    std::vector<size_t> getReserves() const {return m_reserves;}
    void setReserves(const std::vector<size_t> &_reserves) {m_reserves = _reserves;}

    std::vector<size_t> getActive() const {return m_active;}
    void setActive(const std::vector<size_t> _active) {m_active = _active;}
    size_t getNumActive() const {return sumVec(m_active);}

    std::vector<diplomaticStatus> getRelations() const {return m_relations;}
    diplomaticStatus getRelations(const aiTeam _t) const {return m_relations[_t];}

    bool isSame(const aiTeam &_f) const {return m_team == _f;}
    bool isFriend(const aiTeam &_f) const {return m_relations[_f] > DIPLOMACY_NEUTRAL;}
    bool isNeutral(const aiTeam &_f) const {return m_relations[_f] >= DIPLOMACY_NEUTRAL;}
    bool isEnemy(const aiTeam &_f) const {return m_relations[_f] < DIPLOMACY_NEUTRAL;}

    void setRelations(const std::vector<diplomaticStatus> _relations) {m_relations = _relations;}
    void setRelations(const diplomaticStatus _relation, const int _i) {m_relations[_i] = _relation;}

    std::array<float, 4> getColour() const {return m_colour;}
    void setColour(const std::array<float, 4> _colour) {m_colour = _colour;}

    float getWealth() const {return m_wealth;}
    void setWealth(const float _wealth) {m_wealth = _wealth;}
    void addWealth(const float _wealth) {m_wealth += _wealth;}
    float * getWealthPt() {return &m_wealth;}
    float getOldWealth() const {return m_oldWealth;}
    void setOldWealth(const float _w) {m_oldWealth = _w;}
    float getEconomicalStrength() const {return m_economy;}
    void setEconomicalStrength(const float _e) {m_economy = _e;}

    aiTeam getTeam() const {return m_team;}

    std::vector<size_t> getDeployed() {return m_deploy;}
    void clearDeployed() {for(auto &i : m_deploy) i = 0;}

    ship_spec getShittiestShip() const {return m_combatShips.first;}

    std::string getIdentifier() const {return m_identifier;}

    void addActive(const ship_spec _i, const int _v);

    void unitDestroyed(const ship_spec _spec);
    void unitWithdrawn(const ship_spec _spec);

    void addAggression(const float _mult);
    void setAggression(const float _a) {m_aggression = _a;}
    float getAggression() const {return m_aggression;}

    size_t getDeployableRange() {return m_combatShips.second - m_combatShips.first;}

    float getCol(size_t _index) {return m_colour[_index];}

    shipBounds getFighters() const {return m_combatShips;}
    shipBounds getUtilities() const {return m_utilityShips;}
    shipBounds getStructures() const {return m_structures;}

    bool isOrganised() const {return m_organised;}

    uniqueID addSquad(const squad _s) {return m_squads.push_back(_s);}
    squad * getSquad(uniqueID _id) {return m_squads.getByID(_id);}
    uniqueID getBackSquad() {return m_squads.backID();}
    const std::vector<squad> & getSquads() const {return m_squads.m_objects;}

    void resetSquads();

    void setWVel(const vec3 _wvel) {m_wvel = _wvel;}
    vec3 getWVel() const {return m_wvel;}

    float getResourceDemand(const resourceType _type) const {return m_resourceDemand[_type];}
private:
    bool m_organised;
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Team
    //----------------------------------------------------------------------------------------------------------------------
    aiTeam m_team;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Faction colour
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 4> m_colour;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Faction relationships
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<diplomaticStatus> m_relations;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Wealth of the faction
    //----------------------------------------------------------------------------------------------------------------------
    float m_wealth;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Wealth last update, track change
    //----------------------------------------------------------------------------------------------------------------------
    float m_oldWealth;

    float m_economy;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Number of ships this faction can deploy
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<size_t> m_reserves;

    std::vector<size_t> m_deploy;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Ships this faction has in the field
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<size_t> m_active;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Aggression, makes a faction spawn ships more regularly
    //----------------------------------------------------------------------------------------------------------------------
    float m_aggression;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The range of ship types that this faction can spawn
    //----------------------------------------------------------------------------------------------------------------------
    std::pair<ship_spec, ship_spec> m_combatShips;
    std::pair<ship_spec, ship_spec> m_structures;
    std::pair<ship_spec, ship_spec> m_utilityShips;

    std::string m_identifier;

    slotMap<squad> m_squads;

    vec3 m_wvel;

    std::array<float, RESOURCE_NULL> m_resourceDemand;
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief Returns diplomatic status between two factions.
/// \param _a Team A.
/// \param _b Team B.
//----------------------------------------------------------------------------------------------------------------------
bool emnityCheck(const aiTeam _a, const aiTeam _b);
bool friendshipCheck(const aiTeam _a, const aiTeam _b);
bool neutralityCheck(const aiTeam _a, const aiTeam _b);
bool sameTeam(const aiTeam _a, const aiTeam _b);

#endif
