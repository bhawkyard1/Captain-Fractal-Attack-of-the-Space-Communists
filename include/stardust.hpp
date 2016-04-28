#ifndef STARDUST_HPP
#define STARDUST_HPP

#include <array>

#include "base.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// @file stardust.hpp
/// @brief Only relevant to the SDL Renderer, used to draw bits of stardust on the screen. This
/// is unneeded in the opengl version, since the background shader replaces all of this.
/// @author Ben Hawkyard
/// @version 1.0
/// @date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// @class stardust
/// @brief Legacy class, used in the SDL renderer only (although a descendant is used by the opengl
/// renderer).
//----------------------------------------------------------------------------------------------------------------------

class stardust: public base
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for the stardust class.
    /// @param _col The colour for the stardust.
    //----------------------------------------------------------------------------------------------------------------------
    stardust(const std::array<float, 3> &_col);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief ctor for the stardust class.
    /// @param _alpha Alpha value of the stardust.
    //----------------------------------------------------------------------------------------------------------------------
    stardust(float _alpha);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Rather than deleting/creating stardust when it goes of screen, we call gen() which
    /// randomises its member data.
    /// @param _regen Whether the stardust is regenerating or if this is the initial generation
    /// _col Colour of the stardust.
    //----------------------------------------------------------------------------------------------------------------------
    void gen(bool _regen, const std::array<float, 3> &_col);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief This is a slightly altered updatePos function. Since SDL is 2d, the position increment is
    /// based in part off of the z depth.
    /// @param _dt Time difference.
    //----------------------------------------------------------------------------------------------------------------------
    void updatePos(float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Setter and getter for z-depth.
    //----------------------------------------------------------------------------------------------------------------------
    void setZ(float _z) {addPos({0.0f, 0.0f, _z});}
    float getZ() {return getPos().m_z;}

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Getter and setter for the colour.
    //----------------------------------------------------------------------------------------------------------------------
    float getCol(int _i) {return m_col[_i];}
    void setCol(int _i, float _v) {m_col[_i] = _v;}

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief The colour of the stardust.
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 4> m_col;
};

#endif
