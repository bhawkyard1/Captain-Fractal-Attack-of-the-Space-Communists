#version 430 core

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
in vec4 vertColour;

//uniform vec4 inColour;

layout (location = 0) out vec4 fragColour;

void main()
{    
    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(vertColour.rgb, 0.0), dist * 2.0);
    fragColour *= 1.25;
    fragColour = pow(fragColour, vec4(1.0 - vertColour.a));
    //fragColour.a *= vertColour.a;
    //fragColour.a = 1.0;
    //fragColour = vec4(1.0, 1.0, 1.0, 1.0);
}
