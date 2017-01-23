#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file backgroundVertex.glsl
/// \brief An extremely simple vertex shader, does not even apply perspective transformation.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------


layout (location = 0) in vec3 inPosition;
layout (location = 2) in vec2 inUV;

out vec2 UV;

void main()
{
    UV = inUV;
    gl_Position = vec4(inPosition, 1.0);
}
