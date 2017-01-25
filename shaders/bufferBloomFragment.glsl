#version 410 core

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;
uniform sampler2D bloom;

uniform vec4 used;

layout (location = 0) out vec4 fragColour;

void main()
{
    vec2 uv = UV + vec2(0.5);
    fragColour = texture(diffuse, uv) + texture(bloom, UV);
    fragColour.rgb = clamp(fragColour.rgb, vec3(0.0), vec3(1.0));
    fragColour.a = 1.0;
}
