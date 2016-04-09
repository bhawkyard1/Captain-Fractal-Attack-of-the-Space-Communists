#version 410 core

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec2 inUV;

out vec2 fstart;
out vec2 fend;

out vec4 vertColour;
out vec2 UV;

uniform mat4 MVP;
uniform float zoom;
uniform vec2 shake;
uniform vec2 halfwin;

void main()
{
    vertColour = inColour;
    UV = inUV;
    gl_Position = MVP * inPosition;
}
