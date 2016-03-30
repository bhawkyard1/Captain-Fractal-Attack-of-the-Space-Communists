#version 410 core

in vec4 fragCoord;
uniform vec4 inColour;

out vec4 fragColour;

void main()
{
    fragColour = inColour;
}
