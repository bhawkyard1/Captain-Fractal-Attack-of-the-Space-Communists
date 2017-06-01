#include "aiTarget.hpp"
#include "enemy.hpp"

aiTarget::aiTarget()
{
	m_targetFlag = -1;
}

ship * aiTarget::get()
{
	switch(m_targetFlag)
	{
	case -1:
		return nullptr;
		break;
	case 0:
		return m_target.m_player;
		break;
	case 1:
	{
		//I'm so, so sorry, I'll fix this one day.
		//Reinterpret casting vectors is a bad idea because accessing elements gets all messed up.
		//On the flipside, I can have this declared as enemy because it is an incomplete type (this class is used by enemy, and enemy is used by this).
		//My temporary solution is nasty casts.
		slotmap<ship> * pt = m_target.m_agent.m_address;
		slotmap<enemy> * pte = reinterpret_cast<slotmap<enemy>*>(pt);
		return dynamic_cast<ship *>( pte->getByID( slot( m_target.m_agent.m_id, m_target.m_agent.m_version ) ) );
		break;
	}
	case 2:
		return m_target.m_asteroid.get();
        break;
    default:
        return nullptr;
	}
    return nullptr;
}

void aiTarget::setPlayer( ship * _player )
{
	m_targetFlag = 0;
	m_target.m_player = _player;
}

void aiTarget::setAgent(slotID<ship> _agent)
{
	m_targetFlag = 1;
	m_target.m_agent = _agent;
}

void aiTarget::setAsteroid(slotID<ship> _asteroid)
{
	m_targetFlag = 2;
	m_target.m_asteroid = _asteroid;
}

