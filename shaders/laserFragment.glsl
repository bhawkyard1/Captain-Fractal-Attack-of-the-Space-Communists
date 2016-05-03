#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file laserFragment.glsl
/// \brief Produces a laser, essentially a gradient from vertColour to white and back.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------


in vec4 gl_FragCoord;
in vec4 vertColour;
in vec2 UV;

out vec4 fragColour;

void main()
{
    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(vertColour.rgb, 0.0), dist * 2.0);
    fragColour *= 1.0;
    fragColour.a *= vertColour.a;
}
