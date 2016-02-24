enum sound {RED_LASER,GREEN_LASER,BLUE_LASER,EXPLOSION};

vector< vector<Mix_Chunk*> > snds;

void loadSounds()
{
	vector<Mix_Chunk*> temp1;
	snds.push_back(temp1);
		
	for(int i = 0; i < 3; i++)
	{
		string name = "sfx/red_laser_" + to_string(i);
		name += ".wav";

		Mix_Chunk * temp = Mix_LoadWAV( name.c_str() );
		snds.at(0).push_back(temp);
	}
	
	vector<Mix_Chunk*> temp2;
	snds.push_back(temp2);
	for(int i = 0; i < 3; i++)
	{
		string name = "sfx/green_laser_" + to_string(i);
		name += ".wav";
		Mix_Chunk * temp = Mix_LoadWAV( name.c_str() );
		snds.at(1).push_back(temp);
	}
	
	vector<Mix_Chunk*> temp3;
	snds.push_back(temp3);
	for(int i = 0; i < 3; i++)
	{
		string name = "sfx/blue_laser_" + to_string(i);
		name += ".wav";
		Mix_Chunk * temp = Mix_LoadWAV( name.c_str() );
		snds.at(2).push_back(temp);
	}
	
	vector<Mix_Chunk*> temp4;
	snds.push_back(temp4);
	for(int i = 0; i < 3; i++)
	{
		string name = "sfx/explosion_" + to_string(i);
		name += ".wav";
		Mix_Chunk * temp = Mix_LoadWAV( name.c_str() );
		snds.at(3).push_back(temp);
	}
	
	vector<Mix_Chunk*> temp5;
	snds.push_back(temp5);
	for(int i = 0; i < 1; i++)
	{
		string name = "sfx/ricochet_" + to_string(i);
		name += ".wav";
		Mix_Chunk * temp = Mix_LoadWAV( name.c_str() );
		snds.at(4).push_back(temp);
	}
}

void playSnd(size_t snd)
{
	if(snd >= snds.size()) return;
	
	size_t size = snds.at(snd).size();
	
	Mix_Chunk * to_play = snds.at(snd).at(rand()%size);
		
	Mix_PlayChannel( -1, to_play, 0 );
}
