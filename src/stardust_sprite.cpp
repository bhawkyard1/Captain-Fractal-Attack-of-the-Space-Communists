#include "sprite.hpp"
#include "util.hpp"
#include "common.hpp"

sprite::sprite(
        const std::string identifier,
        const vec3 _col,
        const int _w,
        const int _h
        )
{	
    m_identifier = identifier;

    m_dim = std::max(_w, _h) / 2;
    m_ang = randNum(0.0f,360.0f);
    m_angVel = randNum(-0.12f, 0.12f);

    vec3 min = vec3(-_w, -_h, 0.0f);
    min -= tovec3(g_HALFWIN);
    vec3 max = vec3(_w, _h, 0.0f);
    max += tovec3(g_HALFWIN);

    setPos( randVec3(min, max) * g_BG_DENSITY );

    m_col = _col;
}

sprite::sprite(
        const std::string _identifier,
        const float _alph,
        const int _w,
        const int _h
        )
{	
    m_identifier = _identifier;

    m_dim = std::max(_w, _h) / 2;

    m_ang = randNum(0.0f,360.0f);
    m_angVel = randNum(-0.12f, 0.12f);

    m_alph = _alph;
}

void sprite::updateSprite(const float _dt)
{
    m_ang += m_angVel;
    updatePos(_dt);
}

void sprite::incrDim(float _dt)
{
    if(m_identifier == "SMOKE")
    {
        m_dim += 32.0f * _dt;
    }
    else if(m_identifier == "EXPLOSION")
    {
        m_dim -= 32.0f * _dt;
    }
}
