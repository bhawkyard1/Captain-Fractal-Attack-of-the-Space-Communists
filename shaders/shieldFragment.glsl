#version 410 core

in vec4 fragCoord;
in vec2 UV;
in vec3 normal;

uniform float iGlobalTime;
uniform float alpha;

out vec4 fragColour;

void main()
{
        float t = iGlobalTime * 512.0;
        vec2 p = 2.0 * UV - 1.0;
        vec2 op = p;
        vec4 color = vec4(0.0, 0.5, 1.0, 1.0);
        float v = length(2.0*UV-1.0);

        p *= 10.0;

        float movex = sin(t + p.x + p.y * v);
        float movey = sin(t * 0.5);
        float grid = sin(p.x + movex) * sin(p.y + movey);
        float grid2 = grid + 0.2;
        float grid3 = grid + 0.3;

        float inner = 1.0 - ceil(grid + grid2);
        float outer = 1.0 - ceil(grid + grid3);

        inner = max(0.0,min(1.0,inner));
        outer = max(0.0,min(1.0,outer));
        float stencil = inner-outer;

        color.a = inner - outer;
        p *= 0.5;
        p = abs(p);
        p *= v;
        p.x += sin(p.y*sin(p.x));
        float beam = sin(p.x+t);
        beam = max(0.0,min(1.0,beam));

        /*color.a += beam;
        color.a *= beam*grid;
        color *= 2.0;
        color.a += stencil*beam;
        color.a = max(0.0,min(1.0,color.b));
        color.g = color.a*0.5;*/


        // high constrast and vinj
        color *= color * 10.0;
        color *= 1.0-v;

        //output final color
        fragColour = vec4(color);
        fragColour.a = fragColour.g * alpha;
}
