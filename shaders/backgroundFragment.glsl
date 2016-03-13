#version 410 core

#define iterations 12

//This variable does some really weird shit, yo.
//At low values, looks like a big space city.
//Really trippy stuff.
//TO-DO: Delete before Jon reads these comments.
#define formuparam 0.53

#define volsteps 10
#define stepsize 0.1

#define tile   0.850

#define brightness 0.0015
#define darkmatter 0.300
#define distfading 0.730
#define saturation 0.850

in vec4 gl_FragCoord;
uniform vec2 iResolution;
uniform float iGlobalTime;
uniform float zoom;
uniform vec3 unidir;
out vec4 fragColour;

void main()
{
    //Get coords and direction.
    //The current UV point.
    vec2 uv = gl_FragCoord.xy / iResolution.xy - 0.5;
    //Multiply up by current aspect ratio.
    uv.y *= iResolution.y / iResolution.x;

    //Don't know what this does. Seems to kind of link together current uv pos and the zoom level.
    vec3 uvzoom = vec3(uv * zoom, 1.0);

    //Derive speed from the universe velocity.
    float speed = unidir.length() / 200.0;
    float time = iGlobalTime * speed;

    //This controls the direction.
    //from.z controls background changing.
    vec3 from = unidir
    from *= iGlobalTime / 200.0;

    //volumetric rendering
    //S and fade control brightness. High s makes everything very blue.
    float s = 0.2, fade = 1.0;
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

    //FROM : http://casual-effects.blogspot.co.uk/2013/08/starfield-shader.html
    // Motion blur; increases temporal coherence of undersampled flickering stars
    // and provides temporal filtering under true motion.
    float3 oldValue = texelFetch(oldImage, int2(gl_FragCoord.xy), 0).rgb;
    gl_FragColor.rgb = lerp(oldValue - vec3(0.004), gl_FragColor.rgb, 0.5);
    gl_FragColor.a = 1.0;
}
