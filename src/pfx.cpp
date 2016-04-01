#include "pfx.hpp"
#include <iostream>
#include "util.hpp"
#include "common.hpp"

pfx::pfx(const vec2 _p,
        const vec2 _v,
        const vec2 _wv,
        const size_t _no,
        const float _force,
        const std::string _identifier
         )
{
    m_glowA = clamp(255.0f * (_force * _force) / 100.0f, 0.0f, 255.0f);

    m_active = true;

    m_pos = _p;
    m_vel = _v;

    m_col[0] = randNum(0.0f, 150.0f) + 105.0f;
    m_col[1] = randNum(0.0f, 150.0f) + 105.0f;
    m_col[2] = randNum(0.0f, 150.0f) + 105.0f;

    m_identifier = _identifier;

    for(size_t i = 0; i < _no; i++)
    {
        base particle;
        particle.setPos(_p);
        particle.setPPos(_p);

        particle.setVel( { randFloat(-_force, _force), randFloat(-_force, _force) } );
        particle.setWVel( m_vel + _wv );

        m_particles.push_back(particle);

        m_alphas.push_back(rand()%100+55);
    }
    m_force = _force;
}

void pfx::update(float _dt)
{
    m_pos += (m_vel + m_wvel) * g_PIXEL_UNIT_CONVERSION * _dt;

    bool done = true;

    for(size_t i = 0; i < m_alphas.size(); ++i)
    {
        m_particles[i].setWVel(m_wvel);
        m_particles[i].updatePos( _dt );

        if(m_alphas[i] > 0)
        {
            m_alphas[i] -= 1;
            done = false;
        }
    }

    if(done) m_active = false;
    m_glowA = clamp(m_glowA - 16.0f, 0.0f, 255.0f);
}
