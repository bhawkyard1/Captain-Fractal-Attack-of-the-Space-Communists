#include "pfx.hpp"
#include <iostream>
#include "util.hpp"
#include "common.hpp"

pfx::pfx(vec2 p, vec2 v, vec2 wv, size_t no, float force, std::string _identifier)
{
	glowA = clamp(255.0f * (force * force) / 100.0f, 0.0f, 255.0f);

	active = true;
	
	pos = p;
	vel = v;
	
	col[0] = rand()%150+105;
	col[1] = rand()%150+105;
	col[2] = rand()%150+105;
	
  m_identifier = _identifier;

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
