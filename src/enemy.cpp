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
    float cSpd = mag(tVel - v);

    //We draw a line, passing through the target, and at 90 degrees to the velocity.
    //We then find the closest point on this line.
    vec2 cp = closest(tPos, normal(v), p);
    vec2 cpm = (cp - tPos) * cSpd;

    vec2 tcPos = tPos + {clamp(cpm.x, tPos.x, cp.x), clamp(cpm.y, tPos.y, cp.y)};

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
    float accelMul = clamp((( tPPDist - stoppingDistance - stopDist - target->getRadius() )),-0.5f, 0.5f)

    //Angle the ship towards its target.
    setTAng(clampRoll(ang( p - tcPos ), -180.0f, 180.0f));
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
}
