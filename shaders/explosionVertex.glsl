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
layout(location = 2) in vec3 inScale;
layout(location = 3) in float inAngle;
layout(location = 4) in vec4 inSeed;

out vec4 vertCol;
out vec2 geoScale;
out float geoAngle;
out vec4 seed;

//uniform mat4 MVP;

void main()
{
    /*gl_Position = vec4(inPosition.xyz, 1.0);
    vertCol = inColour;

    geoScale = inScale.xy;
    geoScale = vec2(20.0, 20.0f);
    geoAngle = 0.0f;

    seed = inSeed;*/

    gl_Position = vec4(inPosition.xyz, 1.0);
    vertCol = inColour;

    geoScale = inScale.xy;
    geoScale = vec2(20.0, 20.0f);
    geoAngle = 0.0f;

    seed = inSeed;
}
