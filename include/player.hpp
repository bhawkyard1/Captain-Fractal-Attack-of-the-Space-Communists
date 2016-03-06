#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ship.hpp"

class player: public ship
{
public:
    player(vec2 p, float _r): ship(p, PLAYER_SHIP, _r) {setWeap(0); setMissiles(3);}
	void ctrlUpdate();
};

#endif
