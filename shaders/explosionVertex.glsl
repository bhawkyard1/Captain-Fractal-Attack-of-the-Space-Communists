#version 410 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColour;
layout(location = 2) in vec2 inUV;

out vec4 vertCol;
out vec2 UV;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(inPosition.xyz, 1.0);
    vertCol = inColour;
    UV = inUV;
}
