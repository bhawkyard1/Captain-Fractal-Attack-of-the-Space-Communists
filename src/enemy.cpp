#include "enemy.hpp"

enemy::enemy(
        const vec2 _p ,
        const vec2 _v,
        const ship_spec _type,
        const aiTeam _team
        ):
        ship(_type)
{	
    m_curGoal = GOAL_IDLE;
    setPos(_p);
    setVel(_v);
    m_stopDist = randFloat(200.0f,600.0f);
    if(_type == PLAYER_MINER_DROID) m_stopDist = randFloat(20.0f, 60.0f);
    m_target = nullptr;
    m_curGoal = GOAL_IDLE;
    m_team = _team;
    m_confidence = randFloat(5.0f, 20.0f);
    m_squadID = -1;
}

void enemy::behvrUpdate()
{
    if(m_target != nullptr)
    {
        m_tPos = m_target->getPos();
        m_tVel = m_target->getVel();
    }

    if(m_curGoal == GOAL_FLEE)
    {
        if(m_target != nullptr)
        {
            m_tPos = getPos() + (getPos() - m_target->getPos()) * 100.0f;
            m_tVel = {0.0f, 0.0f};
        }
        else
        {
            m_tPos = getPos();
            m_tPos.m_y += 10000.0f;
            m_tVel = {0.0f, 0.0f};
        }
    }
    else if(m_curGoal == GOAL_IDLE)
    {
        m_tPos = {randFloat(-30000.0f, 30000.0f), randFloat(-30000.0f, 30000.0f)};
        m_tVel = {0.0f, 0.0f};
        m_curGoal = GOAL_IDLE;
    }
}

void enemy::steering()
{
    setFiring(false);

    if(g_GAME_OVER or m_curGoal == GOAL_IDLE or m_curGoal == GOAL_SPACE_STATION) return;
    vec2 p = getPos();
    vec2 v = getVel();
    vec2 uv = unit(v);
    vec2 utv = unit(m_tPos - p);


    //This is the distance between the ship and its m_target position.
    float dist = mag( p - m_tPos );
    float radius = 0.0f;
    if(m_target != nullptr) radius = m_target->getRadius();

    if(dist < getRadius() and getCanMove()) accelerate(-1.0f);

    //This is the closing speed. Add player and ship vectors, find magnitude.
    //Initially I didn't know whether the vectors were converging or diverging.
    //I solved it by multiplying by the dot of the ship and m_target vectors.
    float cSpd = mag( v - m_tVel ) * dotProd1(utv, uv);

    //Whereas angleMul is all about the ships angle in relation to its m_target angle, this is about its vector in relation to its m_target angle.
    //ie where the ship is going vs where is should be going.
    float vecMul = dotProd1(uv, utv);

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
    float stoppingDistance = 4 * ( cSpd * cSpd );

    if(vecMul < 0.0f) stoppingDistance *= -1;

    //This controls how much the ship is to accelerate. It depends on the closing speed between the ship and its m_target, their distance apart, and whether the ship is moving towards the m_target, or away.
    float accelMul = clamp( (dist - stoppingDistance - m_stopDist - radius ) / 200,-1.0f, 0.5f);

    //This varies between 1 (ship facing m_target) 0 (ship parallel to m_target) and -1 (ship facing away from m_target).
    //It does this by taking the ship's angle and its m_target angle, and determining the angle between them.
    float angleMul = 1.0f;//(static_cast<float>(shortestAngle(getAng(), getTAng())) + 90.0f ) / 90.0f;

    //We need to compare the distance to the closing speed. If the two points are converging rapidly, the ship must slow down.
    //cSpd will be subtracted from dist once per frame. Ships can accelerate at max 1au per frame.
    //Therefore, frames until collision: f = dist/cSpd
    //if dist - factorial(cSpd) < 0, ship will overshoot. > 0, is will undershoot, and = 0, it will arrive at the correct position.

    //vec2 linePos = closest(m_tPos,unit(normal(v)),p);

    //m_tPos -= unit(linePos - m_tPos) * cSpd * 3;

    //Angle the ship towards its m_target.
    setTAng(clampRoll(computeAngle(p-m_tPos), -180.0f, 180.0f));
    //If we are angled towards the m_target...

    float tvMul = dotProd1(m_tVel, v);
    if( ( tvMul < 0.8f or tvMul > 1.2f )
            and ( getEnergy() / getMaxEnergy() > 0.1f or m_curGoal == GOAL_CONGREGATE )
            and getCanMove()
            )
    {
        accelerate(utv, accelMul*angleMul);
    }

    if(fabs(shortestAngle(getAng(),getTAng())) <= 4.0f
            and dist < 800.0f + radius
            and ( m_curGoal == GOAL_ATTACK or m_curGoal == GOAL_TURRET )
            and getEnergy() / getMaxEnergy() > 0.05f)
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
