#version 410 core

in vec4 fragCoord;
in vec4 inColor;

out vec4 fragColour;

void main()
{
    fragColour = fragCoord;
}
