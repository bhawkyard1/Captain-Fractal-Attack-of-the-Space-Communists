#version 430 core

in vec4 gl_FragCoord;
in vec2 UV;
in vec4 vertCol;

in float iGlobalTime;
in float variation_seed;
in float explosion_seed;
in float speed;

layout (location = 0) out vec4 fragColour;

//float iGlobalTime = vertCol.a;

#define CAM_ROTATION_SPEED 11.7
#define CAM_TILT .15				// put 0. if you do not want to animate camera vertically
#define CAM_DIST 3.8

#define MAX_MULT_EXPLOSIONS 3

// the bounding sphere of the explosion. this is less general but means that
// ray cast is only performed for nearby pixels, and raycast can begin from the sphere
// (instead of walking out from the camera)
float expRadius = 1.75;
// keep this constant for a whole explosion, but when differing from explosion to the next one, you get non-identical looking ones
float downscale = 1.25;				// how much smaller (than expRadius) one explosion ball should be. bigger value = smaller. 1.0 = no scale down.
int steps = int(clamp(4 / speed, 24, 64));				// iterations when marching through cloud noise. default = 64. 40 might still suffice. When putting higher, explosion becomes too dense, so make colBottom and colTop more transparent.
float grain = 1.5;					// increase for more detailed explosions, but then you should also increase iterations (and decrease step, which is done automatically)
//float speed = 0.5;					// total animation speed (time stretch). nice = 0.5, default = 0.4
float ballness = 0.5;				// lower values makes explosion look more like a cloud. higher values more like a ball.
float growth = 2.2;					// initial growth to explosion ball. lower values makes explosion grow faster
float fade = 1.6;					// greater values make fade go faster but later. Thus greater values leave more smoke at the end.
float thinout_smooth = 0.7;			// smoothed thinning out of the outer bounding sphere. 1.0 = no smoothening, 0.0 = heavy thinning, nice = 0.65 to 0.75
float density = 1.2;				// higher values make sharper difference between dark and bright colours. low values make more blurry, less colour spread and more transparent. default = 1.25 or 1.35
vec2 brightness = vec2(2.0, 3.2);	// x = constant offset, y = time-dependent factor
vec2 brightrad = vec2(1.3, 2.0);	// adds some variation to the radius of the brightness falloff. x = constant offset, y = density-dependent factor
vec4 colTop = vec4(0.2, 0.2, 0.2, 0.2);
float colour_low = 0.65;				// the lower the value, the more black spots appear in the explosion. the higher, the more even the explosion looks like.
float contrast = 1.8;				// final colour contrast. higher values make ligher contrast. default = 1.0
float rolling_init_damp = 0.2;		// rolling animation initial damping. 0.0 = no damping. nice = 0.2, default = 0.15
float rolling_speed = 2.0;			// rolling animation speed (static over time). default = 1.0
const int mult_explosions = MAX_MULT_EXPLOSIONS;	// how many explosion balls to draw
//float variation_seed = 0.0;			// influences position variation of the different explosion balls
float delay_seed = 0.0;				// influences the start delay variation of the different explosion balls
float delay_range = 0.1;			// describes the maximum delay for explosion balls to start up. Notice, this delay is relative to one explosion ball duration, so actually before speed is applied.
float ball_spread = 1.0;			// how much to spread ball starting positions from the up vector. 0.0 = all on up vector, 1.0 = any direction between up and down vector.
float tmax = 1.25;

struct Ball
{
    vec3 offset;
    vec3 dir;
    float delay;
};

Ball balls[MAX_MULT_EXPLOSIONS];

float getTime()
{
    float time = clamp(iGlobalTime * speed, 0.0, 1.0);
    return fract(time / tmax) * tmax;
}

const float pi=3.14159265;

float hash( float n )
{
    return fract(cos(n)*41415.92653);	//https://www.shadertoy.com/view/4sXGRM
}

vec2 hash2( float n )
{
    //return fract(cos(n)*vec2(10003.579, 37049.7));	//https://www.shadertoy.com/view/XtsSWs
    return fract(sin(vec2(n,n+1.0))*vec2(13.5453123,31.1459123));
}

float noise( vec3 x )
{
    vec3 f = fract(x);
    vec3 p = x - f; // this avoids the floor() but doesnt affect performance for me.
    f = f*f*(3.0-2.0*f);

    float n = p.x + p.y*157.0 + 113.0*p.z;
    return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
                   mix( hash(n+157.0), hash(n+158.0),f.x),f.y),
               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
                   mix( hash(n+270.0), hash(n+271.0),f.x),f.y),f.z);
}

