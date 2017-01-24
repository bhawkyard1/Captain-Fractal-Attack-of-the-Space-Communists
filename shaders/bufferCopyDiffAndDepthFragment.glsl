#version 410 core

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;
uniform vec2 iResolution;

uniform vec4 used;

layout (location = 0) out vec4 fragColour;
//layout (location = 1) out float fragDepth;

void main()
{
    //Get coords and direction.
    //The current UV point.
    vec2 uv = UV + vec2(0.5);
    //Multiply up by current aspect ratio.
    //uv.y *= iResolution.y / iResolution.x;
    //uv.y /= iResolution.y / iResolution.x;

    fragColour = texture(diffuse, uv);
    //fragDepth = distance(uv, vec2(0.5));
    //if(fragColour.a == 0.0)
    //    discard;
}
