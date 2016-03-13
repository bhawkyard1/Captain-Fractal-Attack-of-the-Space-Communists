#version 410 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec4 inColor;

out vec4 outColor;

uniform vec2 iResolution;
uniform float zoom;

void main()
{
    vec2 tResolution = iResolution * zoom;
    gl_Position  = vec4(inPosition.x / tResolution.x, -inPosition.y / tResolution.y, inPosition.z, 1.0);
}
