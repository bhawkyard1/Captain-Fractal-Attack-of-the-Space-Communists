#ifndef SFX_HPP
#define SFX_HPP

#include <SDL_mixer.h>
#include <vector>

enum sound {RED_LASER_SND, GREEN_LASER_SND, BLUE_LASER_SND, EXPLOSION_SND, RICOCHET_SND, SAVE_SND, PLACE_SND, MENU_SELECT_SND, MENU_FAIL_SND, CLUNK_SND};

extern std::vector< std::vector<Mix_Chunk*> > g_snds;
extern std::vector<Mix_Music*> g_music;

void sfxInit();

void loadSounds();

void playSnd(sound _snd);
void playMusic(size_t _mus);

void deleteSounds();

#endif
