#version 410 core

const int NUM_LIGHTS = 512;
uniform int ACTIVE_LIGHTS;

in vec4 gl_FragCoord;
in vec2 UV;

uniform sampler2D diffuse;
uniform sampler2D normal;
uniform sampler2D position;

uniform sampler2D ambient;
uniform vec2 ambientSteps;
uniform mat4 inverseVP;
uniform float lightMul;

struct light
{
    vec4 pos;
    vec3 col;
    float lum;
};

layout( std140 ) uniform lightBuffer
{
    light buf [NUM_LIGHTS];
} lbuf;

uniform vec2 iResolution;
uniform float iGlobalTime;

layout (location = 0) out vec4 fragColour;

float max3(vec3 _vec)
{
    float max_xy = max( _vec.x, _vec.y );
    return max( max_xy, _vec.z );
    //return max( max( _vec.x, _vec.y ), _vec.z );
}

float dotLight( vec3 lv, vec3 nv )
{
    //Normal based multiplier.
    float mul = dot( lv, nv );
    mul = clamp( mul, 0.0f, 1.0f );

    return mul;
}

vec3 calcDiffuseSpec(vec3 _fragPos, vec3 _fragNorm, vec3 _lightPos, vec3 _lightCol, float _lightBrightness, float _attn, float _dmul, float _smul)
{
    vec3 lightVec = _lightPos - _fragPos;//

    float lightVecLen = length( lightVec );//
    lightVec = normalize(lightVec);

    /*float dist = (256.0 * max3(_lightCol) * _dmul * dot(_fragNorm, lightVec)) / _attn;
    if(lightVecLen * lightVecLen > dist)
        return vec3(0.0);*/

    float attenuation = (1.0 + _attn * lightVecLen * lightVecLen);//1 + parameter + distance^2

    //Diffuse
    //vec3 lightCol = /*_lightBrightness * _lightCol*/ vec3(dotLight(lightVec, _fragNorm)) / attenuation;//Avoid negative backlights, divide by attn then mul by col.
    vec3 lightCol = _lightBrightness * _lightCol * vec3(dotLight(lightVec, _fragNorm)) / attenuation;

    //Spec
    vec3 incidenceVec = -lightVec;//
    vec3 reflectionVec = reflect(incidenceVec, _fragNorm);//
    float angMul = clamp( dot(vec3(0.0, 0.0, 1.0), reflectionVec), 0.0, 1.0 );
    float specCf = pow(angMul, 200.0);
    lightCol += _lightBrightness * _lightCol * (specCf * _smul) / attenuation;

    return lightCol;
}

void main()
{
    //vec2 uv = gl_FragCoord.xy / iResolution;
    //Multiply up by current aspect ratio.
    //uv.y *= iResolution.y / iResolution.x;
    vec2 uv = UV + vec2(0.5);

    fragColour = texture(diffuse, uv);

    if(fragColour.a == 0.0)
        discard;

    vec3 fragNormal = texture(normal, uv).xyz;
    vec3 fragPosition = texture(position, uv).xyz;

    vec3 lightCol = vec3(0.0);

    for(int i = 0; i < ACTIVE_LIGHTS; i++)
    {
        lightCol += calcDiffuseSpec(fragPosition, fragNormal, lbuf.buf[i].pos.xyz, lbuf.buf[i].col, lbuf.buf[i].lum, 2.0, 0.25, 1.0);
    }
    for(int i = 0; i < ambientSteps.x; i++)
    {
        for(float j = 0; j < ambientSteps.y; j++)
        {
            vec2 coord = vec2(i, j) / ambientSteps;
            vec3 ambientCol = 256.0 * texture(ambient, coord).xyz;
            vec4 ambientPos = vec4(1.0);
            ambientPos.xy = coord * 2 - vec2(1.0);//Convert to NDC (-1.0 to 1.0)
            ambientPos = inverseVP * ambientPos;//Multiply by inverse vp to project to world space.
            ambientPos.z = -400.0 + 5.0 * sin(iGlobalTime + coord.x) + 0.001 * length(ambientCol);
            lightCol += calcDiffuseSpec(fragPosition, fragNormal, ambientPos.xyz, ambientCol, lightMul, 0.00025, 0.5, 1.0);
        }
    }

    fragColour.xyz *= lightCol;

    //fragColour.xyz = texture(position, uv).xyz;
}
