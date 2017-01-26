#ifndef AITARGET_HPP
#define AITARGET_HPP

#include "ship.hpp"
#include "slotmap.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file aiTarget.hpp
/// \brief This file contains the aiTarget class, used to wrap up targeting functionality
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class aiTarget
/// \brief Wraps up the stuff a ship can target into one object (the player, another agent, or an asteroid).
//----------------------------------------------------------------------------------------------------------------------

union targetUnion
{
    targetUnion()
    {
        m_player = nullptr;
    }

    ship * m_player;
    slotID<ship> m_agent;
    slotID<ship> m_asteroid;
};

class aiTarget
{
public:
    aiTarget();
    ship * get();
    void unset() {m_targetFlag = -1;}
    void setPlayer( ship * _player );
    void setAgent( slotID<ship> _agent );
    void setAsteroid( slotID<ship> _asteroid );
private:
    targetUnion m_target;
    int m_targetFlag;
};

#endif
