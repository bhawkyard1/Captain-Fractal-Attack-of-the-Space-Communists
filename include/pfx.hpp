#ifndef PFX_HPP
#define PFX_HPP

#include <vector>
#include <array>
#include "base.hpp"
#include <string>

#include "SDL2/SDL_image.h"

class pfx
{
    std::vector<base> m_particles;
    std::vector<int> m_alphas;
    float m_glowA;
    std::string m_identifier;
    std::array<float, 3> m_col;
    vec2 m_pos;
    vec2 m_vel;
    vec2 m_wvel;
    bool m_active;
    float m_force;
public:
    pfx(const vec2 _p, const vec2 _v, const vec2 _wv, const size_t _no, const float _force, const std::string _identifier);
    void update(float _dt);
    bool done() const {return !m_active;}
    void setWVel(vec2 _v) {m_wvel = _v;}
    std::array<float, 3> getCol() const {return m_col;}
    float getCol(int i) const {return m_col[i];}
    float getAlpha() const {return m_glowA;}
    float getAlpha(const int index) const {return m_alphas.at(index);}
    std::string getIdentifier() const {return m_identifier;}
    std::vector<base> * getParticles() {return &m_particles;}
    vec2 getPos() const {return m_pos;}
    float getForce() {return m_force;}
};

#endif
