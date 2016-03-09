#include "ui/button.hpp"

button::button(std::string _txt, std::array<int, 8> _b_col, std::array<int, 8> _t_col, vec2 _pos, vec2 _dim)
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

    m_pos.x = _pos.x;
    m_pos.y = _pos.y;
    m_dim.x = _dim.x;
    m_dim.y = _dim.y;

    m_initCost = 0;
    m_cost = 0;
}

button::button(std::string _txt, std::array<int, 8> _b_col, std::array<int, 8> _t_col, vec2 _pos, vec2 _dim, int _pcost)
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

    m_pos.x = _pos.x;
    m_pos.y = _pos.y;
    m_dim.x = _dim.x;
    m_dim.y = _dim.y;

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
