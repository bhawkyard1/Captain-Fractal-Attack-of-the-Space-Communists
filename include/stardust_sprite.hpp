#ifndef STARDUST_SPRITE_HPP
#define STARDUST_SPRITE_HPP

#include <array>
#include <string>
#include "stardust.hpp"

class stardust_sprite: public stardust
{
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Texture key of the sprite.
    //----------------------------------------------------------------------------------------------------------------------
    std::string m_identifier;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The angle of the sprite.
    //----------------------------------------------------------------------------------------------------------------------
    float m_ang;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The angular velocity of the sprite.
    //----------------------------------------------------------------------------------------------------------------------
    float m_angVel;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The dimensions.
    //----------------------------------------------------------------------------------------------------------------------
    float m_dim;
public:
    stardust_sprite(const std::string _identifier, const std::array<float, 3> _col, const int _w, const int _h);
    stardust_sprite(const std::string _identifier, const float _alph, const int _w, const int _h);
    void spriteGen(const std::array<float, 3> &_col, const int _w, const int _h);
    std::string getTex() const {return m_identifier;}
    void updateSprite(const float _dt);
    void incrDim(float _dt);
    float getDim() const {return m_dim;}
    float getAng() const {return m_ang;}
    std::string getIdentifier() {return m_identifier;}
};

#endif
