#include "pfx.hpp"
#include <iostream>
#include "util.hpp"
#include "common.hpp"

pfx::pfx(
        const vec3 _p,
        const vec3 _v,
        const vec3 _wv,
        const size_t _no,
        const float _force,
        const std::string _identifier,
        const std::array<float, 4> _col
        )
{
    m_elapsed = 0.0f;
    m_seed_position = static_cast<float>(rand());
    m_seed_explosion = static_cast<float>(rand());

    m_duration = _force / 12.0f;

    m_active = true;

    setPos(_p);
    setVel(_v);
    setWVel(_wv);

    m_col = _col;

    m_identifier = _identifier;

    for(size_t i = 0; i < _no; i++)
    {
        base particle;
        particle.setPos(_p);
        particle.setPPos(_p);

        particle.setVel( randVec3(_force) + _v );
        particle.setWVel( _wv );

        m_particles.push_back(particle);

        m_alphas.push_back(rand()%100+55);
    }
    m_force = _force;
}

void pfx::update(float _dt)
{
    bool done = true;

    for(auto &i : m_particles)
    {
        i.setWVel( getWVel() );
    }

    for(size_t i = 0; i < m_alphas.size(); ++i)
    {
        m_particles[i].setWVel( getWVel() );
        m_particles[i].updatePos( _dt );

        if(m_alphas[i] > 0.0f)
        {
            m_alphas[i] -= 1.0f;
            done = false;
        }
    }

    if(m_elapsed < m_duration) done = false;

    if(done) m_active = false;
    m_elapsed += _dt;

    //m_col[3] *= 0.995f;

    updatePos(_dt);
}
