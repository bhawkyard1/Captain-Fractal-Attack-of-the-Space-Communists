#ifndef CLASS_PFX
#define CLASS_PFX

#include "pfx.hpp"

pfx::pfx(vec2 p, vec2 v, vec2 wv, size_t no, float force, SDL_Texture * pt)
{
	glow = pt;
	glowA = clamp(255.0f * (force * force) / 100.0f, 0.0f, 255.0f);

	active = true;
	
	pos = p;
	vel = v;
	
	col[0] = rand()%150+105;
	col[1] = rand()%150+105;
	col[2] = rand()%150+105;
	
	for(size_t i = 0; i < no; i++)
	{
		base particle;
		particle.setPos(p);
		particle.setPPos(p);
		
		particle.setVel( {randFloat(-force,force),randFloat(-force,force)} ); 
		particle.setWVel( vel + wv );
		
		particles.push_back(particle);
		
		alphas.push_back(rand()%100+55);
	}
}

void pfx::update(float dt)
{
	pos += (vel + wvel) * PIXEL_UNIT_CONVERSION * dt;
	
	SDL_RenderDrawPoint(renderer,pos.x,pos.y);
	
	bool done = true;
	
	for(size_t i = 0; i < alphas.size(); i++)
	{
		particles.at(i).updatePos( dt );
				
		if(alphas.at(i) > 0) 
		{
			alphas.at(i) -= 1;
			done = false;
		}
	}
	
	if(done) active = false;
	glowA -= randFloat(15,18);
}

void pfx::draw(float dt)
{
	for(size_t i = 0; i < alphas.size(); i++)
	{
		vec2 p = dt * particles.at(i).getPos() + (1 - dt) * particles.at(i).getPPos();
		vec2 v = particles.at(i).getVel() * ZOOM_LEVEL;
		
		p *= ZOOM_LEVEL;
		p += HALFWIN;
		
		SDL_SetRenderDrawColor(renderer, col[0], col[1], col[2], alphas.at(i));
		SDL_RenderDrawLine(renderer, p.x, p.y, p.x + v.x, p.y + v.y);
	}
	
	SDL_Rect dst;
	dst.w = (glowA / 255.0) * 256 * ZOOM_LEVEL;
	dst.h = dst.w;
	dst.x = (pos.x - dst.w / 2) * ZOOM_LEVEL + HALFWIN.x;
	dst.y = (pos.y - dst.h / 2) * ZOOM_LEVEL + HALFWIN.y;
	
	SDL_Texture * cpy = glow;
	
	SDL_SetTextureAlphaMod(cpy, glowA);
	SDL_SetTextureBlendMode(cpy, SDL_BLENDMODE_ADD);	
	
	SDL_RenderCopy(renderer, cpy, NULL, &dst);
	
	SDL_SetTextureAlphaMod(cpy, 255.0f);
}

#endif
