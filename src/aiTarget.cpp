#include "aiTarget.hpp"

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
        return m_target.m_agent.get();
        break;
    case 2:
        return m_target.m_asteroid.get();
        break;
    }
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
    m_target.m_agent = _asteroid;
}
