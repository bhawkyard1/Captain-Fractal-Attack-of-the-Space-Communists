#ifndef MAT3_HPP
#define MAT3_HPP

#include <algorithm>

#include "mat2.hpp"
#include "vec3.hpp"

struct mat3
{
    // 0 1 2
    // 3 4 5
    // 6 7 8
    float m_data[9];

    float get(const int _i, const int _j) const {return m_data[_j * 3 + _i];}
    float get(const int _i) const {return m_data[_i];}
    void set(const int _i, const int _j, const float _v) {m_data[_j * 3 + _i] = _v;}
    void set(const int _i, const float _v) {m_data[_i] = _v;}
    void incr(const int _i, const int _j, const float _v) {m_data[_j * 3 + _i] += _v;}

    mat3()
    {
        makeIdentity();
    }

    mat3(const vec3 &_r0, const vec3 &_r1, const vec3 &_r2)
    {
        m_data[0] = _r0.m_x;
        m_data[1] = _r0.m_y;
        m_data[2] = _r0.m_z;
        m_data[3] = _r1.m_x;
        m_data[4] = _r1.m_y;
        m_data[5] = _r1.m_z;
        m_data[6] = _r2.m_x;
        m_data[7] = _r2.m_y;
        m_data[8] = _r2.m_z;
    }

    mat3(const mat3 &_rhs)
    {
        std::copy(std::begin(_rhs.m_data), std::end(_rhs.m_data), std::begin(m_data));
    }

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief In-place operators.
    //----------------------------------------------------------------------------------------------------------------------
    mat3& operator=(const mat3 &_rhs)
    {
        std::copy(std::begin(_rhs.m_data), std::end(_rhs.m_data), std::begin(m_data));
        return *this;
    }
    mat3& operator+=(mat3 _rhs)
    {
        for(int i = 0; i < 9; ++i)
            m_data[i] += _rhs.m_data[i];
        return *this;
    }
    mat3& operator-=(mat3 _rhs)
    {
        for(int i = 0; i < 9; ++i)
            m_data[i] -= _rhs.m_data[i];
        return *this;
    }
    mat3& operator*=(const float &_rhs)
    {
        for(int i = 0; i < 9; ++i)
            m_data[i] *= _rhs;
        return *this;
    }
    mat3& operator/=(float &_rhs)
    {
        for(int i = 0; i < 9; ++i)
            m_data[i] /= _rhs;
        return *this;
    }

    void makeIdentity()
    {
        std::fill(std::begin(m_data), std::end(m_data), 0.0f);
        for(int i = 0; i < 3; ++i)
            set(i, i, 1.0f);
    }

    mat2 getMinor(const int _i, const int _j) const
    {
        int s = 0;
        mat2 m;
        for(int i = 0; i < 3; ++i)
            for(int j = 0; j < 3; ++j)
                if(i != _i and j != _j)
                {
                    //Set m at s to be equal to the value of this matrix at i,j.
                    m.set(s, get(i, j));
                    //Increment s.
                    s++;
                }
        return m;
    }

    void transpose()
    {
        std::swap( m_data[0], m_data[8] );
        std::swap( m_data[1], m_data[5] );
        std::swap( m_data[3], m_data[7] );
    }
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief More operators.
//----------------------------------------------------------------------------------------------------------------------
mat3 operator +(const mat3 &_lhs, const mat3 &_rhs);
mat3 operator +(const mat3 &_lhs, const float &_rhs);
mat3 operator -(const mat3 &_lhs, const mat3 &_rhs);
mat3 operator -(const mat3 &_lhs);
mat3 operator *(const mat3 &_lhs, const mat3 &_rhs);
mat3 operator *(const mat3 &_lhs, const float &_rhs);
mat3 operator *(const float &_lhs, const mat3 &_rhs);
bool operator ==(const mat3 &_lhs, const mat3 &_rhs);

mat3 transpose(const mat3 &_m);
float determinant(const mat3 &_m);
mat3 inverse(const mat3 &_m);


#endif
