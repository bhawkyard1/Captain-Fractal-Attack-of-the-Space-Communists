#include "vectors.hpp"
#include "ui/interface.hpp"

interface::interface()
{

}

selectionReturn handleInput(vec2 _pos)
{
  selectionReturn r = {-1, -1};

  for(size_t i = 0; i < m_elements.size(); ++i)
  {
    for(size_t j = 0; j < m_elements.at(i).getButtons()->size(); ++j)
    {
      button * b = m_elements.at(i).getButtons()->at(j);
      if(pointInRect(_pos, b->getPos(), p->getDim()))
      {
        r.m_sel_val = i;
        r.m_button_val = j;
      }
    }
  }
}

void playerUpgrade(int lvl)
{
  int type = upgrades_menu.getSelected();
  button * up = upgrades_menu.getAt(type);
  up->set(false);

  if(lvl < 9)
  {
    *scorept -= up->getCost();
    if(type < 4) up->setCost(up->getCost() * 2);
  }

  if(type > 3) return;

  std::string s1;

  switch(type)
  {
    case 0:
      s1 = "LASERS ";
      break;
    case 1:
      s1 = "SHIELDS ";
      break;
    case 2:
      s1 = "GENERATORS ";
      break;
    case 3:
      s1 = "THRUSTERS ";
      break;
  }

  s1 += roman_nums[lvl];

  if(lvl < 8)
  {
    s1 += " (";
    std::stringstream ss;
    ss << up->getCost();
    s1 += ss.str();
    s1 += ")";
  }

  if(lvl < 9) up->updateText(s1);
}
