#include "squad.cpp"

class faction
{
	vector<squad> squads;
	vector<int> relationships;
	float strength, aggression;
public:
	faction();
	void update(float dt);
};
