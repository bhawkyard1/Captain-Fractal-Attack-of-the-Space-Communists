#version 410 core

in vec2 UV;
in vec3 normal;

uniform sampler2D diffuse;
uniform vec4 shootingLightCol;

layout(location = 0) out vec4 fragColour;

void main()
{
    vec3 ambientLightPos = vec3(0.01, 1.0, -1.2);
    vec3 shootingLightPos = vec3(0.0, 0.02, -1.2);

    //vec4 shootingLightCol = vec4(1.0, 0.0, 0.0, 1.0);

    fragColour = texture( diffuse, UV );
    fragColour += dot(normal, shootingLightPos) * vec4(shootingLightCol.rgb * shootingLightCol.a, shootingLightCol.a);
    fragColour.rgb *= dot(normal, ambientLightPos) * 0.6 + 0.1;
}
