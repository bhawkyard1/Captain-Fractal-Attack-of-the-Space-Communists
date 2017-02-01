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
in vec4 position;

layout (location = 0) out vec4 fragColour;

uniform vec4 origin;
uniform vec4 colour;
uniform float power;
uniform float lifetime;

void main()
{
    vec4 col = colour;
    col.rgb *= power;
    col.a *= dot(normal, vec3(0.0,0.0,1.0));

    fragColour = mix( col, vec4(0.5), distance(origin.xyz, position.xyz) / power );

    fragColour.a = mix(fragColour.a, 0.0, lifetime);

    fragColour.a = clamp(fragColour.a, 0.0, 1.0);
}
