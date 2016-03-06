#include "enemy.hpp"

enemy::enemy(vec2 p , vec2 v, ship_spec pType, ai_team t, float _r):
ship(p, pType, _r)
{	
	curGoal = GOAL_IDLE; 
	setVel(v);
	stopDist = randFloat(200.0f,600.0f);
	if(pType == PLAYER_MINER_DROID) stopDist = randFloat(20.0f, 60.0f);
	target = nullptr;
	curGoal = GOAL_IDLE;
	team = t;
	confidence = randFloat(5.0f, 20.0f);
}

void enemy::behvrUpdate()
{
	if(target != nullptr)
	{
		tPos = target->getPos();
		tVel = target->getVel();
	}
	else
	{
		tPos = {randFloat(-30000.0f, 30000.0f), randFloat(-30000.0f, 30000.0f)};
		tVel = {0.0f, 0.0f};
		curGoal = GOAL_IDLE;
	}
	
	if(curGoal == GOAL_FLEE)
	{
		if(target != nullptr)
		{
			tPos = getPos() + (getPos() - target->getPos()) * 100.0f;
			tVel = {0.0f, 0.0f};
		}
		else
		{
			tPos = getPos();
			tPos.y += 10000.0f;
			tVel = {0.0f, 0.0f};
		}
	}
}

void enemy::steering()
{
	if(GAME_OVER or curGoal == GOAL_IDLE or curGoal == GOAL_SPACE_STATION) return;
	
	vec2 p = getPos();
	vec2 v = getVel();
	vec2 uv = unit(v);
	
	setFiring(false);
	
	/*We need to think about
	 * distance
	 * speed of each
	 * closing speed
	 */
	
	//This is the distance between the ship and its target position.
	float dist = mag( p - tPos );
	
	if(dist < getRadius()) accelerate(-0.5f);
	//This is the closing speed. Add player and ship vectors, find magnitude.
	//It isn't perfect, because I don't know whether the vectors are converging or diverging.
	//---> To solve this, I could look at using the dot product.
	// ---> I did. It worked.
	float cSpd = mag( getVel() - tVel );
	
	//This is how long (in frames) we predict the ship will take to arrive at its target position.
	float frames = 1.0f;
	//if(cSpd != 0.0f) frames = dist/cSpd;
	
	//Whereas angleMul is all about the ships angle in relation to its target angle, this is about its vector in relation to its target angle.
	//ie where the ship is going vs where is should be going.
	float vecMul = dotProd1(uv, computeVector(getTAng() + 90));
	
	//Player position, added to their current vector, multiplied by how long it'll take us to reach them.
	//In other words, where we think they will be, when we get to where they are now.
	vec2 tPPos;
	tPPos = tPos + tVel * clamp(frames, 0.0f, F_MAX);

	float tPPDist = mag( p - tPPos );
	
	/*
	 *The distance it will take the ship to stop, at max deceleration.
	 * 
	 * v^2 = u^2 + 2as
	 * -2as = u^2 - v^2
	 * s = (u^2 - v^2)/-2a
	 * s = (u^2 - v^2)/-2a * (-1/-1)
	 * s = (v^2 - u^2)/2a
	 * given v = spd, u = 0, a = -1:
	 */
	float stoppingDistance = 4 * ( cSpd * cSpd ) / 2;
	
	if(vecMul < 0) stoppingDistance *= -1;
	/* -2as = u^2 - v^2
	 * a =  - (v^2 - u^2) / 2s
	 */
	//float stopNowMult = ( -cSpd ) / ( 2 * tPPDist );
	
	//This controls how much the ship is to accelerate. It depends on the closing speed between the ship and its target, their distance apart, and whether the ship is moving towards the target, or away.
	float accelMul = clamp((( tPPDist - stoppingDistance - stopDist - target->getRadius() )),-0.5f, 0.5f) /*+ stopNowMult*/;
	//This varies between 1 (ship facing target) 0 (ship parallel to target) and -1 (ship facing away from target).
	//It does this by taking the ship's angle and its target angle, and determining the angle between them.
	float angleMul = (static_cast<float>(shortestAngle(getAng(), getTAng())) + 90.0f ) / 90.0f;

	//We need to compare the distance to the closing speed. If the two points are converging rapidly, the ship must slow down.
	//cSpd will be subtracted from dist once per frame. Ships can accelerate at max 1au per frame.
	//Therefore, frames until collision: f = dist/cSpd
	//if dist - factorial(cSpd) < 0, ship will overshoot. > 0, is will undershoot, and = 0, it will arrive at the correct position.
	
	vec2 linePos = closest(tPPos,unit(normal(v)),p);
	
	tPPos -= unit(linePos - tPPos) * cSpd * 3;
	
	//Angle the ship towards its target.
	setTAng(clampRoll(computeAngle(p-tPPos), -180.0f, 180.0f));

	//If we are angled towards the target...
	if(fabs(shortestAngle(getAng(),getTAng())) <= 8.0f)
	{
		float tvMul = dotProd1(tVel, getVel());
		if( ( tvMul < 0.8f or tvMul > 1.2f or tvMul == 0.0f ) and getEnergy() / getMaxEnergy() > 0.1f and !isStatic() ) accelerate(accelMul*angleMul);
		if(dist < 800.0f + target->getRadius() and ( curGoal == GOAL_ATTACK or curGoal == GOAL_TURRET ) and getEnergy() / getMaxEnergy() > 0.05f)
		{
			setFiring(true);
		}
	}
	
	//This variable represents the ships' direction versus its ideal direction.
	float vecMulSide = dotProd1(uv,computeVector(getTAng()));
	
	if(fabs(vecMulSide) > 0.8f)
	{
		//closing speed * how sideways it is flying * its angle relative to velocity
		float dv = clamp(cSpd * vecMulSide * dotProd1(uv, computeVector(getAng())), 0.0f, 1.0f);
		if(vecMulSide < 0) dodge( dv );
		else if(vecMulSide > 0) dodge( -dv );
	}
}
