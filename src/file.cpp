#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "file.hpp"
#include "sfx.hpp"

void saveGame(universe * uni)
{
    std::ofstream save( g_RESOURCE_LOC + "../" + "save.txt" );

    save 	<< "score " << uni->getScore() << std::endl
            << "mec " << uni->getMaxEnemyCount(GALACTIC_FEDERATION) << std::endl
            << "mcc " << uni->getMaxEnemyCount(SPACE_COMMUNISTS) << std::endl
            << "mwc " << uni->getMaxWingmanCount() << std::endl
            << "mmc " << uni->getMaxMinerCount() << std::endl
            << "nm " << uni->getPly()->getMissiles() << std::endl
            << "ps " << uni->getPly()->getVel().m_x << "," << uni->getPly()->getVel().m_y << ","
            << uni->getPly()->getHealth() << ","
            << uni->getPly()->getShield() << ","
            << uni->getPly()->getEnergy() << std::endl
            << "d " << g_DIFFICULTY << std::endl
            << "u ";

    for(int i = 0; i < UPGRADES_LEN; ++i) save << uni->getPly()->getUpgrade(i) << " ";

    save 	<< std::endl
            << "enemies ";
    writeVectorEnemy(save, uni->getAgents());

    save << std::endl
         << "asteroids ";
    writeVectorAsteroid(save, uni->getAsteroids());

    save.close();

    playSnd(SAVE_SND);
    std::cout << "SAVED" << std::endl;
}

void writeVectorEnemy(std::ostream &_file, std::vector<enemy> *_u)
{    
    for(auto &i : *_u)
    {
        _file << "/|" << i.getClassification() << "," << i.getTeam() << "|"
              << i.getPos().m_x << "," << i.getPos().m_y << "|"
              << i.getVel().m_x << "," << i.getVel().m_y << "|"
              << i.getAng() << "|"
              << i.getHealth() << "," << i.getShield() << "," << i.getEnergy() << "|"
              << i.getRadius() << "|"
                 ;
    }
}

void writeVectorAsteroid(std::ostream &_file, std::vector<ship> *_u)
{
    for(auto &i : *_u)
    {
        _file << "/|" << i.getClassification() << "|"
              << i.getPos().m_x << "," << i.getPos().m_y << "|"
              << i.getVel().m_x << "," << i.getVel().m_y << "|"
              << i.getAng() << "|"
              << i.getHealth() << "," << i.getShield() << "," << i.getEnergy() << "|"
                 ;
    }
}

void readVectorEnemy(std::string str, universe * _u)
{
    std::vector<std::string> vecs = split( str, '/' );

    for(size_t i = 1; i < vecs.size(); ++i)
    {
        std::string s = vecs[i];
        std::vector<std::string> stats = split( s, '|' );
        std::vector<std::string> stat;

        int id, team;
        stat = split(stats[1], ',');
        id = std::stof(stat[0]);
        team = std::stof(stat[1]);

        vec3 pos;
        stat = split(stats[2], ',');
        pos.m_x = std::stof(stat[0]);
        pos.m_y = std::stof(stat[1]);
        pos.m_z = 0.0f;

        vec3 vel;
        stat = split(stats[3], ',');
        vel.m_x = std::stof(stat[0]);
        vel.m_y = std::stof(stat[1]);
        vel.m_z = 0.0f;

        float ang = std::stof(stats[4]);

        float health, shield, energy;
        stat = split(stats[5], ',');
        health = std::stof(stat[0]);
        shield = std::stof(stat[1]);
        energy = std::stof(stat[1]);

        enemy temp(pos , vel, static_cast<ship_spec>(id), static_cast<aiTeam>(team));
        temp.setPos({pos.m_x,pos.m_y});

        temp.setAng(ang);
        temp.setHealth(health);
        temp.setShield(shield);
        temp.setEnergy(energy);

        _u->getAgents()->push_back(temp);
    }
}

