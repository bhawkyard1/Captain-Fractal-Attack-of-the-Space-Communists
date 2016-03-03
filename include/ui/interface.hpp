#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <vector>
#include <array>
#include "vectors.hpp"
#include "selection.hpp"

extern std::array< std::string, 10> roman_nums;

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
