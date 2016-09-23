#ifndef PFX_HPP
#define PFX_HPP

#include <vector>
#include <array>
#include "base.hpp"
#include <string>

#include "SDL2/SDL_image.h"

//----------------------------------------------------------------------------------------------------------------------
/// \file pfx.hpp
/// \brief This class represents a simple particle system.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class pfx
/// \brief Inherits from base, also contains instances of base.
//----------------------------------------------------------------------------------------------------------------------

class pfx : public base
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief ctor for the pfx class
    /// \param _p position
    /// \param _v center velocit y
    /// \param _no number of children
    /// \param _force velocity multiplier of children
    /// \param _identifier type of effect, such as "SMOKE", "EXPLOSION" etc
    //----------------------------------------------------------------------------------------------------------------------
    pfx(const vec3 _p, const vec3 _v, const vec3 _wv, const size_t _no, const float _force, const std::string _identifier, const std::array<float, 4> _col);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the system
    /// \param _dt Time difference
    //----------------------------------------------------------------------------------------------------------------------
    void update(float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Has the effect finished?
    //----------------------------------------------------------------------------------------------------------------------
    bool done() const {return !m_active;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Colour getters
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 4> getCol() const {return m_col;}
    float getCol(int i) const {return m_col[i];}

    float getAlpha(const int _i) {return m_alphas[_i];}
    std::array<float, 4> getShaderData() const {return {m_elapsed, m_seed_position, m_seed_explosion, m_duration};}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Identifier getter
    //----------------------------------------------------------------------------------------------------------------------
    std::string getIdentifier() const {return m_identifier;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns the child particle vector
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<base> * getParticles() {return &m_particles;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Force getter
    //----------------------------------------------------------------------------------------------------------------------
    float getForce() {return m_force;}
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of 'base' (glorified points), representing particles
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<base> m_particles;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The alpha of each of the m_particles
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<int> m_alphas;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Data for the explosion shader
    //----------------------------------------------------------------------------------------------------------------------
    float m_elapsed;
    float m_seed_position;
    float m_seed_explosion;
    float m_duration;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The string id of the particle system
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_identifier;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The colour tint of the particle system
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 4> m_col;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Is the particle system active?
    //----------------------------------------------------------------------------------------------------------------------
    bool m_active;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A multiplier for the velocity of child particles
    //----------------------------------------------------------------------------------------------------------------------
    float m_force;
};

#endif
