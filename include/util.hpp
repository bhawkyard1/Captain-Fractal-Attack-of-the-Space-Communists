#ifndef UTIL_HPP
#define UTIL_HPP

#include <math.h>
#include <limits>
#include <algorithm>
#include <sstream>
#include <time.h>

#include "SDL2/SDL_ttf.h"

#include "vectors.hpp"

#define F_MAX std::numeric_limits<float>::max()
#define F_INF std::numeric_limits<float>::infinity()

#define I_MAX std::numeric_limits<int>::max()
#define I_INF std::numeric_limits<int>::max()

void toOctant(int*,int*,int);

float toDeg(float rad);
float toRad(float deg);

float computeAngle(vec2 v);

vec2 computeVector(float angle);

void equateArrays(int arr1[], int arr2[], int ind);

template<typename tt>
tt clamp(tt v, tt m, tt M)
{
    if(v < m) return m;
    else if(v > M) return M;
    return v;
}

template<typename tt>
tt clampdblsw(tt val, tt lim1, tt lim2)
{
  tt min, max;

  if(lim1 < lim2)
  {
    min = lim1;
    max = lim2;
  }
  else if(lim2 > lim1)
  {
    min = lim2;
    max = lim1;
  }
  else
  {
    return lim1;
  }

  if(val < min)
  {
    return min;
  }
  else if(val > max)
  {
    return max;
  }
  return val;
}

template<typename tt>
tt clampRoll(tt val, tt min, tt max)
{
  if(val < min)
  {
    return max;
  }
  else if(val > max)
  {
    return min;
  }
  return val;
}

float shortestAngle(float ang1, float ang2);

int factorial(int val);

double dotProd(double x0, double y0, double x1, double y1);

//DELETE THESE

void drawCircle(SDL_Renderer *renderer, int center[], int radius);

void drawLineGr(SDL_Renderer *renderer, vec2 start, vec2 end, float scol[], float ecol[]);

float sqr(float arg);

float randFloat(int low, int high);

float randFloat(float low, float high);

template<class t>
t randNum(t low, t high)
{
  return static_cast <t> (rand()) / static_cast <t> (RAND_MAX/(high-low))+low;
}

double diffClock(clock_t clock1, clock_t clock2);

SDL_Texture* renderText(const std::string &message, TTF_Font * font, SDL_Color color, SDL_Renderer *renderer, int len);

bool pointInRect(vec2 p, SDL_Rect * r);
bool pointInRect(vec2 p, vec2 r_pos, vec2 r_dim);

bool strToBool(std::string str);

vec2 randVec(float m, float M);

vec2 randVec(float f);

vec2 randVec(vec2 min, vec2 max);

void toOctant(int * x, int * y, int octant);

std::vector<std::string> split(std::string str, char delim);

template<class t>
void swapnpop(std::vector<t> * vec, int i)
{
  iter_swap( vec->begin() + i, vec->end() - 1 );
  vec->pop_back();
}
#endif
