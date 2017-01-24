#version 410 core

//Stolen from
//http://devlog-martinsh.blogspot.co.uk/2011/11/glsl-depth-of-field-with-bokeh-v21.html

uniform sampler2D diffuse;
uniform vec2 bgl_dim;
uniform vec2 mousePos;
uniform float vel;

#define PI  3.14159265

layout( location = 0 ) out vec4 fragColour;

float width = bgl_dim.x; //texture width
float height = bgl_dim.y; //texture height

vec2 texel = vec2(1.0/width,1.0/height);

//------------------------------------------
//user variables

int samples = 16; //samples on the first ring
int rings = 5; //ring count

float maxblur = 4.0; //clamp value of max blur

float threshold = 0.4; //highlight threshold;
float gain = 10.0; //highlight gain;

float bias = 0.4; //bokeh edge bias
float fringe = 0.5; //bokeh chromatic aberration/fringing

bool noise = true; //use noise instead of pattern for sample dithering
float namount = 0.000001; //dither amount

/*
next part is experimental
not looking good with small sample and ring count
looks okay starting from samples = 4, rings = 4
*/

bool pentagon = false; //use pentagon as bokeh shape?
float feather = 0.4; //pentagon shape feather

//------------------------------------------


float penta(vec2 coords) //pentagonal shape
{
    float scale = float(rings) - 1.3;
    vec4  HS0 = vec4( 1.0,         0.0,         0.0,  1.0);
    vec4  HS1 = vec4( 0.309016994, 0.951056516, 0.0,  1.0);
    vec4  HS2 = vec4(-0.809016994, 0.587785252, 0.0,  1.0);
    vec4  HS3 = vec4(-0.809016994,-0.587785252, 0.0,  1.0);
    vec4  HS4 = vec4( 0.309016994,-0.951056516, 0.0,  1.0);
    vec4  HS5 = vec4( 0.0        ,0.0         , 1.0,  1.0);

    vec4  one = vec4( 1.0 );

    vec4 P = vec4((coords),vec2(scale, scale));

    vec4 dist = vec4(0.0);
    float inorout = -4.0;

    dist.x = dot( P, HS0 );
    dist.y = dot( P, HS1 );
    dist.z = dot( P, HS2 );
    dist.w = dot( P, HS3 );

    dist = smoothstep( -feather, feather, dist );

    inorout += dot( dist, one );

    dist.x = dot( P, HS4 );
    dist.y = HS5.w - abs( P.z );

    dist = smoothstep( -feather, feather, dist );
    inorout += dist.x;

    return clamp( inorout, 0.0, 1.0 );
}

vec3 color(vec2 coords,float blur) //processing the sample
{
    vec3 col = vec3(0.0);

    col.r = texture(diffuse,coords + vec2(0.0,1.0)*texel*fringe*blur).r;
    col.g = texture(diffuse,coords + vec2(-0.866,-0.5)*texel*fringe*blur).g;
    col.b = texture(diffuse,coords + vec2(0.866,-0.5)*texel*fringe*blur).b;

    vec3 lumcoeff = vec3(0.299,0.587,0.114);
    float lum = dot(col.rgb, lumcoeff);
    float thresh = max((lum-threshold)*gain, 0.0);
    return col+mix(vec3(0.0),col,thresh*blur);
}

vec2 rand(in vec2 coord) //generating noise/pattern texture for dithering
{
    float noiseX = ((fract(1.0-coord.s*(width/2.0))*0.25)+(fract(coord.t*(height/2.0))*0.75))*2.0-1.0;
    float noiseY = ((fract(1.0-coord.s*(width/2.0))*0.75)+(fract(coord.t*(height/2.0))*0.25))*2.0-1.0;

    if (noise)
    {
        noiseX = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233))) * 43758.5453),0.0,1.0)*2.0-1.0;
        noiseY = clamp(fract(sin(dot(coord ,vec2(12.9898,78.233)*2.0)) * 43758.5453),0.0,1.0)*2.0-1.0;
    }
    return vec2(noiseX,noiseY);
}

void main()
{
    vec2 uv = gl_FragCoord.xy / bgl_dim;

    float depth = distance(uv, mousePos);

    float blur = depth * (vel / 500.0) + 0.001;

    vec2 noise = rand(uv)*namount*blur;

    float w = (1.0/width)*blur+noise.x;
    float h = (1.0/height)*blur+noise.y;

    vec3 col = texture(diffuse, uv).rgb;
    float s = 1.0;

    int ringsamples;

    for (int i = 1; i <= rings; i += 1)
    {
        ringsamples = i * samples;

        for (int j = 0 ; j < ringsamples ; j += 1)
        {
            float step = PI*2.0 / float(ringsamples);
            float pw = (cos(float(j)*step)*float(i));
            float ph = (sin(float(j)*step)*float(i));
            float p = 1.0;
            if (pentagon)
            {
                p = penta(vec2(pw,ph));
            }
            col += color(uv + vec2(pw*w,ph*h),blur)*mix(1.0,(float(i))/(float(rings)),bias)*p;
            s += 1.0*mix(1.0,(float(i))/(float(rings)),bias)*p;
        }
    }

    col /= s;

    fragColour.rgb = col;

    /*if(depth > 500 && depth < 750)
    {
        fragColour.rgb = vec3(1.0, 0.0, 0.0);
    }*/

    //fragColour.rgb = vec3(depth);
    fragColour.a = 1.0;

    //fragColour = texture(diffuse, uv);
    //fragColour.rgb = vec3(uv.x, uv.y, 1.0);
}
