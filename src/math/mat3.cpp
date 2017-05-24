#include "math/mat3.hpp"

mat3 operator +(const mat3 &_lhs, const mat3 &_rhs)
{
    mat3 m;
    for(int i = 0; i < 9; ++i)
        m.set( i, _lhs.get(i) + _rhs.get(i) );
    return m;
}

mat3 operator -(const mat3 &_lhs, const mat3 &_rhs)
{
    mat3 m;
    for(int i = 0; i < 9; ++i)
        m.set( i, _lhs.get(i) - _rhs.get(i) );
    return m;
}

mat3 operator *(const mat3 &_lhs, const mat3 &_rhs)
{
    mat3 m;
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
        {
            vec3 l (
                        _lhs.get( 0, j ),
                        _lhs.get( 1, j ),
                        _lhs.get( 2, j )
                        );
            vec3 r (
                        _rhs.get( i, 0 ),
                        _rhs.get( i, 1 ),
                        _rhs.get( i, 2 )
                        );
            m.set(i, j, dot(l, r));
        }
    return m;
}

mat3 operator *(const mat3 &_lhs, const float &_rhs)
{
    mat3 m;
    for(int i = 0; i < 9; ++i)
        m.set( i, _lhs.get(i) * _rhs );
    return m;
}

mat3 operator *(const float &_lhs, const mat3 &_rhs)
{
    mat3 m;
    for(int i = 0; i < 9; ++i)
        m.set( i, _lhs * _rhs.get(i) );
    return m;
}

mat3 operator /(const mat3 &_lhs, const float &_rhs)
{
    mat3 m;
    for(int i = 0; i < 9; ++i)
        m.set( i, _lhs.get(i) / _rhs );
    return m;
}

bool operator ==(const mat3 &_lhs, const mat3 &_rhs)
{
    for(int i = 0; i < 9; ++i)
        if(_lhs.get(i) != _rhs.get(i))
            return false;
    return true;
}

float determinant(const mat3 &_m)
{
    float a = _m.get(0,0);
    float b = _m.get(1,0);
    float c = _m.get(2,0);
    float d = _m.get(0,1);
    float e = _m.get(1,1);
    float f = _m.get(2,1);
    float g = _m.get(0,2);
    float h = _m.get(1,2);
    float i = _m.get(2,2);

    return a * (e*i - f*h) -
            b * (d*i - f*g) +
            c * (d*h - e*g);
}

mat3 transpose(const mat3 &_m)
{
    mat3 r;
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            r.set(j, i, _m.get(i, j));
		return r;
}

mat3 inverse(const mat3 &_m)
{
    mat3 r;
    float det = determinant( _m );
    for(int i = 0; i < 3; ++i)
        for(int j = 0; j < 3; ++j)
            r.set(i, j,
                  determinant(
                      _m.getMinor( i, j )
                      )
                  );
    for(int i = 0; i < 9; ++i)
        if(i % 2 == 0)
            r.set(i, r.get(i) * -1.0f );
    r.transpose();

    return det * r;
}
