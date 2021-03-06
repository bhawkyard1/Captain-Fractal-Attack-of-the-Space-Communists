#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file shieldFragment.glsl
/// \brief Draws a shield effect. Sourced from shadertoy. I added colour changing functionality.
/// \author lusius, 2013. spaceshooter-shield. Shadertoy, Available from: https://www.shadertoy.com/view/ld2Gzz [Accessed 2 May 2016].
/// Adapted by Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

in vec4 fragCoord;
in vec2 UV;
in vec3 normal;

uniform float iGlobalTime;
uniform vec4 inColour;

layout (location = 0) out vec4 fragColour;

void main()
{
    float t = iGlobalTime * 256.0;
    vec2 p = UV - 1.0;
    vec2 op = p;
    vec4 colour = inColour;
    colour += vec4(0.25);
    float v = length(2.0 * UV - 1.0);

    p *= 10.0;

    float movex = sin(t + p.x + p.y * v);
    float movey = sin(t * 0.5);
    float grid = sin(p.x + movex) * sin(p.y + movey);
    float grid2 = grid + 0.2;
    float grid3 = grid + 0.3;

    float inner = 1.0 - ceil(grid + grid2);
    float outer = 1.0 - ceil(grid + grid3);

    inner = max(0.0, min(1.0,inner));
    outer = max(0.0, min(1.0,outer));
    float stencil = inner-outer;

    colour.a = inner - outer;
    p *= 0.5;
    p = abs(p);
    p *= v;
    p.x += sin(p.y * sin(p.x));
    float beam = sin(p.x + t);
    beam = max(0.0, min(1.0, beam));

    colour.a += beam;
    colour.a *= beam*grid;
    colour *= 2.0;
    colour.a += stencil * beam;
    //colour.a = max(0.0, min(1.0, colour.a));
    //colour.a += 0.3;


    // high constrast and vinj
    colour *= colour * 3.0;
    colour *= 1.0 - v;

    colour.rgb = clamp(colour.rgb, vec3(0.0), vec3(1.0));

    //output final colour
    fragColour = colour;

    float dist = distance(UV, vec2(0.5)) * 2.0;
    fragColour.a = mix(0.0, fragColour.a, dist);
    fragColour.a *= inColour.a;
    fragColour.a = clamp(fragColour.a, 0.0, 1.0);
}
