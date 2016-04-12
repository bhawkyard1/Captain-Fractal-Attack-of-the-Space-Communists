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
    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(vertColour.rgb, 0.0), dist * 2.0);
    fragColour *= 1.0;
    fragColour.a *= vertColour.a;
}
