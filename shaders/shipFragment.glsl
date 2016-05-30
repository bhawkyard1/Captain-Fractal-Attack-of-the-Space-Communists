#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file shipFragment.glsl
/// \brief Used to draw ship. Contains a simple lighting setup, which I activate when a ship shoots.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

in vec2 UV;
in vec3 normal;

uniform sampler2D diffuse;
uniform vec4 shootingLightCol;
uniform float alpha;

layout(location = 0) out vec4 fragColour;

void main()
{
    vec3 ambientLightPos = vec3(0.01, 1.0, -1.2);
    vec3 shootingLightPos = vec3(0.0, 0.02, -1.2);

    fragColour = texture( diffuse, UV );

    //Ambient diffuse
    fragColour.rgb *= dot(normal, ambientLightPos) * 0.3;

    //Shooting diffuse
    fragColour += dot(normal, shootingLightPos) * vec4(shootingLightCol.rgb * shootingLightCol.a, shootingLightCol.a) * 0.2;

    //Ambient Spec
    fragColour += pow( vec4(0.3) * dot(normal, reflect(-ambientLightPos, normal)), vec4(10.0));

    //Shooting Spec
    fragColour += pow( shootingLightCol * dot(normal, reflect(-shootingLightPos, normal)), vec4(1.0)) * shootingLightCol.a;

    fragColour.a *= alpha;
}
