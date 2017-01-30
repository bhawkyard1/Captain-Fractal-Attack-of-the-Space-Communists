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

in vec2 UV;
in vec3 normal;

layout (location = 0) out vec4 fragColour;

uniform vec4 colour;
uniform float power;

void main()
{
    fragColour = colour;
    fragColour.rgb *= power;
    fragColour.a *= dot(normal, vec3(0.0,0.0,1.0));
}
