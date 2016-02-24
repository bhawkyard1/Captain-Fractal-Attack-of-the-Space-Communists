#include "src/stardust.cpp"

class stardust_sprite: public stardust
{
	SDL_Texture * tex;
	float ang, angVel;
	float dim;
public:
	stardust_sprite(SDL_Texture *, float *);
	stardust_sprite(SDL_Texture *, float);
	void spriteGen(float*);
	void draw(float);
	void drawDim(float);
	SDL_Texture * getTex() {return tex;}
	void updateSprite(float);
	void incrDim();
	float getDim() {return dim;}
};
