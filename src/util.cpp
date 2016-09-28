#include "common.hpp"
#include "util.hpp"
#include <iostream>

#ifdef _WIN32
#include <Windows.h>
#endif

float computeAngle(vec2 _v)
{
    return -deg(atan2(_v.m_x,_v.m_y));
}

float shortestAngle(float _ang1, float _ang2)
{
    return fmod(_ang1 - _ang2 + 180, 360) - 180;
}

double dotProd(double x0, double y0, double x1, double y1)
{
    return acos((x0*x1+y0*y1)/(sqrt(x0*x0+y0*y0)*sqrt(x1*x1+y1*y1)));
}

float sqr(float _arg)
{
    return _arg*_arg;
}

int gcd(int _a, int _b)
{
    if(_b == 0)
        return _a;
    return gcd(_b, _a % _b);
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

bool pointInRect(vec2 _point, vec2 _pos, vec2 _dim)
{
    return _point.m_x > _pos.m_x
            and _point.m_x < _pos.m_x + _dim.m_x
            and _point.m_y > _pos.m_y
            and _point.m_y < _pos.m_y + _dim.m_y;
}

bool pointInBox(vec3 _point, vec3 _start, vec3 _end)
{
    return inRange(_point.m_x, _start.m_x, _end.m_x)
            and inRange(_point.m_y, _start.m_y, _end.m_y)
            and inRange(_point.m_z, _start.m_z, _end.m_z);
}

vec2 randVec2(float _m, float _M)
{
    vec2 v = {randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f)};
    float vsum = sqr(v.m_x) + sqr(v.m_y);
    v *= fastInvSqrt(vsum);
    return v * randNum(_m, _M);
}

vec2 randVec2(float _f)
{
    vec2 v = {randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f)};
    float vsum = sqr(v.m_x) + sqr(v.m_y);
    v *= fastInvSqrt(vsum) * randNum(-_f, _f);
    return v;
}

vec2 randVec2(vec2 _min, vec2 _max)
{
    return {randNum(_min.m_x, _max.m_x), randNum(_min.m_y, _max.m_y)};
}

vec3 randVec3(float m, float M)
{
    vec3 v = {randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f)};
    float vsum = sqr(v.m_x) + sqr(v.m_y) + sqr(v.m_z);
    v *= fastInvSqrt(vsum);
    return v * randNum(m, M);
}

vec3 randVec3(float _f)
{
    vec3 v = {randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f), randNum(-1.0f, 1.0f)};
    float vsum = sqr(v.m_x) + sqr(v.m_y) + sqr(v.m_z);
    v *= fastInvSqrt(vsum) * randNum(-_f, _f);
    return v;
}

vec3 randVec3OnLine(vec3 _min, vec3 _max)
{
    return _min + (_max - _min) * randNum(0.0f, 1.0f);
}

vec3 randVec3(vec3 min, vec3 max)
{
    return {randNum(min.m_x, max.m_x), randNum(min.m_y, max.m_y), randNum(min.m_z, max.m_z)};
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

std::array<float, 4> col255to1(const std::array<float, 4> _col)
{
    return {_col[0] / 255.0f, _col[1] / 255.0f, _col[2] / 255.0f, _col[3] / 255.0f};
}

std::array<float, 4> col255to1(const std::array<int, 4> _col)
{
    return {_col[0] / 255.0f, _col[1] / 255.0f, _col[2] / 255.0f, _col[3] / 255.0f};
}

std::array<float, 4> col1to255(const std::array<float, 4> _col)
{
    return {_col[0] * 255.0f, _col[1] * 255.0f, _col[2] * 255.0f, _col[3] * 255.0f};
}

std::array<float, 4> col1to255(const std::array<int, 4> _col)
{
    return {_col[0] * 255.0f, _col[1] * 255.0f, _col[2] * 255.0f, _col[3] * 255.0f};
}

vec2 getMousePos()
{
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&x, &y);
    return vec2(static_cast<float>(x), static_cast<float>(y));
}

vec2 toWorldSpace(vec2 _in)
{
    _in -= g_HALFWIN;
    _in /= g_ZOOM_LEVEL;
    return _in;
}

vec2 toScreenSpace(vec2 _in)
{
    _in += g_HALFWIN;
    return _in;
}

std::istream& getlineSafe(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

void debug(const std::string _msg)
{
    if(PRINT_DEBUG_MESSAGES) std::cout << _msg << std::endl;
}

void clearTerminal()
{
#ifdef _WIN32
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    FillConsoleOutputAttribute(console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE, screen.dwSize.X * screen.dwSize.Y, topLeft, &written);
    SetConsoleCursorPosition(console, topLeft);
#endif
#ifdef __linux__
    return;
#endif
}

std::vector<SDL_Event> getEvents()
{
    std::vector<SDL_Event> ret;
    SDL_Event event;
    while( SDL_PollEvent( &event ) )
    {
        ret.push_back(event);
    }
    return ret;
}

bool prob(const int _r)
{
    return !(rand() % _r);
}

