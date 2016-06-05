#include "camera.hpp"
#include "common.hpp"
#include "util.hpp"

camera::camera()
{
    setPos( vec3() );
    setPPos( vec3() );
    setVel( vec3() );
    setWVel( vec3() );

    m_cameraShake = 0.0f;
    m_cameraShakeOffset = vec2();
    m_cameraShakeTargetOffset = vec2();
    m_tPos = vec3();
}

void camera::update(float _dt)
{
    vec3 diff = m_tPos - getPos();
    //addPos( diff / 5.0f );
    setPos(m_tPos);

    m_cameraShake = clamp(m_cameraShake - _dt * 5.0f, 0.0f, 20.0f);

    //m_cameraShake = 15.0f;

    if(magns(m_cameraShakeTargetOffset - m_cameraShakeOffset) < 80.0f)
    {
        m_cameraShakeTargetOffset = randVec2(m_cameraShake);
    }
    m_cameraShakeOffset += (m_cameraShakeTargetOffset - m_cameraShakeOffset) * clamp(_dt * 5.0f, 0.0f, 1.0f);

    g_ZOOM_LEVEL +=  m_cameraShake * 0.00003f;
}
