#include "src/base.cpp"

class pfx
{
	vector<base> particles;
	vector<int> alphas;
	
	SDL_Texture * glow;
	float glowA;
	
	int col[3];
	
	vec2 pos, vel, wvel;
	bool active;
public:
	pfx(vec2,vec2,vec2,size_t,float,SDL_Texture*);
	void update(float);
	void draw(float);
	bool done() {return !active;}
	void setWVel(vec2 v) {wvel = v;}
	
	void clear() {SDL_DestroyTexture(glow);}
};
