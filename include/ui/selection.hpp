#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <vector>
#include "button.hpp"
#include "selection.hpp"

namespace ui
{

class selection
{
  std::vector<button> m_buttons;
  int m_selected;
public:
  std::vector<button> * getButtons() {return &m_buttons;}
  int getSelected() {return m_selected;}
  void add(button b) {m_buttons.push_back(b);}
  bool click(vec2);
  void draw();
  button * getAt(size_t i) {return &m_buttons.at(i);}
  void reset();
  void update(int _s);
  void clear() {m_buttons.clear();}
};

}
#endif
