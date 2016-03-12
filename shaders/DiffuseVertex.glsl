#version 410 core

layout (location = 0) in vec3 inPosition;

uniform vec2 win_dim;

void main()
{
    gl_Position  = vec4(inPosition.xy / win_dim.xy, inPosition.z, 1.0);
}
