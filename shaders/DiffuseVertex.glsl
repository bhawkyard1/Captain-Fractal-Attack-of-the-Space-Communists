#version 410 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColor;

out vec4 outColor;

uniform mat4 MVP;

void main()
{
    gl_Position = inPosition * MVP;
    //gl_Position = vec4((inPosition.x + 960.0)/ 1920.0, (inPosition.y + 600.0) / 1200.0, inPosition.zw);
    //gl_Position = inPosition;
    //float frac = 1920.0 / 1200.0;
    //gl_Position.xy += 1.0;
}
