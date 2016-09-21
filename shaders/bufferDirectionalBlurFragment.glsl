#version 430 core

#define DIRECTIONAL_STEP_SIZE 0.01
#define BOX_STEP_SIZE 0.0005

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;
uniform vec2 iResolution;
uniform vec2 vel;

uniform vec4 used;

layout (location = 0) out vec4 fragColour;

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

    //Gaussian style blur thing around edges to hide seams because I am lazy
    //Distance to center, blur will be stronger at the edges, and when player goes fast.
    /*float uvdist = distance(uv, vec2(0.5, 0.5));
    //Step size is dictated by uvdist and speed, as both approach zero step size increases (effectively reducing size of kernel)

    len = length(vel) * uvdist * 0.00001;
    //len = 0.2 * uvdist * len;
    int steps = 1;
    vec4 col = vec4(0.0);
    for(float x = -len / 2.0; x < len / 2.0; x += BOX_STEP_SIZE)
    {
        for(float y = -len / 2.0; y < len / 2.0; y += BOX_STEP_SIZE)
        {
            col += texture(diffuse, uv + vec2(x, y));
            steps++;
        }
    }
    col /= (steps - 1);
    fragColour = max(fragColour, col);*/
}
