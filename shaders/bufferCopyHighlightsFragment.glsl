#version 410 core

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;
uniform vec2 iResolution;

uniform vec4 used;

layout (location = 0) out vec4 fragColour;

void main()
{
    vec2 uv = UV + vec2(0.5);

    vec4 col = texture(diffuse, uv);
    col.xyz -= vec3(1.0);
    col.xyz = max(col.xyz, vec3(0.0));
    fragColour = col;
}
