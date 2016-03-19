#version 410 core

layout (location = 0) in vec4 inPosition;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * inPosition;
}
