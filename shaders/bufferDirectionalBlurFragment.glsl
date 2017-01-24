#version 410 core

#define DIRECTIONAL_STEP_SIZE 0.01
#define BOX_STEP_SIZE 0.0005

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;
uniform vec2 iResolution;
uniform vec2 vel;

uniform vec4 used;

layout (location = 0) out vec4 fragColour;
//layout (location = 1) out float fragDepth;

/*float gaussian(float _x)
{
    float variance = 1.0;
    float expected = 0.0;
    float a = 1.0 / (variance * sqrt(2.0 * 3.14159));
    float power = (_x - expected) / variance;
    power *= power;
    power *= -0.5;
    return a * exp(power);
}*/

void main()
{
    //Get coords and direction.
    //The current UV point.
    vec2 uv = UV + vec2(0.5);

    float len = vel.length();
    vec2 unitVel = vel / len;
    unitVel.y /= iResolution.y / iResolution.x;
    fragColour = texture(diffuse, uv);

    for(float i = -len / 2.0; i < len / 2.0; i += DIRECTIONAL_STEP_SIZE)
    {
        vec2 samplePos = uv + (vec2(-unitVel.x, unitVel.y) * i) / 20000.0;
        samplePos = clamp(samplePos, vec2(0.0, 0.0), vec2(1.0, 1.0));
        //fragColour = max(vec4(samplePos.xy, 0.0, 1.0), fragColour);
        fragColour = max(texture(diffuse, samplePos), fragColour);
    }

    //fragDepth = 0.1 + distance(uv, vec2(0.5));
}
