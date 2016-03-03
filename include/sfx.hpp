#ifndef SFX_HPP
#define SFX_HPP

#include <SDL_mixer.h>
#include <vector>

enum sound {RED_LASER,GREEN_LASER,BLUE_LASER,EXPLOSION};

extern std::vector< std::vector<Mix_Chunk*> > snds;

void loadSounds();

void playSnd(size_t snd);

#endif
