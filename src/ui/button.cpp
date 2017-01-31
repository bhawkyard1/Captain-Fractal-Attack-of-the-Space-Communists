#include <array>
#include <string>

#include "ui/button.hpp"
#include "shapes.hpp"

button::button()
{

}

button::button(const std::string _txt,
               const std::array<int, 4> _bcol,
               const std::array<int, 4> _tcol,
               const vec2 _pos,
               const vec2 _dim)
{
    m_smul = 1.0f;
    m_state = BUTTON_STATE_DEFAULT;
    m_selected = false;

    m_initLabel = _txt;
    m_label = _txt;

    m_col = _bcol;

    m_tcol = _tcol;

    m_pos = _pos;
    m_start = _pos;
    m_end = _pos;
    m_dim = _dim;

    m_initCost = -1;
    m_cost = -1;

    m_dcol = col255to1(m_col);
    m_interpolate = false;
}

button::button(const std::string _txt,
               const std::array<int, 4> _bcol,
               const std::array<int, 4> _tcol,
               const vec2 _pos,
               const vec2 _dim,
               const float _smul)
{
    m_smul = _smul;
    m_state = BUTTON_STATE_DEFAULT;
    m_selected = false;

    m_initLabel = _txt;
    m_label = _txt;

    m_col = _bcol;
    m_tcol = _tcol;

    m_pos = _pos;
    m_start = _pos;
    m_end = _pos;
    m_dim = _dim;

    m_initCost = -1;
    m_cost = -1;

    m_dcol = col255to1(m_col);
    m_interpolate = false;
}

button::button(
        const std::string _txt,
        const std::array<int, 4> _bcol,
        const std::array<int, 4> _tcol,
        const vec2 _pos,
        const vec2 _dim,
        const int _pcost
        )
{
    m_smul = 1.0f;
    m_state = BUTTON_STATE_DISABLED;
    m_selected = false;

    m_initLabel = _txt;
    m_label = _txt;

    m_col = _bcol;
    m_tcol = _tcol;

    m_pos = _pos;
    m_start = _pos;
    m_end = _pos;
    m_dim = _dim;

    m_initCost = _pcost;
    m_cost = _pcost;

    m_dcol = col255to1(m_col);
    m_interpolate = false;
}

void button::update(const int _pts, const vec2 _mouse, const float _interp)
{
    for(int i = 0; i < 4; ++i)
        m_dcol[i] = static_cast<float>(m_col[i]);

    if(m_interpolate)
        m_pos = _interp * m_end + (1.0f - _interp) * m_start;

    //Enable if we have enough points, disable if we don't OR it is a non-cost button and it is already disabled.
    if(m_cost == -1 or _pts >= m_cost)
    {
        m_state = BUTTON_STATE_DEFAULT;

        if(pointInRect(_mouse, m_pos, m_dim))
        {
            m_state = BUTTON_STATE_OVER;
            for(auto & i : m_dcol) i += 20.0f;
        }
    }
    else
    {
        m_state = BUTTON_STATE_DISABLED;
        for(auto & i : m_dcol) i /= 2.0f;
    }

    if(m_selected)
    {
        for(auto &i : m_dcol) i += 20.0f;
    }
    else
    {
        for(auto &i : m_dcol) i -= 20.0f;
    }

    for(auto & i : m_dcol) i /= 255.0f;
}

void button::updateText(std::string _str)
{
    m_label = _str;
}

void button::select()
{
    if(m_selected)
    {
        m_selected = false;
    }
    else
    {
        m_selected = true;
    }
}

void button::reset()
{
    m_label = m_initLabel;
    m_cost = m_initCost;
}

void button::setDark(bool b)
{
    for(int i = 0; i < 4; ++i)
        m_dcol[i] = static_cast<float>(m_col[i]);

    if(b)
        m_state = BUTTON_STATE_DISABLED;
    else
    {
        m_state = BUTTON_STATE_DEFAULT;
        for(auto & i : m_dcol)
            i /= 2.0f;
    }

    for(auto & i : m_dcol) i /= 255.0f;
}
