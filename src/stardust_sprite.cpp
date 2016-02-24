#ifndef CLASS_STARDUST_SPRITE
#define CLASS_STARDUST_SPRITE

#include "stardust_sprite.hpp"

stardust_sprite::stardust_sprite(SDL_Texture * pt, float colp[]): stardust(colp) 
{	
	tex = pt;
		
	ang = randFloat(0,360);
	angVel = randFloat(-0.12f, 0.12f);
	
	int w = 0;
	int h = 0;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	
	setPos({(rand()%(WIN_WIDTH+w)-w)*BG_DENSITY,(rand()%(WIN_HEIGHT+h)-h)*BG_DENSITY});
}

stardust_sprite::stardust_sprite(SDL_Texture * pt, float alph): stardust(alph) 
{	
	dim = 32.0f;
	tex = pt;
		
	ang = randFloat(0,360);
	angVel = randFloat(-0.12f, 0.12f);
	
	int w = 0;
	int h = 0;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
}

void stardust_sprite::spriteGen(float colp[])
{
	ang = randFloat(0,360);
	angVel = randFloat(-0.12f, 0.12f);
	
	int w = 0;
	int h = 0;
	
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	
	int m = max(w,h);
	//m *= getZ();
	m *= ZOOM_LEVEL;
	
	int colProb = rand()%100;

	if(colProb < colp[0])
	{
		setCol(0, rand()%26+230);
		setCol(1, rand()%40+30);
		setCol(2, rand()%40+10);
	}
	else if(colProb < colp[1])
	{
		setCol(0, rand()%40+150);
		setCol(1, rand()%40+140);
		setCol(2, rand()%26+230);
	}
	else
	{
		setCol(0, rand()%56+200);
		setCol(1, rand()%40+155);
		setCol(2, rand()%30+30);
	}
	
	int side = rand()%4;
	vec2 vec;
	if(side == 0)
	{
		//Top
		vec = { randFloat(0, (WIN_WIDTH + 2 * m) ) - m, randFloat(0, m) - 2 * m};
		vec *= BG_DENSITY;
		setPos(vec);
		setPPos(getPos());
	}
	else if(side == 1)
	{
		//Left
		vec = { randFloat(0, m) - 2 * m, randFloat(0, WIN_HEIGHT + 2 * m) - m };
		vec *= BG_DENSITY;
		setPos(vec);
		setPPos(getPos());
	}
	else if(side == 2)
	{
		//Right
		vec = { 2*m - randFloat(0, m), randFloat(0, WIN_HEIGHT + 2 * m) - m };
		vec *= BG_DENSITY;
		vec.x += WIN_WIDTH;
		setPos(vec);
		setPPos(getPos());
	}
	else if(side == 3)
	{
		//Bottom
		vec = { randFloat(0, WIN_WIDTH + 2 * m) - m, 2 * m - randFloat(0, m) };
		vec *= BG_DENSITY;
		vec.y += WIN_HEIGHT;
		setPos(vec);
		setPPos(getPos());
	}
}

void stardust_sprite::draw(float dt)
{
	vec2 bp = dt * getPos() + (1 - dt) * getPPos();
	bp *= ZOOM_LEVEL;
	bp += HALFWIN;
	
	int w = 0, h = 0;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	
	SDL_Rect dst;
	dst.w = w * ZOOM_LEVEL;
	dst.h = h * ZOOM_LEVEL;
	dst.x = bp.x - dst.w / 2;
	dst.y = bp.y - dst.h / 2;
	
	float z = getZ();
	dst.w *= z;
	dst.h *= z;
	
	SDL_Texture * cpy = tex;
	
	SDL_SetTextureAlphaMod(cpy, clamp( ZOOM_LEVEL * getCol(3) / ( (static_cast<float>(fabs( getWVel().x * getWVel().y )) ) / 300.0f + 1.0f ), 30.0f, 255.0f ));
	SDL_SetTextureColorMod(cpy,getCol(0),getCol(1),getCol(2));
	
	SDL_SetTextureBlendMode(cpy, SDL_BLENDMODE_ADD);
	
	SDL_RenderCopyEx(renderer, cpy, NULL, &dst, ang, NULL, SDL_FLIP_NONE);
}

void stardust_sprite::drawDim(float dt)
{
	vec2 bp = dt * getPos() + (1 - dt) * getPPos();
	bp *= ZOOM_LEVEL;
	bp += HALFWIN;
	
	int w = 0, h = 0;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	
	SDL_Rect dst;
	dst.w = dim * ZOOM_LEVEL;
	dst.h = dim * ZOOM_LEVEL;
	dst.x = bp.x - dst.w / 2;
	dst.y = bp.y - dst.h / 2;
	
	float z = getZ();
	dst.w *= z;
	dst.h *= z;
	
	SDL_Texture * cpy = tex;
	
	SDL_SetTextureAlphaMod(cpy, clamp(ZOOM_LEVEL * getCol(3), 0.0f, 255.0f));
	
	SDL_SetTextureBlendMode(cpy, SDL_BLENDMODE_BLEND);	
	
	SDL_RenderCopyEx(renderer, cpy, NULL, &dst, ang, NULL, SDL_FLIP_NONE);
}

void stardust_sprite::updateSprite(float dt)
{
	ang += angVel;
	updatePos(dt);
}

void stardust_sprite::incrDim()
{
	if(tex == SMOKE_TEXTURE)
	{
		dim += 2.0f;
	}
	else if(tex == XPLO_TEXT)
	{
		dim -= 2.0f;
	}
}

#endif
