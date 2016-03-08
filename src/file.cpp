#include "file.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include "universe.hpp"

void saveGame(universe * uni)
{
    std::ofstream save(RESOURCE_LOC + "save.txt");

    save 	<< "score " << uni->getScore() << std::endl
            << "mec " << uni->getMaxEnemyCount() << std::endl
            << "mwc " << uni->getMaxWingmanCount() << std::endl
            << "mmc " << uni->getMaxMinerCount() << std::endl
            << "nm " << uni->getPly()->getMissiles() << std::endl
            << "d " << DIFFICULTY << std::endl
            << "u ";

    for(int i = 0; i < UPGRADES_LEN; ++i) save << uni->getPly()->getUpgrade(i) << " ";

    save 	<< std::endl
            << "enemies ";
    writeVector(save, uni->getEnemies());

    save.close();

    std::cout << "SAVED" << std::endl;
}

void writeVector(std::ostream &_file, std::vector<enemy> *_v)
{    
    for(auto &i : *_v)
    {
       _file << "/|" << i.getClassification() << "," << i.getTeam() << "|"
             << i.getPos().x << "," << i.getPos().y << "|"
             << i.getVel().x << "," << i.getVel().y << "|"
             << i.getAng() << "|"
             << i.getHealth() << "," << i.getShield() << "," << i.getEnergy() << "|"
                ;
    }
}

void readVector(std::string str, universe * u, int v)
{
    std::cout << "enter" << std::endl;
    std::vector<std::string> vecs = split( str, '/' );
    std::cout << "vecslen " << vecs.size() << std::endl;

    for(auto &s : vecs)
    {
        std::vector<std::string> stats = split( s, '|' );
        std::vector<std::string> stat;

        int id, team;
        stat = split(stats[0], ',');
        id = std::stof(stat[0]);
        team = std::stof(stat[1]);

        vec2 pos;
        stat = split(stats[1], ',');
        pos.x = std::stof(stat[0]);
        pos.y = std::stof(stat[1]);

        vec2 vel;
        stat = split(stats[2], ',');
        pos.x = std::stof(stat[0]);
        pos.y = std::stof(stat[1]);

        float ang = std::stof(stats[3]);

        float health, shield, energy;
        stat = split(stats[4], ',');
        health = std::stof(stat[0]);
        shield = std::stof(stat[1]);
        energy = std::stof(stat[1]);

        enemy temp(pos , vel, static_cast<ship_spec>(id), static_cast<ai_team>(team), u-> _r);
        temp.setPos(pos);
        temp.setVel(vel);
        temp.setAng(ang);
        temp.setHealth(health);
        temp.setShield(shield);
        temp.setEnergy(energy);

        u->getEnemies()->push_back(temp);
    }
}

void loadGame(universe * uni)
{
    std::ifstream save(RESOURCE_LOC + "save.txt");
    std::string cur;

    while(getline( save, cur ))
    {
        if(cur.length() == 0) continue;

        std::vector<std::string> strings = split(cur, ' ');

        for(size_t i = 0; i < strings.size(); i++)
        {
            if(strings.at(i) == "score") uni->setScore( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings.at(i) == "mec") uni->setMaxEnemyCount( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings.at(i) == "mwc") uni->setMaxWingmanCount( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings.at(i) == "mmc") uni->setMaxMinerCount( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings.at(i) == "nm") uni->getPly()->setMissiles( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings.at(i) == "d") DIFFICULTY = stoi(strings.at(i+1), nullptr, 10);
            else if(strings.at(i) == "u")
            {
                for(int j = 0; j < UPGRADES_LEN; ++j)
                {
                    int lvl = stoi(strings.at(i + j + 1), nullptr, 10);
                    uni->getPly()->setGrade(j, lvl);
                    //setUpgradeTextures(lvl, j);
                }
            }
            else if(strings.at(i) == "tp") { if(strings.size() > 1) readVector(strings.at(i + 1), uni, 0); }
            else if(strings.at(i) == "sp") { if(strings.size() > 1) readVector(strings.at(i + 1), uni, 1); }
        }
    }
    save.close();

    std::cout << "LOADED" << std::endl;
}
