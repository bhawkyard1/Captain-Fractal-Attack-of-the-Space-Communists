#include "math/mat2.hpp"

mat2 operator +(const mat2 &_lhs, const mat2 &_rhs)
{
    mat2 m;
    for(int i = 0; i < 4; ++i)
        m.set( i, _lhs.get(i) + _rhs.get(i) );
    return m;
}

mat2 operator -(const mat2 &_lhs, const mat2 &_rhs)
{
    mat2 m;
    for(int i = 0; i < 4; ++i)
        m.set( i, _lhs.get(i) - _rhs.get(i) );
    return m;
}

mat2 operator *(const mat2 &_lhs, const mat2 &_rhs)
{
    mat2 m;
    for(int i = 0; i < 2; ++i)
        for(int j = 0; j < 2; ++j)
            for(int k = 0; k < 2; k++)
            {
                m.incr(i, j, _lhs.get(k, j) * _rhs.get( i, k ));
            }
    return m;
}

mat2 operator *(const mat2 &_lhs, const float &_rhs)
{
    mat2 m;
    for(int i = 0; i < 4; ++i)
        m.set( i, _lhs.get(i) * _rhs );
    return m;
}

mat2 operator *(const float &_lhs, const mat2 &_rhs)
{
    mat2 m;
    for(int i = 0; i < 4; ++i)
        m.set( i, _lhs * _rhs.get(i) );
    return m;
}

mat2 operator /(const mat2 &_lhs, const float &_rhs)
{
    mat2 m;
    for(int i = 0; i < 4; ++i)
        m.set( i, _lhs.get(i) / _rhs );
    return m;
}

bool operator ==(const mat2 &_lhs, const mat2 &_rhs)
{
    for(int i = 0; i < 4; ++i)
        if(_lhs.get(i) != _rhs.get(i))
            return false;
    return true;
}

float determinant(const mat2 &_m)
{
    return _m.get(0, 0) * _m.get(1, 1) -
            _m.get(1, 0) * _m.get(0, 1);
}
