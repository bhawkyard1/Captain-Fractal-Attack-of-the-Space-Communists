#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include <vector>

#include "enemy.hpp"
#include "player.hpp"
#include "ship.hpp"

enum event {EVENT_AGENT_DESTROYED, EVENT_SHIP_DESTROYED, EVENT_DAMAGE, EVENT_COLLIDE};

class observer
{
public:
    void notify(const ship &entity, event type);
private:
    std::vector<event> m_eventStack;
};

#endif