float fbm( vec3 p, vec3 dir )
{
    float f;
    vec3 q = p - dir; f  = 0.50000*noise( q );
    q = q*2.02 - dir; f += 0.25000*noise( q );
    q = q*2.03 - dir; f += 0.12500*noise( q );
    q = q*2.01 - dir; f += 0.06250*noise( q );
    q = q*2.02 - dir; f += 0.03125*noise( q );

    return f;
}

// assign colour to the media
vec4 computeColour( float density, float radius, float bright )
{
    vec4 result = vec4( vec3(mix( 1.0, colour_low, density )), density );

    result *= mix( vertCol, colTop, min( (radius+0.5)*0.588, 1.0 ) ) * bright;

    return result;
}

// maps 3d position to density
float densityFn( in vec3 p, in float r, float t, in vec3 dir, float seed )
{
    //const float pi = 3.1415926;
    float den = ballness + (growth+ballness)*log(t)*r;
    den -= (2.5+ballness)*pow(t,fade)/r;

    if ( den <= -3. ) return -1.;

    float s = seed-(rolling_speed/(sin(min(t*3.,1.57))+rolling_init_damp));

    dir *= s;

    // invert space
    p = -grain*p/(dot(p,p)*downscale);

    // participating media
    float f = fbm( p, dir );
    //f=clamp(f,.1,.7);

    // add in noise with scale factor
    den += 4.0*f;

    return den;
}

// rad = radius of complete mult explosion (range 0 to 1)
// r = radius of the explosion ball that contributes the highest density
// rawDens = non-clamped density at the current maching location on the current ray
// foffset = factor for offset how much the offsetting should be applied. best to pass a time-based value.
void calcDens( in vec3 pos, out float rad, out float r, out float rawDens, in float t, in float foffset, out vec4 col, in float bright )
{
    float radiusFromExpCenter = length(pos);
    rad = radiusFromExpCenter / expRadius;

    r = 0.0;
    rawDens = 0.0;
    col = vec4(0.0);

    for ( int k = 0; k < mult_explosions; ++k )
    {
        float t0 = t - balls[k].delay;
        if ( t0 < 0.0 || t0 > 1.0 ) continue;

        vec3 p = pos - balls[k].offset * foffset;
        float radiusFromExpCenter0 = length(p);

        float r0 = downscale* radiusFromExpCenter0 / expRadius;
        if( r0 > 1.0 ) continue;

        float rawDens0 = densityFn( p, r0, t0, balls[k].dir, explosion_seed + 33.7*float(k) ) * density;

        // thin out the volume at the far extends of the bounding sphere to avoid
        // clipping with the bounding sphere
        rawDens0 *= 1.-smoothstep(thinout_smooth,1.,r0);

        float dens = clamp( rawDens0, 0.0, 1.0 );

        vec4 col0 = computeColour(dens, r0*(brightrad.x+brightrad.y*rawDens0), bright);	// also adds some variation to the radius

        col0.a *= (col0.a + .4) * (1. - r0*r0);

        // colour by alpha
        col0.rgb *= col0.a;

        col += col0;

        rawDens = max(rawDens, rawDens0);
    }

}

void contributecolour( vec4 col, vec4 sum )
{
    // alpha blend in contribution
    sum = sum + col*(1.0 - sum.a);
    sum.a+=0.15*col.a;
}

vec4 raymarch( vec3 rayo, vec3 rayd, vec2 expInter, float t, out float d )
{
    vec4 sum = vec4( 0.0 );

    float step = 1.5 / float(steps);

    vec3 pos = rayo + rayd * (expInter.x);	// no dither

    float march_pos = expInter.x;
    d = 4000.0;

    // t goes from 0 to 1 + mult delay. that is 0 to 1 is for one explosion ball. the delay for time distribution of the multiple explosion balls.
    // t_norm is 0 to 1 for the whole animation (incl mult delay).
    float t_norm = t / tmax;
    float smooth_t = sin(t_norm*2.1);	//sin(t*2.);

    //float bright = 6.1;
    float t1 = 1.0 - t_norm;	// we use t_norm instead of t so that final colour is reached at end of whole animation and not already at end of first explosion ball.

    float bright = brightness.x + brightness.y * t1*t1;

    for( int i=0; i<steps; i++ )
    {
        if( sum.a >= 0.98 ) { d = march_pos; break; }
        if ( march_pos >= expInter.y ) break;

        float rad, r, rawDens;
        vec4 col;
        calcDens( pos, rad, r, rawDens, t, smooth_t, col, bright );

        if ( rawDens <= 0.0 )
        {
            float s = step * 2.0;
            pos += rayd * s;
            march_pos += s;
            continue;
        }

        contributecolour( col, sum );

        // take larger steps through low densities.
        // something like using the density function as a SDF.
        float stepMult = 1.0 + (1.-clamp(rawDens+col.a,0.,1.));
        // step along ray
        pos += rayd * step * stepMult;
        march_pos += step * stepMult;

        //pos += rayd * step;
    }

    return clamp( sum, 0.0, 1.0 );
}

