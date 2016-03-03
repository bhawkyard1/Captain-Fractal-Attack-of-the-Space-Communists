#ifndef SELECTION_HPP
#define SELECTION_HPP

#include "button.hpp"
#include "selection.hpp"

class selection
{
  std::vector<button> buttons;
  int selected;
public:
  std::vector<button> * getButtons() {return &buttons;}
  int getSelected() {return selected;}
  void add(button b) {buttons.push_back(b);}
  bool click(vec2);
  void draw();
  button * getAt(size_t i) {return &buttons.at(i);}
  void reset();
};

#endif
