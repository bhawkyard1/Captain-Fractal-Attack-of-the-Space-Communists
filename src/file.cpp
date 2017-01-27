#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>

#include "file.hpp"
#include "sfx.hpp"

void saveGame(universe * uni)
{
    std::ofstream save( g_RESOURCE_LOC + "save.txt" );

    save 	<< "score " << uni->getScore() << '\n'
               //<< "mec " << uni->getMaxEnemyCount(GALACTIC_FEDERATION) << '\n'
               //<< "mcc " << uni->getMaxEnemyCount(SPACE_COMMUNISTS) << '\n'
            << "mwc " << uni->getMaxWingmanCount() << '\n'
            << "mmc " << uni->getMaxMinerCount() << '\n'
            << "nm " << uni->getPly()->getMissiles() << '\n'
            << "ps " << uni->getPly()->getVel().m_x << "," << uni->getPly()->getVel().m_y << ","
            << uni->getPly()->getHealth() << ","
            << uni->getPly()->getShield() << ","
            << uni->getPly()->getEnergy() << '\n'
            << "d " << g_DIFFICULTY << '\n'
            << "u ";

    for(int i = 0; i < UPGRADES_LEN; ++i) save << uni->getPly()->getUpgrade(i) << " ";

    save 	<< '\n'
               //<< "uidc " << g_shipIDCounter << '\n'
            << "enemies ";
    writeVectorEnemy(save, uni->getAgents());

    save << '\n'
         << "asteroids ";
		writeVectorAsteroid(save, &uni->getAsteroids()->m_objects);

    save << '\n'
         << "factions ";
    writeVectorFaction(save, uni->getFactions());

    save.close();

    std::cout << "SAVED" << '\n';
}

