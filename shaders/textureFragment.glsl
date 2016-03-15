#version 410 core

in vec2 UV;
in vec3 normal;

uniform sampler2D diffuse;

layout(location = 0) out vec4 fragColour;

void main()
{
    fragColour = texture( diffuse, UV );
}
