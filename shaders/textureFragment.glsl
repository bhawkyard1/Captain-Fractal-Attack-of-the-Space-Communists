#version 410 core

in vec2 UV;

uniform sampler2D diffuse;
uniform vec4 inColour;

layout(location = 0) out vec4 fragColour;

void main()
{
    fragColour = texture( diffuse, UV );
    fragColour *= inColour;
}
