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
    debris(const vec3 _pos, const vec3 _vel, const vec3 _wvel, const resourceType _type);

    void setAng(const float _angle) {m_angle = _angle;}
    float getAng() const {return m_angle;}

    std::string getIdentifier() const {return m_identifier;}

    void setAngleVel(const float _angleVel) {m_angleVel = _angleVel;}
    float getAngleVel() const {return m_angleVel;}

    float getInertia() const {return m_invMass;}

    float getRadius() const {return m_radius;}
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
