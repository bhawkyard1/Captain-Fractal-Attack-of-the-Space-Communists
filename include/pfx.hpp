#ifndef PFX_HPP
#define PFX_HPP

#include <vector>
#include <array>
#include "base.hpp"
#include <string>

#include "SDL2/SDL_image.h"

class pfx : public base
{
public:
    pfx(const vec3 _p, const vec3 _v, const vec3 _wv, const size_t _no, const float _force, const std::string _identifier);
    void update(float _dt);
    bool done() const {return !m_active;}
    std::array<float, 3> getCol() const {return m_col;}
    float getCol(int i) const {return m_col[i];}
    float getAlpha() const {return m_glowA;}
    float getAlpha(const int index) const {return m_alphas.at(index);}
    std::string getIdentifier() const {return m_identifier;}
    std::vector<base> * getParticles() {return &m_particles;}
    float getForce() {return m_force;}
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief vector of 'base' (glorified points), representing particles
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<base> m_particles;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the alpha of each of the m_particles
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<int> m_alphas;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the alpha of the systems central sprite
    //----------------------------------------------------------------------------------------------------------------------
    float m_glowA;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the string id of the particle system
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_identifier;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief the colour tint of the particle system
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 3> m_col;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief is the particle system active?
    //----------------------------------------------------------------------------------------------------------------------
    bool m_active;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief is the particle system active?
    //----------------------------------------------------------------------------------------------------------------------
    float m_force;
};

#endif
