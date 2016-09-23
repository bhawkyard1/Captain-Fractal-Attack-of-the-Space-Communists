#version 430 core

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
layout(location = 3) in vec4 inSeed;

out vec4 vertCol;
out vec2 UV;

/*out float iGlobalTime;
out float variation_seed;
out float explosion_seed;
out float speed;*/

out float elapsed_time;
out float variation_seed;
out float explosion_seed;
out float duration;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(inPosition.xyz, 1.0);
    vertCol = inColour;
    UV = inUV;

    /*iGlobalTime = inSeed.x;
    variation_seed = inSeed.y;
    explosion_seed = inSeed.z;
    speed = clamp(12.0 / inSeed.w, 0.4, 0.8);*/
    elapsed_time = inSeed.x;
    variation_seed = inSeed.y;
    explosion_seed = inSeed.z;
    duration = inSeed.w;
}
