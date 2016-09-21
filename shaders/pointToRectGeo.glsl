#version 430 core

layout ( points ) in;
layout ( triangle_strip ) out;
layout ( max_vertices = 4 ) out;

in vec2 geoScale[1];
in vec4 geoColour[1];
in vec4 seeds[1];
in float geoAngle[1];

out vec4 vertColour;
out vec2 UV;

out float iGlobalTime;
out float variation_seed;
out float explosion_seed;
out float speed;

uniform mat4 MVP;

void main()
{
    vertColour = geoColour[0];

    iGlobalTime = seeds[0].x;
    variation_seed = seeds[0].y;
    explosion_seed = seeds[0].z;
    speed = clamp(12.0 / seeds[0].w, 0.4, 0.8);

    vec4 pos = gl_in[0].gl_Position;

    vec2 corners[4];

    //Scaling.
    corners[0] = vec2(-geoScale[0].x, -geoScale[0].x);
    corners[1] = vec2(geoScale[0].x, -geoScale[0].x);
    corners[2] = vec2(-geoScale[0].x, geoScale[0].x);
    corners[3] = vec2(geoScale[0].x, geoScale[0].x);

    float s = sin(geoAngle[0]);
    float c = cos(geoAngle[0]);

    for(int i = 0; i < 4; ++i)
    {
        float x = corners[i].x;
        float y = corners[i].y;
        corners[i] = vec2(x * c - y * s, x * s + y * c);
    }

    gl_Position = MVP * (pos + vec4(corners[0].x, corners[0].y, 0.0, 0.0));
    UV = vec2(0.0, 0.0);
    EmitVertex();

    gl_Position = MVP * (pos + vec4(corners[1].x, corners[1].y, 0.0, 0.0));
    UV = vec2(1.0, 0.0);
    EmitVertex();

    gl_Position = MVP * (pos + vec4(corners[2].x, corners[2].y, 0.0, 0.0));
    UV = vec2(0.0, 1.0);
    EmitVertex();

    gl_Position = MVP * (pos + vec4(corners[3].x, corners[3].y, 0.0, 0.0));
    UV = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();

    /*std::array<vec3, 6> pos;

    //Scaling
    pos[0] = {-_d.m_x / 2.0f, -_d.m_y / 2.0f, -0.5f};
    pos[1] = {_d.m_x / 2.0f, -_d.m_y / 2.0f, -0.5f};
    pos[2] = {-_d.m_x / 2.0f, _d.m_y / 2.0f, -0.5f};

    pos[3] = {_d.m_x / 2.0f, _d.m_y / 2.0f, -0.5f};
    pos[4] = {-_d.m_x / 2.0f, _d.m_y / 2.0f, -0.5f};
    pos[5] = {_d.m_x / 2.0f, -_d.m_y / 2.0f, -0.5f};

    float s = sin(_ang);
    float c = cos(_ang);

    //Rotation
    for(auto &i : pos)
    {
        i = {
            i.m_x * c - i.m_y * s,
            i.m_x * s + i.m_y * c,
            i.m_z
        };
    }

    //Positioning
    for(auto &i : pos)
    {
        i += _p;
    }

    //UVs
    std::array<vec2, 6> uv;

    uv[0] = {0.0f, 0.0f};
    uv[1] = {1.0f, 0.0f};
    uv[2] = {0.0f, 1.0f};

    uv[3] = {1.0f, 1.0f};
    uv[4] = {0.0f, 1.0f};
    uv[5] = {1.0f, 0.0f};

    for(int i = 0; i < 6; ++i)
    {
        m_verts.push_back(pos[i]);
        m_UVs.push_back(uv[i]);
        m_colours.push_back(_col);
    }*/
}
