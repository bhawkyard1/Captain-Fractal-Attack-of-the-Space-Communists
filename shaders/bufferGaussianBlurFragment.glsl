#version 410 core

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;

layout (location = 0) out vec4 fragColour;

uniform bool horizontal;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{
    vec2 tex_offset = 1.0 / textureSize(diffuse, 0); // gets size of single texel
    vec3 result = texture(diffuse, UV).rgb * weight[0]; // current fragment's contribution
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(diffuse, UV + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(diffuse, UV - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(diffuse, UV + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(diffuse, UV - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    fragColour = vec4(result, 1.0);
}
