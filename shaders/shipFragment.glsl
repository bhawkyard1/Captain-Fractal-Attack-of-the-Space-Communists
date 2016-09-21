#version 430 core

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
in vec4 position;

uniform sampler2D diffuse;
uniform float alpha;

layout(location = 0) out vec4 fragColour;
layout(location = 1) out vec4 fragNormal;
layout(location = 2) out vec4 fragPosition;

void main()
{
    fragColour = texture( diffuse, UV );
    fragNormal = vec4(normalize(normal), 1.0);
    fragPosition = position;
    //fragPosition = gl_FragCoord;

    //Ambient diffuse
    /*fragColour.rgb *= dot(normal, ambientLightPos) * 0.5;

    //Shooting diffuse
    fragColour += dot(normal, shootingLightPos) * vec4(shootingLightCol.rgb * shootingLightCol.a, shootingLightCol.a) * 0.2;

    //Ambient Spec
    fragColour += pow( vec4(0.5) * dot(normal, reflect(-ambientLightPos, normal)), vec4(10.0));

    //Shooting Spec
    fragColour += pow( shootingLightCol * dot(normal, reflect(-shootingLightPos, normal)), vec4(1.0)) * shootingLightCol.a;

    fragColour.a *= alpha;*/
}
