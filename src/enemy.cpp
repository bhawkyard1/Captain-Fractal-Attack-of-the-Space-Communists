#include "enemy.hpp"

enemy::enemy(
        const vec3 _p ,
        const vec3 _v,
        const ship_spec _type,
        const aiTeam _team
        ):
    ship(g_ship_templates[_type], _p)
{	
    m_curGoal = GOAL_IDLE;
    setPos(_p);
    setPPos(_p);
    setVel(_v);
    m_stopDist = randNum(200.0f,600.0f);
    if(_type == PLAYER_MINER_DROID) m_stopDist = randNum(20.0f, 60.0f);
    m_target = nullptr;
    m_curGoal = GOAL_IDLE;
    m_team = _team;
    m_confidence = randNum(5.0f, 20.0f);
    m_squadID = -1;
    m_tPos = getPos();
}

void enemy::behvrUpdate(float _dt)
{
    if(m_target != nullptr)
    {
        float dist = mag(getPos() - m_target->getPos());
        vec3 diff = (m_target->getPos() - getPos()) / dist;
        m_tPos = m_target->getPos() - diff * fmin(m_target->getRadius(), dist);
        m_tVel = m_target->getVel();
    }

    if(m_curGoal == GOAL_FLEE)
    {
        if(m_target != nullptr)
        {
            m_tPos = getPos() + (getPos() - m_target->getPos()) * 100.0f;
            m_tVel = {0.0f, 0.0f, 0.0f};
        }
        else
        {
            m_tPos = getPos();
            m_tPos.m_y += 10000.0f;
            m_tVel = {0.0f, 0.0f, 0.0f};
        }
    }
    else if(m_curGoal == GOAL_WANDER)
    {
        m_tVel = {0.0f, 0.0f, 0.0f};
        m_tPos += getWVel();
        if(magns(getPos() - m_tPos) < 10000.0f)
        {
            m_tPos += tovec3(randVec2(2000.0f));
        }
    }
    else if(m_curGoal == GOAL_IDLE)
    {
        m_tPos = {randNum(-30000.0f, 30000.0f), randNum(-30000.0f, 30000.0f), 0.0f};
        m_tVel = {0.0f, 0.0f, 0.0f};
        m_curGoal = GOAL_IDLE;
    }
    else if(m_curGoal == GOAL_RETREAT)
    {
        m_tPos = getPos() * F_MAX;
        m_tVel = {0.0f, 0.0f, 0.0f};
    }
}

void enemy::steering()
{
    setFiring(false);

    if(m_curGoal == GOAL_IDLE or m_curGoal == GOAL_SPACE_STATION) return;
    vec3 p = getPos();
    vec3 v = getVel();
    vec3 uv = unit(v);
    vec3 diff = m_tPos - p;
    diff.m_z = 0.0f;
    vec3 utv = unit(diff);

    //This is the distance between the ship and its m_target position.

    float dist = mag(diff);

    float radius = 0.0f;
    if(m_target != nullptr)
    {
        radius = m_target->getRadius();
    }

    /*vec2 goPos = m_tPos;
    if(m_curGoal = GOAL_ATTACK)
    {
        goPos += (vec(m_target->getAng()) * 100);
    }*/

    if(dist < getRadius() and getCanMove()) accelerate(-1.0f);

    //This is the closing speed. Add player and ship vectors, find magnitude.
    //Initially I didn't know whether the vectors were converging or diverging.
    //I solved it by multiplying by the dot of the ship and m_target vectors.
    float cSpd = mag( v - m_tVel ) * dotProd(utv, uv);

    //Whereas angleMul is all about the ships angle in relation to its m_target angle, this is about its vector in relation to its m_target angle.
    //ie where the ship is going vs where is should be going.
    float vecMul = dotProd(uv, utv);

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
    float accelMul = clamp( (dist - stoppingDistance - m_stopDist - radius ) / 200, -1.0f, 0.5f);

    //This varies between 1 (ship facing m_target) 0 (ship parallel to m_target) and -1 (ship facing away from m_target).
    //It does this by taking the ship's angle and its m_target angle, and determining the angle between them.
    //float angleMul = 1.0f;//(static_cast<float>(shortestAngle(getAng(), getTAng())) + 90.0f ) / 90.0f;

    //We need to compare the distance to the closing speed. If the two points are converging rapidly, the ship must slow down.
    //cSpd will be subtracted from dist once per frame. Ships can accelerate at max 1au per frame.
    //Therefore, frames until collision: f = dist/cSpd
    //if dist - factorial(cSpd) < 0, ship will overshoot. > 0, is will undershoot, and = 0, it will arrive at the correct position.

    //vec2 linePos = closest(m_tPos,unit(normal(v)),p);

    //m_tPos -= unit(linePos - m_tPos) * cSpd * 3;

    //Angle the ship towards its m_target.
    setTAng(clampRoll(computeAngle(tovec2(p - (m_tPos + m_tVel))), -180.0f, 180.0f));
    //If we are close to the target, aim towards it.
    //if( dist < 800.0f + radius ) setTAng(clampRoll(computeAngle(p - m_tPos), -180.0f, 180.0f));

    //If we are angled towards the m_target...
    float tvMul = dotProd(m_tVel, v);
    if( ( tvMul < 0.8f or tvMul > 1.2f )
            and ( getEnergy() / getMaxEnergy() > 0.1f or m_curGoal == GOAL_CONGREGATE )
            and getCanMove()
            )
    {
        accelerate(utv, accelMul);
    }

    if(m_curGoal == GOAL_TURRET) dist -= 200.0f;

    float selfRadius = getRadius();
    if(getParent().m_id != -1) selfRadius += 2048.0f;

    if(fabs(shortestAngle(getAng(),getTAng())) <= 2.0f
            and dist < 800.0f + radius + selfRadius
            and ( m_curGoal == GOAL_ATTACK or m_curGoal == GOAL_TURRET )
            and getEnergy() / getMaxEnergy() > 0.05f)
    {
        setFiring(true);

        //Combat moves
        if(getType() == SHIP_TYPE_FIGHTER)
        {
            //Dodge
            if(!(rand()%64) and
                    m_target != nullptr and
                    dotProd(vec(getAng()), vec(m_target->getAng())) < -0.8f
                    )
            {
                dodge(randNum(10.0f, 20.0f) * randNum(-1,1));
            }

            //Charge
            if(!(rand()%128) and
                    m_target != nullptr and
                    m_target->getType() == SHIP_TYPE_FIGHTER
                    )
            {
                accelerate(
                            unit(m_tPos - getPos()),
                            randNum(15.0f, 25.0f)
                            );
            }
        }
    }

    //This variable represents the ships' direction versus its ideal direction.
    float vecMulSide = dotProd(tovec2(uv), vec(getTAng()));

    if(fabs(vecMulSide) > 0.8f)
    {
        //closing speed * how sideways it is flying * its angle relative to velocity
        float dv = clamp(cSpd * vecMulSide * dotProd(tovec2(uv), vec(getAng())), 0.0f, 1.0f);
        if(vecMulSide < 0) dodge( dv );
        else if(vecMulSide > 0) dodge( -dv );
    }
}
