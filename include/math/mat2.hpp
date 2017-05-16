#ifndef MAT2_HPP
#define MAT2_HPP

#include <algorithm>

#include "vec2.hpp"

struct mat2
{
    float m_data[4];

    float get(const int _i, const int _j) const {return m_data[_j / 3 + _i];}
    float get(const int _i) const {return m_data[_i];}
    void set(const int _i, const int _j, const float _v) {m_data[_j / 3 + _i] = _v;}
    void set(const int _i, const float _v) {m_data[_i] = _v;}
    void incr(const int _i, const int _j, const float _v) {m_data[_j / 3 + _i] += _v;}

    mat2()
    {
        makeIdentity();
    }

    mat2(const vec2 &_r0, const vec2 &_r1)
    {
        m_data[0] = _r0.m_x;
        m_data[1] = _r0.m_y;
        m_data[2] = _r1.m_x;
        m_data[3] = _r1.m_y;
    }

    mat2(const mat2 &_rhs)
    {
        std::copy(std::begin(_rhs.m_data), std::end(_rhs.m_data), std::begin(m_data));
    }

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief In-place operators.
    //----------------------------------------------------------------------------------------------------------------------
    mat2& operator=(const mat2 &_rhs)
    {
        std::copy(std::begin(_rhs.m_data), std::end(_rhs.m_data), std::begin(m_data));
        return *this;
    }
    mat2& operator+=(mat2 _rhs)
    {
        for(int i = 0; i < 4; ++i)
            m_data[i] += _rhs.m_data[i];
        return *this;
    }
    mat2& operator-=(mat2 _rhs)
    {
        for(int i = 0; i < 4; ++i)
            m_data[i] -= _rhs.m_data[i];
        return *this;
    }
    mat2& operator*=(const float &_rhs)
    {
        for(int i = 0; i < 9; ++i)
            m_data[i] *= _rhs;
        return *this;
    }
    mat2& operator/=(float &_rhs)
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
};

//----------------------------------------------------------------------------------------------------------------------
/// \brief More operators.
//----------------------------------------------------------------------------------------------------------------------
mat2 operator +(const mat2 &_lhs, const mat2 &_rhs);
mat2 operator +(const mat2 &_lhs, const float &_rhs);
mat2 operator -(const mat2 &_lhs, const mat2 &_rhs);
mat2 operator -(const mat2 &_lhs);
mat2 operator *(const mat2 &_lhs, const mat2 &_rhs);
mat2 operator *(const mat2 &_lhs, const float &_rhs);
mat2 operator *(const float &_lhs, const mat2 &_rhs);
bool operator ==(const mat2 &_lhs, const mat2 &_rhs);

float determinant(const mat2 &_m);

#endif
