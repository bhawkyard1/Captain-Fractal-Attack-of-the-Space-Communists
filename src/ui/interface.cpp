#include "util.hpp"
#include "vectors.hpp"
#include "ui/interface.hpp"
#include <iostream>

std::array< std::string, 10> roman_nums = {"I","II","III","IV","V","VI","VII","IX","X"};

interface::interface()
{

}

selectionReturn interface::handleInput(vec2 _pos)
{
  selectionReturn r = {-1, -1};
  for(size_t i = 0; i < m_elements.size(); ++i)
  {
    for(size_t j = 0; j < m_elements[i].getButtons()->size(); ++j)
    {
      button b = m_elements[i].getButtons()->at(j);
      if(pointInRect(_pos, b.getPos(), b.getDim()))
      {
        r.m_sel_val = i;
        r.m_button_val = j;
      }
    }
  }
  return r;
}

void interface::reset()
{
  for(auto i = m_elements.begin(); i != m_elements.end(); ++i) i->reset();
}

void interface::update(int _s)
{
  for(auto i = m_elements.begin(); i != m_elements.end(); ++i) i->update(_s);
}