void writeVectorEnemy(std::ostream &_file, slotmap<enemy> * _u)
{
    slotmap<enemy> cpy = *_u;
    for(size_t i = 0; i < cpy.size(); ++i)
    {
        _file << "/|" << cpy[i].getClassification() << "," << cpy[i].getTeam() << "|"
              << cpy.getID(i).m_id << "," << cpy.getID(i).m_version << "," << cpy[i].getParent().m_id << "," << cpy[i].getParent().m_version << "|"
              << cpy[i].getParentOffset().m_x << "," << cpy[i].getParentOffset().m_y << "|"
              << cpy[i].getPos().m_x << "," << cpy[i].getPos().m_y << "|"
              << cpy[i].getVel().m_x << "," << cpy[i].getVel().m_y << "|"
              << cpy[i].getAng() << "|"
              << cpy[i].getHealth() << "," << cpy[i].getShield() << "," << cpy[i].getEnergy() << "|"
              << cpy[i].getMaxHealth() << "," << cpy[i].getMaxShield() << "," << cpy[i].getMaxEnergy() << "|"
              << cpy[i].getKills() <<"|"
              << cpy[i].getRadius() << "|"
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

void writeVectorFaction(std::ostream &_file, std::vector<faction> *_f)
{
    for(auto &i : *_f)
    {
        std::string name = i.getIdentifier();
				for(size_t n = 0; n < name.length(); ++n)
						if(name[n] == ' ') name[n] = '_';
        _file << "/|" << name << "|"
              << i.getTeam() << "|"
              << i.getCol(0) << "," << i.getCol(1) << "," << i.getCol(2) << "," << i.getCol(3) << "|";
        for(auto &r : i.getRelations())
        {
            _file << r << ",";
        }
        _file << "|"
              << i.getWealth() << "," << i.getOldWealth() << "," << i.getEconomicalStrength() << "|"
              << i.getAggression() << "|"
              << i.getFighters().first << "," << i.getFighters().second << "|"
              << i.getUtilities().first << "," << i.getUtilities().second << "|"
              << i.getStructures().first << "," << i.getStructures().second << "|"
              << i.isOrganised() << "|";
        for(auto &r : i.getActive())
        {
            _file << r << ",";
        }
        _file << "|";
        for(auto &r : i.getDeployed())
        {
            _file << r << ",";
        }
        _file << "|";
        for(auto &r : i.getReserves())
        {
            _file << r << ",";
        }
        _file << "|";
    }
}

void readVectorEnemy(std::string _str, universe * _u)
{
    std::vector<std::string> vecs = split( _str, '/' );

    for(size_t i = 1; i < vecs.size(); ++i)
    {
        std::string s = vecs[i];
        std::vector<std::string> stats = split( s, '|' );
        std::vector<std::string> stat;

        int type, team;
        stat = split(stats[1], ',');
        type = std::stof(stat[0]);
        team = std::stof(stat[1]);

        stat = split(stats[2], ',');
        slotmap<ship> * container = reinterpret_cast< slotmap<ship> *>(_u->getAgents());
        slotID<ship> id  (std::stoi(stat[0]), std::stoi(stat[1]), container);
        slotID<ship> parentID (std::stoi(stat[2]), std::stoi(stat[3]), container);

        vec3 parentOffset;
        stat = split(stats[3], ',');
        parentOffset.m_x = std::stof(stat[0]);
        parentOffset.m_y = std::stof(stat[1]);
        parentOffset.m_z = 0.0f;

        vec3 pos;
        stat = split(stats[4], ',');
        pos.m_x = std::stof(stat[0]);
        pos.m_y = std::stof(stat[1]);
        pos.m_z = 0.0f;

        vec3 vel;
        stat = split(stats[5], ',');
        vel.m_x = std::stof(stat[0]);
        vel.m_y = std::stof(stat[1]);
        vel.m_z = 0.0f;

        float ang = std::stof(stats[6]);

        float health, shield, energy;
        stat = split(stats[7], ',');
        health = std::stof(stat[0]);
        shield = std::stof(stat[1]);
        energy = std::stof(stat[1]);

        float maxhealth, maxshield, maxenergy;
        stat = split(stats[8], ',');
        maxhealth = std::stof(stat[0]);
        maxshield = std::stof(stat[1]);
        maxenergy = std::stof(stat[2]);

        unsigned long kills = std::stoi(stats[9]);

        enemy temp(pos , vel, static_cast<ship_spec>(type), static_cast<aiTeam>(team));
        temp.setPos(pos);

        temp.setAng(ang);
        temp.setMaxHealth(maxhealth, false);
        temp.setMaxShield(maxshield, false);
        temp.setMaxEnergy(maxenergy, false);
        temp.setHealth(health);
        temp.setShield(shield);
        temp.setEnergy(energy);
        temp.setKills(kills);

        //NEED TO SET UNIQUE ID
        //temp.setUniqueID(id);
        temp.setParent(parentID);
        temp.setParentOffset(parentOffset);

        _u->getAgents()->push_back(temp);
    }
}

void readVectorAsteroid(std::string _str, universe * _u)
{
    std::vector<std::string> vecs = split( _str, '/' );

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
        temp.setPos({pos.m_x, pos.m_y, 0.0f});

        temp.setAng(ang);
        temp.setHealth(health);

        _u->getAsteroids()->push_back(temp);
    }
}

void readVectorFaction(std::string _str, universe *_u)
{
    std::vector<std::string> vecs = split( _str, '/' );

    for(size_t i = 1; i < vecs.size(); ++i)
    {
        std::string s = vecs[i];
        std::vector<std::string> stats = split( s, '|' );
        std::vector<std::string> stat;

        std::string name = stats[1];
        for(size_t i = 0; i < name.length(); ++i)
        {
            if(name[i] == '_') name[i] = ' ';
        }
        aiTeam team = static_cast<aiTeam>(std::stoi(stats[2]));

        stat = split(stats[3], ',');
        std::array<float, 4> colour = {{
            std::stof(stat[0]),
            std::stof(stat[1]),
            std::stof(stat[2]),
            std::stof(stat[3])
        }};

        std::vector<diplomaticStatus> relations;
        stat = split(stats[4], ',');
        for(auto &i : stat)
						relations.push_back( static_cast<diplomaticStatus>( std::stoi(i)) );


        stat = split(stats[5], ',');
        float wealth = std::stof(stat[0]);
        float oldWealth = std::stof(stat[1]);
        float economy = std::stof(stat[2]);

        float aggression = std::stof(stats[6]);

        stat = split(stats[7], ',');
        shipBounds fighters = {
            static_cast<ship_spec>(std::stoi(stat[0])),
            static_cast<ship_spec>(std::stoi(stat[1]))
        };

        stat = split(stats[8], ',');
        shipBounds utilities = {
            static_cast<ship_spec>(std::stoi(stat[0])),
            static_cast<ship_spec>(std::stoi(stat[1]))
        };

        stat = split(stats[9], ',');
        shipBounds structures = {
            static_cast<ship_spec>(std::stoi(stat[0])),
            static_cast<ship_spec>(std::stoi(stat[1]))
        };

        bool organised = static_cast<bool>(std::stoi(stats[10]));

        std::vector<size_t> active;
        std::vector<size_t> deployed;
        std::vector<size_t> reserves;
        stat = split(stats[11], ',');
        for(auto &i : stat)
            active.push_back(std::stoul(i));
        stat = split(stats[12], ',');
        for(auto &i : stat)
            deployed.push_back(std::stoul(i));
        stat = split(stats[13], ',');
        for(auto &i : stat)
            reserves.push_back(std::stoul(i));

        faction f(name, colour, team, fighters, utilities, structures, organised);
        f.setRelations(relations);
        f.setWealth(wealth);
        f.setOldWealth(oldWealth);
        f.setEconomicalStrength(economy);
        f.setAggression(aggression);
				f.setActive( active );
				f.setDeployed( deployed );
				f.setReserves( reserves );

        _u->getFactions()->push_back(f);
    }
}

void loadGame(universe * uni)
{
    std::ifstream save(g_RESOURCE_LOC + "save.txt");
    std::string cur;

    unsigned long tempIDCounter;

    while(getlineSafe( save, cur ))
    {
        if(cur.length() == 0) continue;
        std::vector<std::string> strings = split(cur, ' ');

        for(size_t i = 0; i < strings.size(); i++)
        {
            if(strings[i] == "score") uni->setScore( stoi(strings[i+1], nullptr, 10) );
            else if(strings[i] == "mwc") uni->setMaxWingmanCount( stoi(strings[i+1], nullptr, 10) );
            else if(strings[i] == "mmc") uni->setMaxMinerCount( stoi(strings[i+1], nullptr, 10) );
            else if(strings[i] == "nm") uni->getPly()->setMissiles( stoi(strings[i+1], nullptr, 10) );
            else if(strings[i] == "d") g_DIFFICULTY = stoi(strings[i+1], nullptr, 10);
            else if(strings[i] == "iudc") tempIDCounter = stoi(strings[i+1], nullptr, 10);
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
            else if(strings[i] == "factions") { if(strings.size() > 1) readVectorFaction(strings.at(i + 1), uni); }
        }
    }
    save.close();

    std::cout << "LOADED" << std::endl;
}

void loadConfig()
{
    std::ifstream config( g_RESOURCE_LOC + "config.txt" );
    std::string cur;

    while(getline( config, cur ))
    {
        if(cur.length() == 0) continue;

        std::vector<std::string> strings = split(cur, ' ');

        for(size_t i = 0; i < strings.size(); i++)
        {
            if(strings[i] == "resolution") setResolution( strings[i+1], strings[i+2] );
            else if(strings[i] == "graphical_detail") g_GRAPHICAL_DETAIL = stoi(strings[i+1], nullptr, 10);
            else if(strings[i] == "devmode") g_DEV_MODE = static_cast<bool>( stoi(strings[i+1], nullptr, 10) );
            else if(strings[i] == "beastmode") g_BEAST_MODE = static_cast<bool>( stoi(strings[i+1], nullptr, 10) );
            else if(strings[i] == "difficulty") g_DIFFICULTY = stoi(strings[i+1], nullptr, 10);
        }
    }
    config.close();
    g_HALFWIN = {g_WIN_WIDTH / 2.0f, g_WIN_HEIGHT / 2.0f};
    std::cout << "g_HALFWIN is " << g_HALFWIN.m_x << ", " << g_HALFWIN.m_y << std::endl;

    std::cout << "Resolution: " << g_WIN_WIDTH << " x " << g_WIN_HEIGHT << std::endl;
    std::cout << "Difficulty: " << g_DIFFICULTY << std::endl;
    std::cout << "Devmode: " << g_DEV_MODE << std::endl;
}

void setResolution(std::string _width, std::string _height)
{
    if(_width == "NULL" or _height == "NULL")
    {
        g_WIN_WIDTH = -1;
        g_WIN_HEIGHT = -1;
    }
    else
    {
        g_WIN_WIDTH = std::stoi(_width);
        g_WIN_HEIGHT = std::stoi(_height);
    }
}

void setConfigValue(const std::string _entry, const int _val)
{
    std::ifstream src( g_RESOURCE_LOC + "config.txt");
    std::ofstream config( g_RESOURCE_LOC + "temp.txt");
    std::string cur;

    while(getlineSafe( src, cur ))
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

    std::remove(( g_RESOURCE_LOC + "config.txt").c_str());
    std::rename(( g_RESOURCE_LOC + "temp.txt").c_str(), ( g_RESOURCE_LOC + "config.txt").c_str());
}

void setConfigValue(const std::string _entry, const vec2 _val)
{
    std::ifstream src( g_RESOURCE_LOC + "config.txt");
    std::ofstream config( g_RESOURCE_LOC + "temp.txt");
    std::string cur;

    while(getlineSafe( src, cur ))
    {
        if(cur.length() == 0) continue;
        std::vector<std::string> strings = split(cur, ' ');

        if(strings[0] == _entry)
        {
            config << _entry << " " << _val.m_x << ' ' << _val.m_y << std::endl;
        }
        else
        {
            config << cur << std::endl;
        }
    }
    src.close();
    config.close();

    std::remove(( g_RESOURCE_LOC + "config.txt").c_str());
    std::rename(( g_RESOURCE_LOC + "temp.txt").c_str(), ( g_RESOURCE_LOC + "config.txt").c_str());
}

selection loadSelection(const std::string _path)
{
    selection menu;

    std::ifstream data( g_RESOURCE_LOC + "menus/" + _path );
    if(!data.is_open())
    {
        std::cerr << "Error! Cannot open text file in loadSelection(). Aborting...";
        exit(EXIT_FAILURE);
    }

    std::string cur;
    std::vector< std::string > allStrings;

    while(getlineSafe( data, cur ))
    {
        if(cur.length() > 0) allStrings.push_back(cur);
    }
    data.close();

    for(auto i = allStrings.begin(); i != allStrings.end(); ++i)
    {
        //std::cout << "READING STRING : " << *i << ", " << i->length() << ", last character is " << ((*i)[i->length() - 1] == '\r') << '\n';
        if(*i == "BUTTON START")
        {
            button temp;
            temp.setCost(-1);
            temp.setInitCost(-1);

            for(auto j = i; j != allStrings.end(); ++j)
            {
                if(*j == "BUTTON END")
                {
                    menu.add(temp);
                    i = j;
                    break;
                }
                std::vector<std::string> btnData = split(*j, ' ');

                if(btnData[0] == "Name")
                {
                    std::string tempStr = "";
                    for(size_t k = 1; k < btnData.size(); ++k)
                    {
                        tempStr += btnData[k];
                        if(k < btnData.size() - 1) tempStr += ' ';
                    }
                    temp.setLabel(tempStr);
                    temp.setInitLabel(tempStr);
                }
                else if(btnData[0] == "Pos")
                {
                    temp.setPos({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                    temp.setStart({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                    temp.setEnd({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                }
                else if(btnData[0] == "StartPos")
                {
                    temp.setPos({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                    temp.setStart({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                }
                else if(btnData[0] == "EndPos")
                {
                    temp.setEnd({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                }
                else if(btnData[0] == "Dim") temp.setDim({convertMeasureF(btnData[1]), convertMeasureF(btnData[2])});
                else if(btnData[0] == "Cost")
                {
                    temp.setCost(std::stoi(btnData[1]));
                    temp.setInitCost(std::stoi(btnData[1]));
                }
                else if(btnData[0] == "TextSize") temp.setTextSizeMul(std::stof(btnData[1]));
                else if(btnData[0] == "TextCol")
                {
                    temp.setTCol({{
                                     std::stoi(btnData[1]),
                                     std::stoi(btnData[2]),
                                     std::stoi(btnData[3]),
                                     std::stoi(btnData[4])
                                 }});
                }
                else if(btnData[0] == "BackCol")
                {
                    temp.setCol({{
                                    std::stoi(btnData[1]),
                                    std::stoi(btnData[2]),
                                    std::stoi(btnData[3]),
                                    std::stoi(btnData[4])
                                }});
                }
            }
        }
        else if(*i == "SELECTION START")
        {
            for(auto &j = i; j != allStrings.end(); ++j)
            {
                if(*j == "SELECTION END")
                {
                    i = j;
                    break;
                }

                std::vector<std::string> slnData = split(*j, ' ');

                if(slnData[0] == "SaveSelection") menu.setSaveSelected(static_cast<bool>( std::stoi( slnData[1], nullptr) ) );
                else if(slnData[0] == "Type") menu.setType(std::stoi(slnData[1]));
            }
        }
    }
    //std::cout << "menu loaded, size " << menu.getButtons()->size() << '\n';
    return menu;
}

ship loadShip(const std::string &_path, std::string * _asset, int _classification)
{
    std::ifstream data (_path);
    std::string cur;

    ship ret;
    ret.setClassification(static_cast<ship_spec>(_classification));

    while(getlineSafe( data, cur ))
    {
        if(cur.length() == 0) continue;

        std::vector<std::string> s = split(cur, ' ');

        for(size_t i = 0; i < s.size(); i++)
        {
            if(s[i] == "identifier") ret.setIdentifier( s[i + 1] );
            else if(s[i] == "asset") *_asset = s[i + 1];
            else if(s[i] == "type") ret.setType( static_cast<ship_type>(stoi(s[i+1])) );
            else if(s[i] == "size") ret.setRadius( stof(s[i + 1]) );
            else if(s[i] == "health") ret.setMaxHealth( stof(s[i + 1]), true );
            else if(s[i] == "shield") ret.setMaxShield( stof(s[i + 1]), true );
            else if(s[i] == "energy") ret.setMaxEnergy( stof(s[i + 1]), true );
            else if(s[i] == "inertia") ret.setInertia( stof(s[i + 1]), true );
            else if(s[i] == "enginepower") ret.setEnginePower( stof(s[i + 1]) );
            else if(s[i] == "generatorpower") ret.setGeneratorMul( stof(s[i + 1]) );
            else if(s[i] == "weaponrange") ret.addWeap( rand() % (stoi(s[i + 2]) - stoi(s[i + 1])) + stoi(s[i + 1]) );
            else if(s[i] == "weapons")
                for(size_t j = i + 1; j < s.size(); ++j)
                    ret.addWeap(stoi(s[j]));
            else if(s[i] == "angvelrange") ret.setAngVelRange( stof(s[i + 1]), stof(s[i + 2]), true );
            else if(s[i] == "holdspace") ret.getCargo()->setDim( {stof(s[i + 1]), stof(s[i + 2])} );
            else if(s[i] == "canshoot") ret.setCanShoot( stoi(s[i + 1]) );
            else if(s[i] == "canmove") ret.setCanMove( stoi(s[i + 1]) );
            else if(s[i] == "angvelrange") ret.setAngVelRange(stof(s[i + 1]), stof(s[i + 2]), true);
            else if(s[i] == "attachmentpoints")
            {
                std::vector<vec3> attachmentPoints;
                std::vector<std::string> vecs = split(s[i + 1], '|');
                for(auto &v : vecs)
                {
                    std::vector<std::string> vector = split(v, ',');
                    if(vector.size() == 3)
                        attachmentPoints.push_back(
                                    vec3(stof(vector[0]), stof(vector[1]), stof(vector[2]))
                                );
                }
                ret.setAttachmentPoints(attachmentPoints);
            }
        }
    }

    data.close();

    return ret;
}

std::vector<std::string> readLines(std::istream &_file)
{
    std::string cur;
    std::vector< std::string > ret;

    while(getlineSafe( _file, cur ))
    {
        if(cur.length() > 0) ret.push_back(cur);
    }

    return ret;
}

float convertMeasureF(std::string _str)
{
    std::string toConvert = "";
    for(size_t i = 0; i < _str.length() - 1; ++i) toConvert += _str[i];
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
    case 'i':
        return std::stof(toConvert, nullptr) + g_HALFWIN.m_x;
        break;
    case 'j':
        return std::stof(toConvert, nullptr) + g_HALFWIN.m_y;
        break;
    default:
        break;
    }
    return 0.0f;
}

int convertMeasureI(std::string _str)
{
    std::string toConvert = "";
    for(size_t i = 0; i < _str.length() - 1; ++i) toConvert += _str[i];

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

std::array<float, 10> loadWeapon(const std::string &_path)
{
    std::array<float, 10> ret;

    std::ifstream data (_path);
    if(!data.is_open())
    {
        std::cerr << "Weapon definition file " << _path << " could not be opened! Aborting...\n";
        exit(EXIT_FAILURE);
    }
    std::string cur;

    while(getlineSafe( data, cur ))
    {
        //std::cout << "loading line " << cur << '\n';
        if(cur.length() == 0) continue;

        std::vector<std::string> s = split(cur, ' ');

        for(size_t i = 0; i < s.size(); i++)
        {
            if(s[i] == "lasers") ret[0] = stof(s[i + 1]);
            else if(s[i] == "spread") ret[1] = stof(s[i + 1]);
            else if(s[i] == "damage") ret[2] = stof(s[i + 1]);
            else if(s[i] == "speed") ret[3] = stof(s[i + 1]);
            else if(s[i] == "red") ret[4] = stof(s[i + 1]);
            else if(s[i] == "green") ret[5] = stof(s[i + 1]);
            else if(s[i] == "blue") ret[6] = stof(s[i + 1]);
            else if(s[i] == "cost") ret[7] = stof(s[i + 1]);
            else if(s[i] == "cooldown") ret[8] = stof(s[i + 1]);
            else if(s[i] == "power") ret[9] = stof(s[i + 1]);
        }
    }

    data.close();

    return ret;
}
