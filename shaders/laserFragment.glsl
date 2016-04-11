#version 410 core


uniform vec2 resolution;

in vec4 gl_FragCoord;
in vec4 vertColour;
in vec2 UV;

in vec2 fstart;
in vec2 fend;

out vec4 fragColour;

void main()
{
    /*vec2 fdiff = (fend - fstart);
    vec2 tstart = fstart - fdiff * 0.0001;

    float dist = (gl_FragCoord.xy - tstart).length();
    dist = pow(dist, 4.0);
    dist /= 12.0;

    float dotMul = dot(normalize(fend - tstart), normalize(gl_FragCoord.xy - tstart));
    dotMul = pow(dotMul, 100);*/

    //fragColour = vertColour;

    /*fragColour += vec4(1.0) / dist;

    fragColour *= dotMul;
    fragColour.a *= vertColour.a;*/

    //fragColour = vec4(fract(sin(dot(fragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453), fract(sin(dot(fragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453), 0.0, 1.0);

    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(vertColour.rgb, 0.0), dist * 2.0);
    fragColour *= 1.5;
    fragColour.a *= vertColour.a;

    //fragColour = vertColour * 2.2;

    //fragColour = mix(vec4(1.0), vertColour, abs(UV.x));

    //fragColour.a *= UV.y;
}
