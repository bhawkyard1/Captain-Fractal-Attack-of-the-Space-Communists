#include "util.hpp"
#include "ui/selection.hpp"

selection::selection()
{
    m_type = SELECTION_STATIC;
    m_visible = true;
    m_interpolant = 0.0f;
    m_saveSelected = false;
    m_pos = {F_INF, F_INF};
    m_dim = {-F_INF, -F_INF};
}

selection::selection(selectionType _type)
{
    m_visible = true;
    m_interpolant = 0.0f;
    m_saveSelected = false;
    m_pos = {F_INF, F_INF};
    m_dim = {-F_INF, -F_INF};
}

bool selection::click(vec2 _p)
{
    if(!m_visible or !pointInRect(_p, m_pos, m_dim)) return false;

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
    if(m_type = SELECTION_ROLLOUT)
    {
        float tempInterp = m_interpolant;

        if(pointInRect(_mouse, m_pos, m_dim)) m_interpolant = clamp(m_interpolant + 0.05f, 0.0f, 1.0f);
        else m_interpolant = clamp(m_interpolant - 0.05f, 0.0f, 1.0f);

        //If the menu has changed size...
        if(tempInterp != m_interpolant)
        {
            m_pos = {F_INF, F_INF};
            m_dim = {-F_INF, -F_INF};

            for(auto &b : m_buttons)
            {
                vec2 p = b.getPos();

                //Recalc bounding box.
                if(p.m_x < m_pos.m_x) m_pos.m_x = p.m_x;
                if(p.m_y < m_pos.m_y) m_pos.m_y = p.m_y;
            }
            for(auto &b : m_buttons)
            {
                vec2 d = b.getPos() + b.getDim() - m_pos;

                if(d.m_x > m_dim.m_x) m_dim.m_x = d.m_x;
                if(d.m_y > m_dim.m_y) m_dim.m_y = d.m_y;
            }
        }
    }

    for(auto &i : m_buttons)
    {
        if(!m_saveSelected) i.set(false);
        i.update(_s, _mouse, m_interpolant);
    }
}

void selection::add(const button _b)
{
    vec2 p = _b.getPos();
    vec2 d = _b.getDim() + (p - m_pos);

    //Recalc bounding box.
    if(p.m_x < m_pos.m_x) m_pos.m_x = p.m_x;
    if(p.m_y < m_pos.m_y) m_pos.m_y = p.m_y;

    if(d.m_x > m_dim.m_x) m_dim.m_x = d.m_x;
    if(d.m_y > m_dim.m_y) m_dim.m_y = d.m_y;

    m_buttons.push_back(_b);
}
