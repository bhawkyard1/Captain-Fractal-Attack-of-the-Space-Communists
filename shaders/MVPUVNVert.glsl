#version 430 core

//----------------------------------------------------------------------------------------------------------------------
/// \file MVPUVNVert.glsl
/// \brief Used mostly for models, this shader takes position, uv and normal and transforms appropriately.
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
