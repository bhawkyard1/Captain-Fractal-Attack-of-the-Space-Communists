#version 410 core

//This variable does some really weird shit, yo.
//At low values, looks like a big space city.
//Really trippy stuff.
//TO-DO: Delete before Jon reads these comments.
//#define formuparam 0.53

#define volsteps 16
#define stepsize 0.3

#define tile 0.850

#define brightness 0.0015
#define darkmatter 0.300

//Higher= brigwhter/less faded.
#define distfading 0.5

#define saturation 0.6

in vec4 gl_FragCoord;

uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float zoom;
uniform vec2 univel;
uniform vec2 unipos;
uniform float iterations;

float formuparam = 0.5 + (sin(iGlobalTime / 256.0)) / 16.0;

out vec4 fragColour;

float vec2Ang(vec2 _v)
{
    return atan(_v.y, _v.x);
}

vec2 rotatePoint(vec2 _p, float _a)
{
    mat2 matt;
    matt[0][0] = cos(_a);
    matt[0][1] = -sin(_a);
    matt[1][0] = sin(_a);
    matt[1][1] = cos(_a);
    return matt * _p;
}

void main()
{
    //Get coords and direction.
    //The current UV point.
    vec2 uv = gl_FragCoord.xy / iResolution.xy - 0.5;
    //Multiply up by current aspect ratio.
    uv.y *= iResolution.y / iResolution.x;

    /*float ang = vec2Ang(univel);
    float len = univel.length();
    uv = rotatePoint(uv, ang);
    uv.y *= 1.0 + pow(len, 5.0) / 1000.0;
    uv = rotatePoint(uv, -ang);*/

    //Don't know what this does. Seems to kind of link together current uv pos and the zoom level.
    vec3 uvzoom = vec3(uv * zoom, 20.0);

    //This controls the direction.
    //from.z controls background changing.
    //vec3 from = vec3( cos(iGlobalTime), 0.0f, 0.001);
    vec3 from = vec3(unipos.xy + 20., 0.0001);
    from.xy /= 20000.0;
    from += vec3(12.0, 1.0, 0.0);
    from.z *= iGlobalTime;

    //volumetric rendering
    //S and fade control brightness. High s makes everything very blue.
    float s = 0.5, fade = 1.0;
    vec3 v = vec3(0.0);
    for (int r = 0; r < volsteps; r++)
    {
        //P controls how close we are. If wo are too far out, we will see the tiling!
        vec3 p = from + s * uvzoom;//* 0.5;
        p = abs( vec3(tile) - mod(p, vec3(tile * 2.0))); // tiling fold
        float pa, a = pa = 0.0;
        for (int i = 0; i < iterations; i++)
        {
            p = abs(p) / dot(p, p) - formuparam; // the magic formula
            a += abs(length(p) - pa); // absolute sum of average change
            pa = length(p);
        }
        float dm = max(0.0, darkmatter - a * a * 0.001); //dark matter
        a *= a * a; // add contrast
        if(r > 6) fade *= 1.0 - dm; // dark matter, don't render near
        v += fade;
        v += vec3(s, s * s, s * s * s * s) * a * brightness * fade; // coloring based on distance
        fade *= distfading; // distance fading
        s += stepsize;
    }
    v = mix( vec3(length(v)), v, saturation); //color adjust

    fragColour = vec4(v * 0.01, 1.0);

    /*if(gl_FragCoord.x > 960)
    {
        fragColour = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else
    {
        fragColour = vec4(0.0, 1.0, 0.0, 1.0);
    }*/

    //FROM : http://casual-effects.blogspot.co.uk/2013/08/starfield-shader.html
    // Motion blur; increases temporal coherence of undersampled flickering stars
    // and provides temporal filtering under true motion.
    /*float3 oldValue = texelFetch(oldImage, int2(gl_FragCoord.xy), 0).rgb;
    gl_FragColor.rgb = lerp(oldValue - vec3(0.004), gl_FragColor.rgb, 0.5);
    gl_FragColor.a = 1.0;*/
}
