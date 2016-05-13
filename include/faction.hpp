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
    std::array<diplomaticStatus, 8> m_relations;
};

#endif
