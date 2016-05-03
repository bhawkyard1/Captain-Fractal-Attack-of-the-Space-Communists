#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file shipVertex.glsl
/// \brief Transforms the vertices of a ship onto the screen.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

layout(location = 0) in vec4 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;

out vec2 UV;
out vec3 normal;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * inPosition;
    UV = inUV;
    normal = inNormal;
}
