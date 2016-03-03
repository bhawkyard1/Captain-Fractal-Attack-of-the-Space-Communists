#include "laser.hpp"

laser::laser(vec2 p, vec2 v, float pAng, std::array<float, WEAPS_W> pData, ai_team pTeam)
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
