#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file MVPUVVert.glsl
/// \brief Used mostly for models, this shader takes position and and transforms appropriately.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

layout (location = 0) in vec4 inPosition;
layout (location = 2) in vec2 inUV;

uniform mat4 MVP;

out vec2 UV;

void main()
{
    UV = inUV;
    gl_Position = MVP * inPosition;
}
