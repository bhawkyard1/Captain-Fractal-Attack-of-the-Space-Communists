#version 410 core

uniform vec2 fstart;
uniform vec2 fend;
uniform vec2 resolution;

in vec4 gl_FragCoord;
in vec4 vertColour;

out vec4 fragColour;

void main()
{
    vec2 fdiff = (fend - fstart);
    vec2 tstart = fstart - fdiff * 0.0001;

    float dist = (gl_FragCoord.xy - tstart).length();
    dist = pow(dist, 4.0);
    dist /= 12.0;

    float dotMul = dot(normalize(fend - tstart), normalize(gl_FragCoord.xy - tstart));
    dotMul = pow(dotMul, 100);

    fragColour = vertColour;

    fragColour += vec4(1.0) / dist;

    fragColour *= dotMul;
    fragColour.a *= vertColour.a;

    //fragColour = vec4(fract(sin(dot(fragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453), fract(sin(dot(fragCoord.xy ,vec2(12.9898,78.233))) * 43758.5453), 0.0, 1.0);
}
