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
    /*vec3 toCam = cam - pos;
    float depth = distance(cam, pos);
    toCam /= depth;
    //toCam = normalize(toCam);
    depthDiff = abs(depth - fdepth) * fmul;
    scale = 2.0f + depthDiff;

    if(toCam.y == 1.0) toCam = vec3(0.0, 0.0, 1.0);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCam, up);
    right *= scale;

    pos -= right * 0.5;
    gl_Position = VP * vec4(pos, 1.0);
    uv = vec2(0.0, 0.0);
    EmitVertex();

    pos.y += scale;
    gl_Position = VP * vec4(pos, 1.0);
    uv = vec2(0.0, 1.0);
    EmitVertex();

    pos.y -= scale;
    pos += right;
    gl_Position = VP * vec4(pos, 1.0);
    uv = vec2(1.0, 0.0);
    EmitVertex();

    pos.y += scale;
    gl_Position = VP * vec4(pos, 1.0);
    uv = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();*/
}
