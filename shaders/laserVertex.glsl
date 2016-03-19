#version 410 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColour;

out vec4 vertColour;

uniform mat4 MVP;

void main()
{
    vertColour = inColour;
    gl_Position = MVP * inPosition;
}
