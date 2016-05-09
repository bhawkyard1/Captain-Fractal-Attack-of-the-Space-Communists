#include "util.hpp"
#include "vectors.hpp"
#include "ui/user_interface.hpp"
#include <iostream>

std::array< std::string, 10> g_ROMAN_NUMS = {"I","II","III","IV","V","VI","VII","IX","X","X"};

userInterface::userInterface()
{

}

selectionReturn userInterface::handleInput(vec2 _pos)
{
  selectionReturn r = {-1, -1};
  for(size_t i = 0; i < m_elements.size(); ++i)
  {
    if(!m_elements[i].isVisible()) continue;
    for(size_t j = 0; j < m_elements[i].getbuttons()->size(); ++j)
    {
      button * b = m_elements[i].getAt(j);
      if(pointInRect(_pos, b->getPos(), b->getDim()))
      {
        r.m_sel_val = i;
        r.m_button_val = j;
      }
    }
  }
  return r;
}

void userInterface::reset()
{
  for(auto &i : m_elements) i.reset();
}

void userInterface::update(int _s)
{
  for(auto &i : m_elements) i.update(_s);
}
