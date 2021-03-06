#version 410 core

//----------------------------------------------------------------------------------------------------------------------
/// \file flameFragment.glsl
/// \brief A flame shader sourced from shadertoy, I adapted and repositioned the elements, added variable speed and recolouring.
/// \author xbe, 2014. Fires. Shadertoy, Available from: https://www.shadertoy.com/view/XsXSWS [Accessed 2 May 2016].
/// Adapted by Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

in vec4 gl_FragCoord;
in vec2 UV;

layout (location = 0) out vec4 fragColour;

uniform float iGlobalTime;
uniform vec4 flameCol;

uniform float speed;

//////////////////////
// Fire Flame shader

// procedural noise from IQ
vec2 hash( vec2 p )
{
    p = vec2( dot(p,vec2(127.1,311.7)),
              dot(p,vec2(269.5,183.3)) );
    return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p )
{
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;

    vec2 i = floor( p + (p.x+p.y)*K1 );

    vec2 a = p - i + (i.x+i.y)*K2;
    vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0);
    vec2 b = a - o + K2;
    vec2 c = a - 1.0 + 2.0*K2;

    vec3 h = max( 0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );

    vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));

    return dot( n, vec3(70.0) );
}

float fbm(vec2 UV)
{
    float f;
    mat2 m = mat2( 1.6,  1.2, -1.2,  1.6 );
    f  = 0.5000*noise( UV ); UV = m*UV;
    f += 0.2500*noise( UV ); UV = m*UV;
    f += 0.1250*noise( UV ); UV = m*UV;
    f += 0.0625*noise( UV ); UV = m*UV;
    f = 0.5 + 0.5*f;
    return f;
}

void main()
{
    vec2 q = UV;
    //q.y = abs(q.y - 1.0);
    q.x *= 2.0;
    q.y *= 2.0;
    float strength = 1.0;
    float T3 = max(3.0, 1.25 * strength) * iGlobalTime * speed * 0.001;
    q.x = mod(q.x, 1.0) - 0.5;
    q.y -= 0.25;
    float n = fbm(strength * q - vec2(0.0, T3));
    float c = 1.0 - 16.0 * pow( max( 0.0, length(q * vec2(1.8 + q.y * 1.5, 0.75) ) - n * max( 0.0, q.y + 0.25 ) ), 1.2 );
    float c1 = n * c * 1.5;
    c1=clamp(c1, 0.0, 1.0);

    vec3 tempFlameCol = flameCol.rgb;
    vec3 col = vec3(pow(c1, 0.25/tempFlameCol.r), pow(c1, 0.25/tempFlameCol.g), pow(c1, 0.25/tempFlameCol.b));

    float a = c * ( 1.0 - pow(UV.y,3.0) );

    fragColour = mix(vec4(col, 0.0),vec4(col, 1.0), a * (1.0 - UV.y));
    fragColour.rgb *= 2.0;
    fragColour.a = clamp(fragColour.a, 0.0, 1.0);
    //fragColour = vec4(1.0, 0.0, 0.0, 1.0);
}