void readVectorAsteroid(std::string str, universe * _u)
{
    std::vector<std::string> vecs = split( str, '/' );

    for(size_t i = 1; i < vecs.size(); ++i)
    {
        std::string s = vecs[i];
        std::vector<std::string> stats = split( s, '|' );
        std::vector<std::string> stat;

        int id;
        id = std::stof(stats[1]);

        vec2 pos;
        stat = split(stats[2], ',');
        pos.m_x = std::stof(stat[0]);
        pos.m_y = std::stof(stat[1]);

        vec2 vel;
        stat = split(stats[3], ',');
        vel.m_x = std::stof(stat[0]);
        vel.m_y = std::stof(stat[1]);

        float ang = std::stof(stats[4]);

        float health, shield, energy;
        stat = split(stats[5], ',');
        health = std::stof(stat[0]);
        shield = std::stof(stat[1]);
        energy = std::stof(stat[1]);

        ship temp( g_ship_templates[id] );
        temp.setPos({pos.m_x,pos.m_y});

        temp.setAng(ang);
        temp.setHealth(health);

        _u->getAsteroids()->push_back(temp);
    }
}

void loadGame(universe * uni)
{
    std::ifstream save(g_RESOURCE_LOC + "../" + "save.txt");
    std::string cur;

    while(getline( save, cur ))
    {
        if(cur.length() == 0) continue;
        std::vector<std::string> strings = split(cur, ' ');

        for(size_t i = 0; i < strings.size(); i++)
        {
            if(strings[i] == "score") uni->setScore( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings[i] == "mec") uni->setMaxEnemyCount( stoi(strings.at(i+1), nullptr, 10), GALACTIC_FEDERATION );
            else if(strings[i] == "mcc") uni->setMaxEnemyCount( stoi(strings.at(i+1), nullptr, 10), SPACE_COMMUNISTS );
            else if(strings[i] == "mwc") uni->setMaxWingmanCount( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings[i] == "mmc") uni->setMaxMinerCount( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings[i] == "nm") uni->getPly()->setMissiles( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings[i] == "d") g_DIFFICULTY = stoi(strings.at(i+1), nullptr, 10);
            else if(strings[i] == "u")
            {
                for(int j = 0; j < UPGRADES_LEN; ++j)
                {
                    int lvl = stoi(strings.at(i + j + 1), nullptr, 10);
                    std::cout << "READING, LEVEL IS " << lvl << std::endl;
                    uni->getPly()->setGrade(j, clamp(lvl,0,9));

                    uni->upgradeSetLabels(1, j, uni->getPly()->getUpgrade(j));
                }
            }
            else if(strings[i] == "ps")
            {
                std::vector<std::string> stats = split(strings.at(i + 1), ',');
                vec3 vel = {std::stof(stats[0]), std::stof(stats[1]), 0.0f};
                float health = std::stof(stats[2]);
                float shield = std::stof(stats[3]);
                float energy = std::stof(stats[4]);
                player * ply = uni->getPly();
                ply->setVel(vel);
                uni->setVel(-vel);
                ply->setHealth(health);
                ply->setShield(shield);
                ply->setEnergy(energy);
            }
            else if(strings[i] == "enemies") { if(strings.size() > 1) readVectorEnemy(strings.at(i + 1), uni); }
            else if(strings[i] == "asteroids") { if(strings.size() > 1) readVectorAsteroid(strings.at(i + 1), uni); }
        }
    }
    save.close();

    playSnd(SAVE_SND);
    std::cout << "LOADED" << std::endl;
}

void loadConfig()
{
    std::ifstream config( g_RESOURCE_LOC + "../" + "config.txt" );
    std::string cur;

    while(getline( config, cur ))
    {
        if(cur.length() == 0) continue;

        std::vector<std::string> strings = split(cur, ' ');

        for(size_t i = 0; i < strings.size(); i++)
        {
            if(strings[i] == "res_x") g_WIN_WIDTH = stoi(strings.at(i+1), nullptr, 10);
            else if(strings[i] == "res_y") g_WIN_HEIGHT = stoi(strings.at(i+1), nullptr, 10);
            else if(strings[i] == "graphical_detail") g_GRAPHICAL_DETAIL = stoi(strings.at(i+1), nullptr, 10);
            else if(strings[i] == "devmode") g_DEV_MODE = static_cast<int>( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings[i] == "beastmode") g_BEAST_MODE = static_cast<int>( stoi(strings.at(i+1), nullptr, 10) );
            else if(strings[i] == "difficulty") g_DIFFICULTY = stoi(strings.at(i+1), nullptr, 10);
        }
    }
    config.close();
    g_HALFWIN = {g_WIN_WIDTH / 2.0f, g_WIN_HEIGHT / 2.0f};
    g_MAX_DIM = std::max( g_WIN_WIDTH, g_WIN_HEIGHT );
    std::cout << "g_HALFWIN is " << g_HALFWIN.m_x << ", " << g_HALFWIN.m_y << std::endl;

    std::cout << "Resolution: " << g_WIN_WIDTH << " x " << g_WIN_HEIGHT << std::endl;
    std::cout << "Difficulty: " << g_DIFFICULTY << std::endl;
    std::cout << "Devmode: " << g_DEV_MODE << std::endl;
}

