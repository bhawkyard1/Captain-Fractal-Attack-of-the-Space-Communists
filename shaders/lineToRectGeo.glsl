#version 410 core

layout ( points ) in;
layout ( triangle_strip ) out;
layout ( max_vertices = 4 ) out;

in float scale;
out vec2 UV;

void main()
{
    vec3 start = gl_in[0].gl_Position.xyz;
    vec3 end = gl_in[1].gl_Position.xyz;

    vec3 dir = (end - start).normalize();
    vec3 norm = dir.yxz;
    norm *= scale;

    gl_Position = start - norm;
    UV = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = start + norm;
    UV = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = end - norm;
    UV = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = end + norm;
    UV = vec2(1.0, 1.0);
    EmitVertex();

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
