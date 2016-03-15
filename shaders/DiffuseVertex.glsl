#version 410 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColor;

out vec4 outColor;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * inPosition;
    //gl_Position = inPosition * MVP;
    //gl_Position = vec4(inPosition.x / 960.0, inPosition.y + 600.0, inPosition.zw);
}
