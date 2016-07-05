#ifndef OBSERVER_HPP
#define OBSERVER_HPP

#include "enemy.hpp"
#include "player.hpp"
#include "ship.hpp"

enum event {EVENT_AGENT_DESTROYED, EVENT_SHIP_DESTROYED, EVENT_DAMAGE, EVENT_COLLIDE};

class observer
{
public:
    //virtual ~observer();
    //virtual void onNotify(const ship &entity, event type) = 0;
    //virtual void onNotify(const enemy &entity, event type) = 0;
    virtual void onNotify(const player &entity, event type) = 0;
};

#endif
