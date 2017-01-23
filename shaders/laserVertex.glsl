#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file laserVertex.glsl
/// \brief Takes in position, colour and uv, and transforms appropriately. Used for lasers.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inColour;
layout (location = 2) in vec2 inScale;

out vec4 geoColour;
out vec2 geoScale;

void main()
{
    geoColour = inColour;
    geoScale = inScale;
    gl_Position = inPosition;
}
