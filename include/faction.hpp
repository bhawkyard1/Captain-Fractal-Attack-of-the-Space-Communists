#ifndef FACTION_HPP
#define FACTION_HPP

#include <array>
#include <vector>

#include "enemy.hpp"
#include "squad.hpp"

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

enum diplomaticStatus {DIPLOMACY_ENEMY, DIPLOMACY_NEUTRAL, DIPLOMACY_FRIEND};

struct faction
{
public:
    faction(std::string _name, std::array<float, 4> _col, aiTeam _team, ship_spec _low, ship_spec _high, bool _organised);
    ~faction();
    void update(const float _dt, const size_t _totalShips);

    void addReserve();
    void deploy(size_t _num);

    std::vector<size_t> getReserves() const {return m_reserves;}
    void setReserves(const std::vector<size_t> &_reserves) {m_reserves = _reserves;}

    int getActive() const {return m_active;}
    void setActive(const int _active) {m_active = _active;}

    std::array<diplomaticStatus, 8> getRelations() const {return m_relations;}
    diplomaticStatus getRelations(const aiTeam _t) const {return m_relations[_t];}

    void setRelations(const std::array<diplomaticStatus, NUMBER_OF_FACTIONS> _relations) {m_relations = _relations;}
    void setRelations(const diplomaticStatus _relation, const int _i) {m_relations[_i] = _relation;}

    std::array<float, 4> getColour() const {return m_colour;}
    void setColour(const std::array<float, 4> _colour) {m_colour = _colour;}

    float getWealth() const {return m_wealth;}
    void setWealth(const float _wealth) {m_wealth = _wealth;}
    void addWealth(const float _wealth) {m_wealth += _wealth;}
    float * getWealthPt() {return &m_wealth;}

    aiTeam getTeam() const {return m_team;}

    std::vector<size_t> getDeployed() {return m_deploy;}
    void clearDeployed() {for(auto &i : m_deploy) i = 0;}

    ship_spec getShittiestShip() const {return m_bounds.first;}

    std::string getIdentifier() const {return m_identifier;}

    void addActive(const int _i) {m_active = clamp(m_active + _i, 0, I_INF);}

    void unitDestroyed(const ship_spec _spec);
    void unitWithdrawn(const ship_spec _spec);
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
    std::array<diplomaticStatus, NUMBER_OF_FACTIONS> m_relations;

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
    /// \brief Number of ships this faction has in the field
    //----------------------------------------------------------------------------------------------------------------------
    int m_active;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Aggression, makes a faction spawn ships more regularly
    //----------------------------------------------------------------------------------------------------------------------
    float m_aggression;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The range of ship types that this faction can spawn
    //----------------------------------------------------------------------------------------------------------------------
    std::pair<ship_spec, ship_spec> m_bounds;

    std::string m_identifier;
};

#endif
