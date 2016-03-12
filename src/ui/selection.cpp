#include "util.hpp"
#include "ui/selection.hpp"

namespace ui
{

selection::selection()
{
    m_visible = true;
}

bool selection::click(vec2 p)
{
  bool any = false;
  for(auto &i : m_buttons)
  {
    if(i.isDark()) continue;
    if(pointInRect(p, i.getPos(), i.getDim()))
    {
      any = true;
    }
  }

  if(!any) return false;

  for(size_t i = 0; i < m_buttons.size(); ++i)
  {
    m_buttons.at(i).set(false);
    if(m_buttons.at(i).isDark()) continue;
    if(pointInRect(p, m_buttons.at(i).getPos(), m_buttons.at(i).getDim()))
    {
      m_buttons.at(i).select();
      m_selected = i;
    }
  }
  return true;
}

void selection::reset()
{
  for(auto &i : m_buttons) i.reset();
}

void selection::update(int _s)
{
  for(auto &i : m_buttons) i.update(_s);
}

}
