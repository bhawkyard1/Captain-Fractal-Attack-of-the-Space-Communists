#version 410 core

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inUV;

out vec2 UV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * inPosition;
    UV = inUV;
}
