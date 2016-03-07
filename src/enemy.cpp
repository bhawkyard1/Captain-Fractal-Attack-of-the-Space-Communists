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
    setFiring(false);

    if(GAME_OVER or curGoal == GOAL_IDLE or curGoal == GOAL_SPACE_STATION) return;
    vec2 p = getPos();
    vec2 v = getVel();
    vec2 uv = unit(v);
    vec2 utv = unit(tPos - p);

    //This is the distance between the ship and its target position.
    float dist = mag( p - tPos );

    if(dist < getRadius() and getCanMove()) accelerate(-1.0f);

    //This is the closing speed. Add player and ship vectors, find magnitude.
    //Initially I didn't know whether the vectors were converging or diverging.
    //I solved it by multiplying by the dot of the ship and target vectors.
    float cSpd = mag( v - tVel );// * dotProd1(tVel, uv);

    //Whereas angleMul is all about the ships angle in relation to its target angle, this is about its vector in relation to its target angle.
    //ie where the ship is going vs where is should be going.
    float vecMul = dotProd1(uv, computeVector(getTAng() + 90));

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

    //This controls how much the ship is to accelerate. It depends on the closing speed between the ship and its target, their distance apart, and whether the ship is moving towards the target, or away.
    float accelMul = clamp((( dist - stoppingDistance - stopDist - target->getRadius() )),-0.5f, 0.5f);
    //This varies between 1 (ship facing target) 0 (ship parallel to target) and -1 (ship facing away from target).
    //It does this by taking the ship's angle and its target angle, and determining the angle between them.
    float angleMul = (static_cast<float>(shortestAngle(getAng(), getTAng())) + 90.0f ) / 90.0f;

    //We need to compare the distance to the closing speed. If the two points are converging rapidly, the ship must slow down.
    //cSpd will be subtracted from dist once per frame. Ships can accelerate at max 1au per frame.
    //Therefore, frames until collision: f = dist/cSpd
    //if dist - factorial(cSpd) < 0, ship will overshoot. > 0, is will undershoot, and = 0, it will arrive at the correct position.

    vec2 linePos = closest(tPos,unit(normal(v)),p);

    tPos -= unit(linePos - tPos) * cSpd * 3;

    //Angle the ship towards its target.
    setTAng(clampRoll(computeAngle(p-tPos), -180.0f, 180.0f));
    //If we are angled towards the target...

    float tvMul = dotProd1(tVel, getVel());
    if( ( tvMul < 0.8f or tvMul > 1.2f or tvMul == 0.0f ) and getEnergy() / getMaxEnergy() > 0.1f and getCanMove() ) accelerate(utv, accelMul*angleMul);
    if(fabs(shortestAngle(getAng(),getTAng())) <= 4.0f and dist < 800.0f + target->getRadius() and ( curGoal == GOAL_ATTACK or curGoal == GOAL_TURRET ) and getEnergy() / getMaxEnergy() > 0.05f)
    {
      setFiring(true);
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
