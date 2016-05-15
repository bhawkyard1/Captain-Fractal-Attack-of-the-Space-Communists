#ifndef DEBRIS_HPP
#define DEBRIS_HPP

#include <string>
#include <vector>

#include "base.hpp"

enum resourceType {
    RESOURCE_IRON, RESOURCE_SILVER, RESOURCE_SILICON, RESOURCE_GOLD
};

extern std::vector< std::pair< std::string, float > > g_resource_keys;

class debris : public base
{
public:
    debris(const vec2 _pos, const resourceType _type);

    void setAngleVel(const float _angleVel) {m_angleVel = _angleVel;}
    float getAngleVel() {return m_angleVel;}

    float getInertia() {return m_invMass;}

    float getRadius() {return m_radius;}
private:
    float m_radius;
    resourceType m_type;
    std::string m_identifier;
    int m_baseValue;
    float m_angle;
    float m_angleVel;
    float m_invMass;
};

#endif
