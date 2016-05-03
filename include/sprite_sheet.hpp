#ifndef SPRITE_SHEET_HPP
#define SPRITE_SHEET_HPP

#include <vector>
#include <string>
#include "SDL.h"

//----------------------------------------------------------------------------------------------------------------------
/// \file sprite_sheet.hpp
/// \brief This file contains the sprite_sheet class, which is used with the SDL renderer. It's opengl
/// counterpart is found in sprite_sheet_opengl.hpp.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class sprite_sheet
/// \brief Stores a bunch of SDL_Textures, and the functionality to destroy them. The functionality
/// to fill them is actually stores in the renderer class, although I suppose I could move it here. Used
/// mainly to draw text.
//----------------------------------------------------------------------------------------------------------------------

struct sprite_sheet
{
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Storage for the sdl textures.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<SDL_Texture *>  m_sheet;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Destroys the textures in m_sheet.
    //----------------------------------------------------------------------------------------------------------------------
    void destroy();
};

#endif
