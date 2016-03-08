#version 410 core

in vec3 fragmentNormal;
out vec4 fragmentColour;
uniform vec4 colour;
uniform vec3 lightPos;
uniform vec4 lightDiffuse;

void main()
{
    fragmentColour = vec4(0.0);
    vec3 N = normalize(fragmentNormal);
    vec3 L = normalize(lightPos);
    fragmentColour += colour * lightDiffuse * dot(L,N);
}
