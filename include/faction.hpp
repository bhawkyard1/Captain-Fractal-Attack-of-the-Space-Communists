#ifndef FACTION_HPP
#define FACTION_HPP

#include "squad.hpp"
#include <vector>

class faction
{
  std::vector<squad> squads;
  std::vector<int> relationships;
	float strength, aggression;
public:
	faction();
	void update(float dt);
};

#endif
