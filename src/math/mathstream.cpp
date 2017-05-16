#include "math/mathstream.hpp"

std::ostream& operator<<(std::ostream& os, const vec2 &_v)
{
    os << "[ " << _v.m_x << ", " << _v.m_y << " ]\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const vec3 &_v)
{
    os << "[ " << _v.m_x << ", " << _v.m_y << ", " << _v.m_z << " ]\n";
    return os;
}

std::ostream& operator<<(std::ostream& os, const mat2 &_v)
{
    for(int i = 0; i < 2; ++i)
    {
        for(int j = 0; j < 2; ++j)
        {
            os << _v.get(i, j) << " ";
        }
        os << '\n';
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const mat3 &_v)
{
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < 3; ++j)
        {
            os << _v.get(i, j) << " ";
        }
        os << '\n';
    }
    return os;
}