// iq's sphere intersection, but here fixed for a sphere at (0,0,0)
vec2 iSphere(vec3 ro, vec3 rd, float rad)
{
    float b = dot(ro, rd);					//=dot(oc, rd);
    float c = dot(ro, ro) - rad * rad;		//=dot(oc, oc) - sph.w * sph.w; //sph.w is radius
    float h = b*b - c; // delta
    if(h < 0.0)
        return vec2(-1.0);
    //h = sqrt(h);
    h *= 0.5;		// just some rough approximation to prevent sqrt.
    return vec2(-b-h, -b+h);
}

vec3 computePixelRay( vec2 p, out vec3 cameraPos )
{
    // camera orbits around explosion

    float camRadius = CAM_DIST;
    // use mouse x coord
    float a = iGlobalTime*CAM_ROTATION_SPEED;
    float b = CAM_TILT * sin(a * .014);

    float phi = b * 3.14;
    float camRadiusProjectedDown = camRadius * cos(phi);
    float theta = -(a)/80.;
    float xoff = camRadiusProjectedDown * cos(theta);
    float zoff = camRadiusProjectedDown * sin(theta);
    float yoff = camRadius * sin(phi);
    cameraPos = vec3(xoff,yoff,zoff);

    // camera target
    vec3 target = vec3(0.);

    // camera frame
    vec3 fo = normalize(target-cameraPos);
    vec3 ri = normalize(vec3(fo.z, 0., -fo.x ));
    vec3 up = normalize(cross(fo,ri));

    // multiplier to emulate a fov control
    float fov = .5;

    // ray direction
    vec3 rayDir = normalize(fo + fov*p.x*ri + fov*p.y*up);

    return rayDir;
}

void setup()
{
    // first expl ball always centered looking up
    balls[0] = Ball(
                vec3(0.),
                vec3(0.,.7,0.),		// not normalized so that expl ball 0 rolls somewhat slower
                0.0
                );

    float pseed = variation_seed;
    float tseed = delay_seed;
    float maxdelay = 0.0;
    for ( int k = 1; k < mult_explosions; ++k )
    {
        float pseed = variation_seed + 3. * float(k-1);
        float tseed = delay_seed + 3. * float(k-1);
        vec2 phi = hash2(pseed) * vec2(2.*pi, pi*ball_spread);
        vec2 tilted = vec2( sin(phi.y), cos(phi.y) );
        vec3 rotated = vec3( tilted.x * cos(phi.x), tilted.y, tilted.x * sin(phi.x) );
        balls[k].offset = 0.7 * rotated;
        balls[k].dir = normalize( balls[k].offset );
        balls[k].delay = delay_range * hash(tseed);
        pseed += 3.;
        tseed += 3.;
        maxdelay = max(maxdelay, balls[k].delay);
    }

    if ( maxdelay > 0.0 )
    {
        // Now stretch the ball explosion delays to the maximum allowed range.
        // So that the last ball starts with a delay of exactly delay_range and thus we do not waste any final time with just empty space.
        for ( int k = 0; k < mult_explosions; ++k )
            balls[k].delay *= delay_range / maxdelay;
    }
}

void main()
{
    fragColour = vec4(0.0);
    return;
    float t = getTime();

    // some global initialization.
    setup();

    // get aspect corrected normalized pixel coordinate
    vec2 q = UV;
    vec2 p = -1.0 + 2.0*q;
    vec3 rayDir, cameraPos;

    rayDir = computePixelRay( p, cameraPos );

    vec4 col = vec4(0.);
    float d = 4000.0;

    // does pixel ray intersect with exp bounding sphere?
    vec2 boundingSphereInter = iSphere( cameraPos, rayDir, expRadius );
    if( boundingSphereInter.x > 0. )
    {
        // yes, cast ray
        col = raymarch( cameraPos, rayDir, boundingSphereInter, t, d );
    }
    col.xyz = col.xyz * col.xyz * ( 1.0 + contrast * (1.0 - col.xyz) );

    fragColour = col;
    fragColour.a *= vertCol.a;
    //fragColour = vec4(1.0, 0.0, 0.0, 1.0);
}


