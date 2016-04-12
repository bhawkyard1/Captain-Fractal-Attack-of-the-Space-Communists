#include "sfx.hpp"
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "common.hpp"

std::vector< std::vector<Mix_Chunk*> > g_snds;
std::vector<Mix_Music*> g_music;

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

void loadMusic(std::string _path)
{

    std::string name = _path;
    name += ".wav";
    Mix_Music * temp = Mix_LoadMUS( name.c_str() );
    if(!temp) std::cerr << name + ": Sound loading error! " << SDL_GetError() << std::endl;
    g_music.push_back(temp);
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
    loadSound(g_RESOURCE_LOC + "sfx/menu_button_", 1);

    loadMusic(g_RESOURCE_LOC + "sfx/deep_space_0");
    loadMusic(g_RESOURCE_LOC + "sfx/soviet_national_anthem_0");
}

void playSnd(sound _snd)
{
    if(g_GAME_STATE == MODE_MENU and _snd != MENU_SELECT_SND) return;
    size_t snd = static_cast<size_t>(_snd);
    if(snd >= g_snds.size()) return;

    size_t size = g_snds.at(snd).size();

    Mix_Chunk * to_play = g_snds.at(snd).at(rand()%size);
    Mix_PlayChannel( -1, to_play, 0 );
}

void playMusic(size_t _mus)
{
    Mix_PlayMusic( g_music.at(_mus), -1 );
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

    Mix_HaltMusic();
    for(auto &i : g_music)
    {
        Mix_FreeMusic(i);
    }
    g_music.clear();

    Mix_CloseAudio();
}
