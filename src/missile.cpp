#include "missile.hpp"

missile::missile(vec2 p): ship(p, ION_MISSILE_MKI) 
{
	setPos(p);
	setMaxHealth(10.0f, true);
	setMaxShield(5.0f, true);
	setTAng(0.0f);
	det = false;
}

void missile::steering()
{
	if(target != nullptr) 
	{
		vec2 tPos = target->getPos();
		
		float dist = mag(tPos - getPos());

		float cSpd = mag(getVel() - target->getVel());
		
		float frames = dist/cSpd;
		
		vec2 tPPos = target->getPos() + target->getVel() * frames;
		float tPPDist = mag(tPPos - getPos());

		float vecMul = dotProd1(unit(getVel()),computeVector(getTAng() + 90));
	
		float stoppingDistance = (cSpd*cSpd)/2;
		if(vecMul < 0) stoppingDistance *= -1;
		
		float accelMul = clamp(((tPPDist-stoppingDistance)),-3.0f,1.5f);

		float angleMul = (shortestAngle(getAng(), getTAng())+90)/90.0;
		
		setTAng(clampRoll(computeAngle(getPos() - tPos),-180.0f,180.0f));
				
		if(fabs(shortestAngle(getAng(), getTAng())) <= 2.0f) accelerate(accelMul*angleMul);
		
		if(magns(getPos() - target->getPos()) < 50*50*cSpd) 
		{
			det = true;
		}
	}
	else
	{
		setTAng(getAng());
		accelerate(1);
	}
}
