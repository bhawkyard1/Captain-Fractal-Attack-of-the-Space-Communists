#version 430 core

//----------------------------------------------------------------------------------------------------------------------
/// \file debugFragment.glsl
/// \brief Takes position and colour, transforms appropriately.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColour;

out vec4 vertColour;

uniform mat4 MVP;

void main()
{
    vertColour = inColour;
    gl_Position = MVP * inPosition;  
}
