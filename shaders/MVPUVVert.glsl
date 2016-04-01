#version 410 core

layout (location = 0) in vec4 inPosition;
layout (location = 2) in vec2 inUV;

uniform mat4 MVP;

out vec2 UV;

void main()
{
    UV = inUV;
    gl_Position = MVP * inPosition;
}
