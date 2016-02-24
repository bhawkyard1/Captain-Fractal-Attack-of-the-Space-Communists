#ifndef FACTION_HPP
#define FACTION_HPP

#include "squad.hpp"

class faction
{
	vector<squad> squads;
	vector<int> relationships;
	float strength, aggression;
public:
	faction();
	void update(float dt);
#endif
};
