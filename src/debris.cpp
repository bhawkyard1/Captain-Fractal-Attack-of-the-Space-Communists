#include "debris.hpp"
#include "util.hpp"

debris::debris(const vec3 _pos, const vec3 _vel, const vec3 _wvel, const resourceType _type)
{
    setPos(_pos);
    setPPos(_pos);
    setVel(_vel);
    setWVel(_wvel);

    m_angle = randNum(0.0f, 360.0f);
    m_angleVel = randNum(-1.0f, 1.0f);

    m_radius = 16.0f;

    switch(_type)
    {
    case RESOURCE_IRON:
        m_identifier = "RESOURCE_IRON_ROCK";
        m_baseValue = 3;
        break;
    case RESOURCE_SILVER:
        break;
    case RESOURCE_SILICON:
        break;
    case RESOURCE_GOLD:
        break;
    default:
        break;
    }
}
