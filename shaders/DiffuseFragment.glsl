#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file DiffuseFragment.glsl
/// \brief Similar to debugFragment, but it takes its colour from the vertex colour rather than having it input via uniform.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

in vec4 fragCoord;
in vec4 vertColour;

out vec4 fragColour;

void main()
{
    fragColour = vertColour;
}
