#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file MVPUVNVert.glsl
/// \brief An extremely basic shader, this takes position and transforms appropriately.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

layout (location = 0) in vec4 inPosition;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * inPosition;
}
