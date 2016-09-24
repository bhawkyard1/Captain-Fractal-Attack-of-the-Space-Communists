#include "sfx.hpp"
#include <SDL_mixer.h>
#include <vector>
#include <string>
#include "common.hpp"

soundPlayer::soundPlayer()
{
    m_channels = 128;
    m_curChannel = 0;
}

void soundPlayer::sfxInit()
{
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
    {
        std::cerr << "Mix_OpenAudio() failed! " << SDL_GetError() << std::endl;
        //SDL_Quit();
        //exit(EXIT_FAILURE);
    }

    Mix_AllocateChannels(m_channels);
}

void soundPlayer::loadSound(std::string _path, int _len)
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
    m_snds.push_back(temp_vec);
}

void soundPlayer::loadMusic(std::string _path)
{
    std::string name = _path;
    name += ".wav";
    Mix_Music * temp = Mix_LoadMUS( name.c_str() );
    if(!temp) std::cerr << name + ": Sound loading error! " << SDL_GetError() << std::endl;
    m_music.push_back(temp);
}

void soundPlayer::loadSounds()
{
    loadSound(g_RESOURCE_LOC + "sfx/red_laser_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/green_laser_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/blue_laser_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/explosion_", 3);
    loadSound(g_RESOURCE_LOC + "sfx/ricochet_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/save_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/place_", 4);
    loadSound(g_RESOURCE_LOC + "sfx/menu_button_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/ui_fail_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/clunk_", 1);
    loadSound(g_RESOURCE_LOC + "sfx/radio_chatter_", 5);

    loadMusic(g_RESOURCE_LOC + "sfx/deep_space_0");
    loadMusic(g_RESOURCE_LOC + "sfx/soviet_national_anthem_0");
}

void soundPlayer::playSnd(sound _snd, vec3 _pos, float _vol)
{
    float dist = mag(_pos);
    float mul = 1000.0f / dist;

    mul *= _vol;

    if(mul < 0.01f) return;

    int left = clamp(-_pos.m_x / 2.0f, 0.0f, 255.0f);

    Mix_Volume(m_curChannel, mul * 128);
    Mix_SetPanning(m_curChannel,
                   left,
                   255 - left
                   );

    playSnd(_snd);
}

void soundPlayer::playUISnd(sound _snd)
{
    Mix_Volume(m_curChannel, 128);
    Mix_SetPanning(m_curChannel,
                   255,
                   255
                   );

    playSnd(_snd);
}

void soundPlayer::playSnd(sound _snd)
{
    if(g_GAME_STATE == MODE_MENU and _snd != MENU_SELECT_SND) return;
    size_t snd = static_cast<size_t>(_snd);
    if(snd >= m_snds.size()) return;

    size_t size = m_snds.at(snd).size();

    Mix_Chunk * to_play = m_snds.at(snd).at(rand()%size);

    Mix_PlayChannel( m_curChannel, to_play, 0 );
    //std::cout << Mix_PlayChannel( m_curChannel, to_play, 0 ) << std::endl;

    m_curChannel++;
    if(m_curChannel >= m_channels) m_curChannel = 0;
}

void soundPlayer::playMusic(size_t _mus)
{
    Mix_PlayMusic( m_music.at(_mus), -1 );
}

soundPlayer::~soundPlayer()
{
    for(auto &i : m_snds)
    {
        for(auto &j : i)
        {
            Mix_FreeChunk(j);
        }
    }
    m_snds.clear();

    Mix_HaltMusic();
    for(auto &i : m_music)
    {
        Mix_FreeMusic(i);
    }
    m_music.clear();

    Mix_CloseAudio();
}
