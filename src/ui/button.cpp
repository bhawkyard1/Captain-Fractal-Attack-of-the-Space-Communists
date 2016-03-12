#include <array>
#include <string>

#include "ui/button.hpp"

namespace ui
{

button::button(
        const std::string _txt,
        const std::array<int, 8> _b_col,
        const std::array<int, 8> _t_col,
        const vec2 _pos,
        const vec2 _dim
        )
{
    m_dark = false;
    m_selected = false;

    m_initLabel = _txt;
    m_label = _txt;

    for(size_t i = 0; i < 8; i++)
    {
        m_col[i] = _b_col[i];
    }
    for(size_t i = 0; i < 8; i++)
    {
        m_tcol[i] = _t_col[i];
    }

    m_pos.m_x = _pos.m_x;
    m_pos.m_y = _pos.m_y;
    m_dim.m_x = _dim.m_x;
    m_dim.m_y = _dim.m_y;

    m_initCost = 0;
    m_cost = 0;
}

button::button(
        const std::string _txt,
        const std::array<int, 8> _b_col,
        const std::array<int, 8> _t_col,
        const vec2 _pos,
        const vec2 _dim,
        const int _pcost
        )
{
    m_dark = true;
    m_selected = false;

    m_initLabel = _txt;
    m_label = _txt;

    for(size_t i = 0; i < 8; i++)
    {
        m_col[i] = _b_col[i];
    }
    for(size_t i = 0; i < 8; i++)
    {
        m_tcol[i] = _t_col[i];
    }

    m_pos.m_x = _pos.m_x;
    m_pos.m_y = _pos.m_y;
    m_dim.m_x = _dim.m_x;
    m_dim.m_y = _dim.m_y;

    m_initCost = _pcost;
    m_cost = _pcost;
}

void button::update(int _pts)
{
    if(_pts > m_cost) m_dark = false;
    else m_dark = true;
}

void button::updateText(std::string _text)
{
    m_label = _text;
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

}
