#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file explosionFragment.glsl
/// \brief Used for the explosion shader, a simple radial gradient.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------


in vec4 gl_FragCoord;
in vec2 UV;
in vec4 vertCol;

//uniform vec4 inColour;

layout (location = 1) out vec4 fragColour;

void main()
{    
    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(vertCol.rgb, 0.0), dist * 2.0);
    fragColour *= 1.5;
    fragColour.a *= vertCol.a;
}
