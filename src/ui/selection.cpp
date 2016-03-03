#include "util.hpp"
#include "ui/selection.hpp"

bool selection::click(vec2 p)
{
  bool any = false;
  for(size_t i = 0; i < buttons.size(); i++)
  {
    if(buttons.at(i).isDark()) continue;
    if(pointInRect(p, buttons.at(i).getPos(), buttons.at(i).getDim()))
    {
      any = true;
    }
  }

  if(!any) return false;

  for(size_t i = 0; i < buttons.size(); i++)
  {
    buttons.at(i).set(false);
    if(buttons.at(i).isDark()) continue;
    if(pointInRect(p, buttons.at(i).getPos(), buttons.at(i).getDim()))
    {
      buttons.at(i).select();
      selected = i;
    }
  }
  return true;
}

void selection::reset()
{
  for(auto i = buttons.begin(); i != buttons.end(); ++i) i->reset();
}
