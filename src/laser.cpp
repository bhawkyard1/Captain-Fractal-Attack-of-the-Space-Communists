#ifndef CLASS_LASER
#define CLASS_LASER

#include "laser.hpp"

laser::laser(vec2 p, vec2 v, float pAng, float pData[], ai_team pTeam)
{
	float temp_angle = pAng + randFloat(-pData[1],pData[1]);//rand()%(2*pData[1])-pData[1];
	vec2 vAdd = computeVector(temp_angle);
		
	setVel({v.x + vAdd.x * pData[3], v.y + vAdd.y * pData[3]});
	setPos(p);
	setPPos(p);
	
	ang = temp_angle;
	dmg = pData[2];

	col[0] = pData[4];
	col[1] = pData[5];
	col[2] = pData[6];
	
	spd = pData[3];
	
	team = pTeam;
	
	power = 255.0f;

  stop = pData[9];
}

void laser::update(float dt)
{
	power -= 32.0f * dt;
	updatePos(dt);
}

void laser::draw(float dt)
{
	vec2 p = dt * getPos() + (1 - dt) * getPPos();
	p *= ZOOM_LEVEL;
	p += HALFWIN;
		
	vec2 vec = computeVector( ang ) * ZOOM_LEVEL;
	
	float scol[4] = {col[0] * 1.2f, col[1] * 1.2f, col[2] * 1.2f, 255.0f * power};
	float ecol[4] = {col[0] * 0.8f, col[1] * 0.8f, col[2] * 0.8f, 0.0f};
	
	for(int i = 0; i < 4; ++i)
	{
		scol[i] = clamp(scol[i], 0.0f, 255.0f);
		ecol[i] = clamp(ecol[i], 0.0f, 255.0f);
	}
	
	drawLineGr( renderer, {p.x, p.y}, {p.x + vec.x * spd * 4.0f, p.y + vec.y * spd * 4.0f}, ecol, scol );
}

#endif
