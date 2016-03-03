#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <vector>

#include "vectors.hpp"
#include "selection.hpp"

std::string roman_nums[] = {"I","II","III","IV","V","VI","VII","IX","X"};

struct selectionReturn
{
  int m_sel_val;
  int m_button_val;
};

class interface
{
  std::vector<selection> m_elements;
public:
  interface();
  selectionReturn handleInput(vec2 _pos);
  void add(selection _s) {m_elements.push_back(_s);}
  std::vector<selection> * getElements() {return &m_elements;}
  selection getElement(size_t _i) {return m_elements.at(_i);}
  void reset();
};

#endif
