#include <array>
#include <string>

#include "ui/button.hpp"
#include "shapes.hpp"

button::button()
{

}

button::button(const std::string _txt,
               const std::array<int, 4> _b_col,
               const std::array<int, 4> _t_col,
               const vec2 _pos,
               const vec2 _dim)
{
    m_smul = 1.0f;
    m_state = BUTTON_STATE_DEFAULT;
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

    m_initCost = -1;
    m_cost = -1;

    m_dcol = {m_col[4] / 255.0f, m_col[5] / 255.0f, m_col[6] / 255.0f, m_col[7] / 255.0f};
}

button::button(const std::string _txt,
               const std::array<int, 4> _b_col,
               const std::array<int, 4> _t_col,
               const vec2 _pos,
               const vec2 _dim,
               const float _smul)
{
    m_smul = _smul;
    m_state = BUTTON_STATE_DEFAULT;
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

    m_initCost = -1;
    m_cost = -1;

    m_dcol = {m_col[4] / 255.0f, m_col[5] / 255.0f, m_col[6] / 255.0f, m_col[7] / 255.0f};
}

button::button(
        const std::string _txt,
        const std::array<int, 4> _label,
        const std::array<int, 4> _pcol,
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

    for(size_t i = 0; i < 8; i++)
    {
        m_col[i] = _label[i];
    }
    for(size_t i = 0; i < 8; i++)
    {
        m_tcol[i] = _pcol[i];
    }

    m_pos.m_x = _pos.m_x;
    m_pos.m_y = _pos.m_y;
    m_dim.m_x = _dim.m_x;
    m_dim.m_y = _dim.m_y;

    m_initCost = _pcost;
    m_cost = _pcost;

    m_dcol = {m_col[0] / 255.0f, m_col[1] / 255.0f, m_col[2] / 255.0f, m_col[3] / 255.0f};
}

void button::update(const int _pts, const vec2 _mouse)
{
    for(int i = 0; i < 4; ++i) m_dcol[i] = static_cast<float>(m_col[i]);

    if(_pts >= m_cost)
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
  if(b) m_state = BUTTON_STATE_DISABLED;
  else m_state = BUTTON_STATE_DEFAULT;
}
