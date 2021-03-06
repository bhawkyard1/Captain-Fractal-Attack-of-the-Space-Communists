#include "missile.hpp"

missile::missile(
        const vec3 _p
        )
        :
        ship(g_ship_templates[ION_MISSILE_MKI], _p)
{
    setPos(_p);
    setMaxHealth(10.0f, true);
    setMaxShield(5.0f, true);
    setMaxEnergy(10000.0f, true);
    setTAng(0.0f);
    m_det = false;
}

void missile::steering()
{
    if(m_target != nullptr)
    {
        vec3 m_tPos = m_target->getPos();

        float dist = mag(m_tPos - getPos());

        float cSpd = mag(getVel() - m_target->getVel());

        float frames = dist/cSpd;

        vec3 tPPos = m_target->getPos() + m_target->getVel() * frames;
        float tPPDist = mag(tPPos - getPos());

        float vecMul = dot(unit(getVel()),tovec3(vec(getTAng() + 90)));

        float stoppingDistance = (cSpd*cSpd)/2;
        if(vecMul < 0) stoppingDistance *= -1;

        float accelMul = clamp(((tPPDist-stoppingDistance)),-3.0f,1.5f);

        float angleMul = (shortestAngle(getAngle().getPitch(), getTAng())+90)/90.0;

        setTAng(clampRoll( ang(tovec2(getPos() - m_tPos) ),-180.0f,180.0f));

        if(fabs(shortestAngle(getAngle().getPitch(), getTAng())) <= 2.0f)
        {
            accelerate(accelMul*angleMul);
        }

        if(magns(getPos() - m_target->getPos()) < 50*50*cSpd)
        {
            m_det = true;
        }
    }
    else
    {
        setTAng(getAngle().getPitch());
    }
}
