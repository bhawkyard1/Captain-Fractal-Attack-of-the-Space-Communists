#version 410 core

in vec4 gl_FragCoord;
in vec2 UV;

uniform float alpha;

out vec4 fragColour;

void main()
{
    float dist = length(vec2(0.5, 0.5) - UV);
    fragColour = mix(vec4(1.0), vec4(1.0,1.0,0.8,0.0), dist * 2.0);
    fragColour.a *= alpha;
}
