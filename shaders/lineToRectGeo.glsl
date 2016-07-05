#version 410 core

layout ( lines ) in;
layout ( triangle_strip ) out;
layout ( max_vertices = 4 ) out;

in vec2 geoScale[2];
in vec4 geoColour[2];

out vec4 vertColour;
out vec2 UV;

uniform mat4 MVP;

void main()
{
    vertColour = geoColour[0];

    vec4 start = gl_in[0].gl_Position;
    vec4 end = gl_in[1].gl_Position;

    vec4 dir = normalize(end - start);
    vec4 norm = dir;
    norm.xy = norm.yx;
    norm.x *= -1.0;
    //vec4 norm = vec4(0.0, 1.0, 0.0, 0.0);
    norm *= geoScale[0].x;
    //norm *= 20.0;

    gl_Position = MVP * (start - norm);
    UV = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = MVP * (start + norm);
    UV = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = MVP * (end - norm);
    UV = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = MVP * (end + norm);
    UV = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
