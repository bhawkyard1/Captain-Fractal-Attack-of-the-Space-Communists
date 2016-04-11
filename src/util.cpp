#include "util.hpp"

float toDeg(float rad)
{
  return (rad * 180) / 3.14159;
}

float toRad(float deg)
{
  return (deg * 3.14159) / 180;
}

float computeAngle(vec2 v)
{
  return -toDeg(atan2(v.m_x,v.m_y));
}

vec2 computeVector(float angle)
{
  return { static_cast<float>(-cos(toRad(-angle))), static_cast<float>(sin(toRad(-angle))) };
}

void equateArrays(int arr1[], int arr2[], int ind)
{
  for(int i = 0; i < ind; i++)
  {
    arr2[ind] = arr1[ind];
  }
}


float shortestAngle(float ang1, float ang2)
{
  return fmod(ang1 - ang2 + 180, 360) - 180;
}

int factorial(int val)
{
  int returnVal = 0;

  for(int i = val; i > 0; i--)
  {
    returnVal += i;
  }
  return returnVal;
}

double dotProd(double x0, double y0, double x1, double y1)
{
  return acos((x0*x1+y0*y1)/(sqrt(x0*x0+y0*y0)*sqrt(x1*x1+y1*y1)));
}


float sqr(float arg)
{
  return arg*arg;
}

float fastInvSqrt(float _val)
{
    //Fast inverse sqrt, discovered in quake 3 source, attributed to Greg Walsh.
    float half = 0.5f * _val;
    int i = *(int*)&_val; // get bits for floating value
    i = 0x5f3759df - (i >> 1); // gives initial guess y0
    _val = *(float*)&i; // convert bits back to float
    _val = _val * (1.5f - half * _val * _val); // Newton step, repeating increases accuracy
    return _val;
}

float randFloat(int low, int high)
{
  float flow = static_cast<float>(low);
  float fhigh = static_cast<float>(high);
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(fhigh-flow))+flow;
}

float randFloat(float low, float high)
{
  return static_cast <float> (rand()) / static_cast <float> (RAND_MAX/(high-low))+low;
}

double diffClock(clock_t clock1, clock_t clock2)
{
  double diffticks = clock1-clock2;
  double diffs = diffticks/CLOCKS_PER_SEC;
  return diffs;
}

bool pointInRect(vec2 p, SDL_Rect * r)
{
  if(p.m_x > r->x and p.m_x < r->x + r->w and p.m_y > r->y and p.m_y < r->y + r->h) return true;
  return false;
}

bool pointInRect(vec2 p, vec2 r_pos, vec2 r_dim)
{
  if(p.m_x > r_pos.m_x and p.m_x < r_pos.m_x + r_dim.m_x and p.m_y > r_pos.m_y and p.m_y < r_pos.m_y + r_dim.m_y) return true;
  return false;
}

bool strToBool(std::string str)
{
  if(str == "true") return true;
  return false;
}

vec2 randVec(float m, float M)
{
  float a = randFloat(0.0f, 360.0f);
  vec2 v = {static_cast<float>(cos(a)), static_cast<float>(sin(a))};
  return v * randFloat(m, M);
}

vec2 randVec(float f)
{
  float a = randFloat(0.0f, 360.0f);
  vec2 v = {static_cast<float>(cos(a)), static_cast<float>(sin(a))};
  return v * randFloat(0.0f, f);
}

vec2 randVec(vec2 min, vec2 max)
{
  return {randFloat(min.m_x, max.m_x), randFloat(min.m_y, max.m_y)};
}

void toOctant(int * x, int * y, int octant)
{
  switch(octant)
  {
    int t;
    case 0:
      break;
    case 1:
      t = *x;
      *x = *y;
      *y = t;
      break;
    case 2:
      t = *x;
      *x = -*y;
      *y = t;
      break;
    case 3:
      *x = -*x;
      break;
    case 4:
      *x = -*x;
      *y = -*y;
      break;
    case 5:
      t = *x;
      *x = -*y;
      *y = -t;
      break;
    case 6:
      t = *x;
      *x = *y;
      *y = -t;
      break;
    case 7:
      *y = -*y;
      break;
  }
}

std::vector<std::string> split(std::string _str, char _delim)
{
  std::vector<std::string> ret;

  std::stringstream ss(_str);
  std::string sub;

  while( getline( ss, sub, _delim ) )
  {
    ret.push_back(sub);
  }

  return ret;
}

vec2 front(float _ang)
{
    _ang -= rad(90.0f);
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

vec2 back(float _ang)
{
    _ang += rad(90.0f);
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

vec2 left(float _ang)
{
    _ang -= rad(180.0f);
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

vec2 right(float _ang)
{
    return {static_cast<float>(cos(_ang)), static_cast<float>(sin(_ang))};
}

std::array<float, 4> col255to1(std::array<float, 4> _col)
{
  return {_col[0] / 255.0f, _col[1] / 255.0f, _col[2] / 255.0f, _col[3] / 255.0f};
}

std::array<float, 4> col255to1(std::array<int, 4> _col)
{
  return {_col[0] / 255.0f, _col[1] / 255.0f, _col[2] / 255.0f, _col[3] / 255.0f};
}
