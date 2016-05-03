#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file explosionVertex.glsl
/// \brief Takes in position, colour and uv, and transforms appropriately. Used for explosions.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

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
