#version 430 core

#define SMOOTHNESS 0.04

in vec4 gl_FragCoord;
in vec2 UV;
in vec4 vertCol;

in float elapsed_time;
in float variation_seed;
in float explosion_seed;
in float duration;

layout (location = 0) out vec4 fragColour;

const int ITERATIONS = 30;
const float STEP = 0.01;
const float DITHER = 0.2;
//const float DURATION = 2.0;
const float PI = 3.14159265;

uniform sampler2D noiseTex;

float sq(float _n)
{
    return _n * _n;
}

vec3 grayscale(vec3 _v)
{
    return vec3((_v.x + _v.y + _v.z) / 3.0);
}

/*float hash( float n )
{
    return fract(cos(n)*41415.92653);	//https://www.shadertoy.com/view/4sXGRM
    //return fract(sin(n)*753.5453123);	//https://www.shadertoy.com/view/4sfGzS
}*/

/*float noise( in vec3 x )
{
    vec3 f = fract(x);
    vec3 p = x - f; // this avoids the floor() but doesnt affect performance for me.
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}*/

float noise(vec3 x) // iq's 3D noise
{
    vec3 f = fract(x);
    vec3 p = x - f;
    f = f*f*(3.0 - 2.0*f);
    vec2 uv = (p.xy + vec2(37.0, 17.0) * p.z) + f.xy;
    vec2 rg = texture(noiseTex, (uv + 0.5)/256.0, -100.0).rg;
    return mix(rg.y, rg.x, f.z);
}

float fbm(vec3 x)
{
    float r = 0.0;
    float w = 1.0, s = 1.0;
    for (int i=0; i<5; i++)
    {
        w *= 0.5;
        s *= 2.0;
        r += w * noise(s * x);
    }
    return r;
}

float sphere(vec3 p, float r)
{
    return length(p) - r;
}

vec4 scene(vec3 pos, vec3 dir, out float dist)
{
    float nTime = clamp(sqrt(elapsed_time / duration), 0.0, 1.0);
    float rnTime = 1.0 - nTime;
    
    //Main radius
    float rad1 = 0.5 * nTime;//0.4;
    //Smoke radius
    float rad2 = 0.9 * nTime - 0.2;//0.35;
    //Radius used to interpolate from fire to smoke
    float rad0 = 0.6 * nTime;
    
    float d = sphere(pos, rad2);
    d -= rad0 * fbm(pos * 2.0 + elapsed_time * 1.1);
    dist = d;
    
    vec4 colour = vec4(0.0);
    float r = length(pos) / rad0;
    
    colour.a = smoothstep(1.0, 0.0, clamp(d / SMOOTHNESS, 0.0, 1.0));
    colour.a *= smoothstep(1.0, 0.0, clamp((r - 1.3) * 2., 0., 1.));
    colour.a *= clamp(1.5 * rnTime, 0.0, 1.0);
    
    //Base colour
    colour.rgb = 1.2 * mix((vertCol.rgb + vec3(0.05)) * 5.5, vertCol.rgb * 0.9, clamp((r - 1.1) * 3.0, 0.0, 1.0));
    
    //Fire->Smoke interpolation
    colour.rgb = mix(colour.rgb, grayscale(colour.rgb) * 0.25, clamp((rad2 - rad0) * 15.0, 0.0, 1.0));

    //Ambient smoke
    colour.rgb = mix(colour.rgb, vec3(0.95,0.95,0.97)*0.25, clamp((r - 1.3) * 3.0, 0., 1.));

    return colour;
}

vec4 render(vec3 eye, vec3 dir, vec2 fragCoord)
{
    vec4 color = vec4(0.0);

    vec3 pos = eye + dir * DITHER;

    for (int i=0; i<ITERATIONS; i++)
    {
        if (color.a > 0.99) continue;

        float dist;
        vec4 d = scene(pos, dir, dist);
        d.rgb *= d.a;
        color += d * (1.0 - color.a);

        pos += dir * max(STEP, dist * 0.5);
    }

    color.rgb += vec3(0.1 * vec3(1.0,0.5,0.1)) * (1.0 - color.a);

    return clamp(color, 0.0, 1.0);
}

vec3 lookAtDir(vec2 uv, vec3 up)
{
    vec3 f = vec3(0., 0., -1.);
    vec3 r = vec3(1., 0., 0.);
    vec3 u = vec3(0., 1., 0.);
    //vec3 u = normalize(vec3(-1./sqrt(2.0),-1./sqrt(2.0), 0.));

    return normalize(f + (uv.x * r + uv.y * u));
    //return normalize(vec3(0.0, 10.0, 0.0));
}

void main()
{
    vec2 uv = UV * 2.0 - 1.0;
    vec3 eye = vec3(0.0, 0.0, 1.5);
    vec3 dir = lookAtDir(uv, vec3(0.0, 1.0, 0.0));

    vec4 colour = render(eye, dir, gl_FragCoord.xy);

    fragColour = colour;

    //fragColour = vec4(uv, 1.0, 1.0);
}
