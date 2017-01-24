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
    fragColour.a = 1.0;
}