void setConfigValue(const std::string _entry, const int _val)
{
    std::ifstream src( g_RESOURCE_LOC + "../" + "config.txt");
    std::ofstream config( g_RESOURCE_LOC + "../" + "temp.txt");
    std::string cur;

    while(getline( src, cur ))
    {
        if(cur.length() == 0) continue;
        std::vector<std::string> strings = split(cur, ' ');

        if(strings[0] == _entry)
        {
            config << _entry << " " << _val << std::endl;
        }
        else
        {
            config << cur << std::endl;
        }
    }
    src.close();
    config.close();

    std::remove(( g_RESOURCE_LOC + "../" + "config.txt").c_str());
    std::rename(( g_RESOURCE_LOC + "../" + "temp.txt").c_str(), ( g_RESOURCE_LOC + "../" + "config.txt").c_str());
}

selection loadSelection(const std::string _path)
{
    selection menu;

    std::ifstream data( g_RESOURCE_LOC + "../menus/" + _path );
    std::string cur;
    std::vector< std::string > allStrings;

    while(getline( data, cur ))
    {
        if(cur.length() > 0) allStrings.push_back(cur);
    }
    data.close();

    for(auto i = allStrings.begin(); i != allStrings.end(); ++i)
    {
        if(*i == "BUTTON START")
        {
            button temp;
            for(auto j = i; j != allStrings.end(); ++j)
            {
                if(*j == "BUTTON END")
                {
                    menu.add(temp);
                    break;
                }
                std::vector<std::string> btnData = split(*j, ' ');

                if(btnData[0] == "Name") temp.setLabel(btnData[1]);
                else if(btnData[0] == "Pos") temp.setPos({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                else if(btnData[0] == "Dim") temp.setDim({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                else if(btnData[0] == "Cost") temp.setCost(std::stoi(btnData[1]));
                else if(btnData[0] == "TextCol")
                {
                    temp.setTCol({
                                     stof(btnData[1], nullptr),
                                     stof(btnData[2], nullptr),
                                     stof(btnData[3], nullptr),
                                     stof(btnData[4], nullptr)
                                 });
                }
                else if(btnData[0] == "BackCol")
                {
                    temp.setCol({
                                     stof(btnData[1], nullptr),
                                     stof(btnData[2], nullptr),
                                     stof(btnData[3], nullptr),
                                     stof(btnData[4], nullptr)
                                });
                }
            }
        }
    }

    return menu;
}

float convertMeasureF(std::string _str)
{
    std::string toConvert = "";
    for(int i = 0; i < _str.length() - 1; ++i) toConvert += _str[i];

    switch(_str[ _str.length() - 1 ])
    {
    case 'a':
        return std::stof(toConvert, nullptr);
        break;
    case 'w':
        return std::stof(toConvert, nullptr) * g_WIN_WIDTH;
        break;
    case 'h':
        return std::stof(toConvert, nullptr) * g_WIN_HEIGHT;
        break;
    default:
        break;
    }
    return 0.0f;
}

int convertMeasureI(std::string _str)
{
    std::string toConvert = "";
    for(int i = 0; i < _str.length() - 1; ++i) toConvert += _str[i];

    switch(_str[ _str.length() - 1 ])
    {
    case 'a':
        return std::stoi(toConvert, nullptr);
        break;
    case 'w':
        return std::stoi(toConvert, nullptr) * g_WIN_WIDTH;
        break;
    case 'h':
        return std::stoi(toConvert, nullptr) * g_WIN_HEIGHT;
        break;
    default:
        break;
    }
    return 0.0f;
}
