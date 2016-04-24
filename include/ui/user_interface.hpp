#ifndef USERINTERFACE_HPP
#define USERINTERFACE_HPP

#include <vector>
#include <array>
#include "vectors.hpp"
#include "selection.hpp"
#include "util.hpp"

extern std::array< std::string, 10> g_ROMAN_NUMS;

namespace ui
{

struct selectionReturn
{
  int m_sel_val;
  int m_button_val;
};

class userInterface
{
  std::vector<selection> m_elements;
public:
  userInterface();
  selectionReturn handleInput(vec2 _pos);
  void add(const selection _s) {m_elements.push_back(_s);}
  std::vector<selection> * getElements() {return &m_elements;}
  selection getElement(size_t _i) {return m_elements.at(_i);}
  void reset();
  void update(int _s);
  void clear() {m_elements.clear();}
  void remove(int _i) {swapnpop(&m_elements, _i);}
  void pop() {m_elements.pop_back();}
};

}

#endif
