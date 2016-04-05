#version 410 core

in vec4 gl_FragCoord;
in vec2 UV;

uniform vec4 inColour;

out vec4 fragColour;

void main()
{
    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(inColour.rgb, 0.0), dist * 2.0);
    fragColour *= 2.0;
    fragColour.a *= inColour.a;
}
