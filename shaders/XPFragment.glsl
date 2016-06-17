#version 410 core

in vec2 UV;
in vec4 fragCoord;

out vec4 fragColour;

uniform float xp;

float repetitions = 5.0;
vec4 activeColour = vec4(1.0);
vec4 inactiveColour = vec4(0.5);
vec4 outerColour = vec4(0.0);

float magns(vec2 _in)
{
    return _in.x * _in.x + _in.y * _in.y;
}

bool circle(vec2 _uv, vec2 _center, float _radius)
{
    return magns(_uv - _center) < _radius * _radius;
}

void main()
{
    vec2 suv = UV;
    suv.x *= repetitions;
    vec2 ruv = fract(suv);

    if(circle(ruv, vec2(0.5, 0.5), 1.0 / repetitions))
    {
        if(UV.x < xp) fragColour = activeColour;
        else fragColour = inactiveColour;
    }
    else
    {
        fragColour = outerColour;
    }
}
