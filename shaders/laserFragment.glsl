#version 410 core

in vec4 fragCoord;
in vec4 vertColour;

out vec4 fragColour;

void main()
{
    fragColour = vertColour;
    //fragColour = vec4(1.0);
}
