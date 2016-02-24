#include <vector>
#include "ship.hpp"

std::vector<ship> GLOBAL_SHIPS;

//This function loads all the ships in the game into a global vector that we can copy from later.
void loadShips()
{
	for(int i = 0; i < static_cast<int>(SHIPS_END); ++i)
	{
		ship insert( {0.0f, 0.0f}, static_cast<ship_spec>(i) );
		GLOBAL_SHIPS.push_back(insert);
	}
	cout << "len of global ship presets is " << GLOBAL_SHIPS.size() << endl;;
}
