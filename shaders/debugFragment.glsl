#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file debugFragment.glsl
/// \brief A simple debug fragment shader, I used it early in development.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------


in vec4 fragCoord;
uniform vec4 inColour;

layout (location = 0) out vec4 fragColour;

void main()
{
    fragColour = inColour;
}
