#include "sfx.hpp"
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "common.hpp"

std::vector< std::vector<Mix_Chunk*> > g_snds;

void sfxInit()
{
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    {
        std::cerr << "Mix_OpenAudio() failed! " << SDL_GetError() << std::endl;
        //SDL_Quit();
        //exit(EXIT_FAILURE);
    }
}

void loadSound(std::string _path, int _len)
{
    std::vector<Mix_Chunk*> temp_vec;
    for(int i = 0; i < _len; i++)
    {
        std::string name = _path + std::to_string(i);
        name += ".wav";
        Mix_Chunk * temp = Mix_LoadWAV( name.c_str() );
        if(!temp) std::cerr << name + ": Sound loading error! " << SDL_GetError() << std::endl;
        temp_vec.push_back(temp);
    }
    g_snds.push_back(temp_vec);
}

void loadSounds()
{
    loadSound(g_RESOURCE_LOC + "sfx/red_laser_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/green_laser_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/blue_laser_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/explosion_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/ricochet_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/save_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/place_", 4);
}

void playSnd(sound _snd)
{
    if(g_GAME_STATE == MODE_MENU) return;
    size_t snd = static_cast<size_t>(_snd);
    if(snd >= g_snds.size()) return;

    size_t size = g_snds.at(snd).size();

    Mix_Chunk * to_play = g_snds.at(snd).at(rand()%size);

    Mix_PlayChannel( -1, to_play, 0 );
}

void deleteSounds()
{
    for(auto &i : g_snds)
    {
        for(auto &j : i)
        {
            Mix_FreeChunk(j);
        }
    }
    g_snds.clear();
    Mix_CloseAudio();
}
