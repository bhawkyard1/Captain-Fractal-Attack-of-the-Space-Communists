#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "vectors.hpp"

struct pointLight
{
    pointLight(vec3 _p, vec3 _c, float _l)
    {
        m_pos = _p;
        m_col = _c;
        m_brightness = _l;
    }

    vec3 m_pos;
    vec3 m_col;
    float m_brightness;
};

#endif
