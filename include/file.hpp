#include <string>
#include <fstream>
#include <sstream>

void writeVector(ostream &file, universe * u, int v);

void saveGame(universe * uni)
{
	ofstream save("save.txt");
	
	save 	<< "score " << uni->getScore() << endl
			<< "mec " << uni->getMaxEnemyCount() << endl
			<< "mwc " << uni->getMaxWingmanCount() << endl
			<< "mmc " << uni->getMaxMinerCount() << endl
			<< "nm " << uni->getPly()->getMissiles() << endl
			<< "d " << DIFFICULTY << endl
			<< "u ";
			
	for(int i = 0; i < UPGRADES_LEN; ++i) save << uni->getPly()->getUpgrade(i) << " ";
	
	save 	<< endl 
			<< "tp ";
	writeVector(save, uni, 0);
	
	save << "sp ";
	writeVector(save, uni, 1);
	
	save.close();
	
	cout << "SAVED" << endl;
}

void writeVector(ostream &file, universe * u, int v)
{
	vector<enemy> * e = u->getEnemies();
	switch(v)
	{
		case 0:
			//TURRETS
			for(size_t i = 0; i < e->size(); ++i)
			{
				if(e->at(i).getClassification() != 998) continue;
				vec2 p = e->at(i).getPos();
				file << "|" << p.x << "," << p.y;
			}
			file << endl;
			break;
		case 1:
			//STATIONS
			for(size_t i = 0; i < e->size(); ++i)
			{
				if(e->at(i).getClassification() != 999) continue;
				vec2 p = e->at(i).getPos();
				file << "|" << p.x << "," << p.y;
			}
			file << endl;
			break;
	}
}

void readVector(string str, universe * u, int v)
{
	cout << "enter" << endl;
	vector<string> vecs = split( str, '|' );
	cout << "vecslen " << vecs.size() << endl;
	switch(v)
	{
		case 0:
			//TURRETS
			for(size_t i = 1; i < vecs.size(); ++i)
			{
				vector<string> vals = split( vecs.at(i), ',' );
				u->addBuild({stof(vals.at(0), nullptr), stof(vals.at(1), nullptr)}, PLAYER_TURRET);
			}
			break;
		case 1:
			//STATIONS
			for(size_t i = 1; i < vecs.size(); ++i)
			{
				vector<string> vals = split( vecs.at(i), ',' );
				u->addBuild({stof(vals.at(0), nullptr), stof(vals.at(1), nullptr)}, PLAYER_STATION);
			}
			break;
	}
}

void loadGame(universe * uni)
{
	ifstream save("save.txt");	
	string cur;
			
	while(getline( save, cur ))
	{
		if(cur.length() == 0) continue;

		vector<string> strings = split(cur, ' ');
		
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
	
	cout << "LOADED" << endl;
}
