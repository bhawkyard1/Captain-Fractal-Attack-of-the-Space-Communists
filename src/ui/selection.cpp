#include "util.hpp"
#include "ui/selection.hpp"

selection::selection()
{
    m_visible = true;
    m_interpolant = 0.0f;
    m_saveSelected = false;
}

selection::selection(selectionType _type)
{
    m_visible = true;
    m_interpolant = 0.0f;
    m_saveSelected = false;
}

bool selection::click(vec2 _p)
{
    if(!m_visible) return false;
    bool any = false;
    for(auto &i : m_buttons)
    {
        if(i.isDark()) continue;
        if(pointInRect(_p, i.getPos(), i.getDim()))
        {
            any = true;
        }
    }

    if(!any) return false;

    for(size_t i = 0; i < m_buttons.size(); ++i)
    {
        m_buttons[i].set(false);
        if(m_buttons[i].isDark()) continue;
        if(pointInRect(_p, m_buttons[i].getPos(), m_buttons[i].getDim()))
        {
            m_buttons[i].set(true);
            m_selected = i;
        }
    }
    return true;
}

void selection::reset()
{
    for(auto &i : m_buttons) i.reset();
}

void selection::update(const int _s, const vec2 _mouse)
{
    if(m_mouseState == MOUSE_OVER) m_interpolant += 0.1f;
    else m_interpolant -= 0.1f;

    for(auto &i : m_buttons)
    {
        if(!m_saveSelected) i.set(false);
        i.update(_s, _mouse);
    }
}
