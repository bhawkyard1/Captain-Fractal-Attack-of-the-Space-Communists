#ifndef SPRITESHEETOPENGL_HPP
#define SPRITESHEETOPENGL_HPP

#include <vector>
#include <ngl/NGLInit.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file sprite_sheet.hpp
/// @brief This file contains the spriteSheet class, which is used with the NGL renderer. It's SDL
/// counterpart is found in sprite_sheet.hpp.
/// @author Ben Hawkyard
/// @version 1.0
/// @date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// @class spriteSheet
/// @brief Stores a bunch of opengl textures, and their dimensions. Used mainly to draw text.
//----------------------------------------------------------------------------------------------------------------------

struct spriteSheet
{
    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Container of dimensions, matching indexes with texture vector.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<std::pair<int, int>> m_dim;

    //----------------------------------------------------------------------------------------------------------------------
    /// @brief Storage for opengl texture IDs.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<GLuint> m_sheet;
};


#endif
