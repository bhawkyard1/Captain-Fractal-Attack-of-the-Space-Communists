#include "file.hpp"
#include "weapons.hpp"

#include <iostream>

std::vector<std::array<float,WEAPS_W>> g_weapons;

void loadWeapons()
{
    std::ifstream data (g_RESOURCE_LOC + "weapons/config.txt");
    std::string cur;

    while(getlineSafe( data, cur ))
    {
        if(cur.length() == 0)
            continue;
        std::cout << "loading weapon!\n";
        std::array<float, 10> weapon = loadWeapon(g_RESOURCE_LOC + "weapons/" + cur);
        g_weapons.push_back(
                    weapon
                    );
        std::cout << g_weapons.size() << '\n';
    }

    data.close();
}
