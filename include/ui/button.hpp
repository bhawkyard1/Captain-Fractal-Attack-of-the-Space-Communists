#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <array>
#include <string>
#include "vectors.hpp"

class button
{
  bool selected = false;
  std::array<int, 8> col;
  std::array<int, 8> tcol;
  std::string m_initLabel;
  std::string m_label;
  int m_initCost;
  int cost;
  bool dark;
  vec2 m_pos, m_dim;
public:
  button(std::string,std::array<int, 8>,std::array<int, 8>,vec2,vec2);
  button(std::string,std::array<int, 8>,std::array<int, 8>,vec2,vec2,int);
  void select();
  bool isSelected() {return selected;}
  void updateText(std::string);
  void set(bool s) {selected = s;}
  bool on() {return selected;}
  void update(int _s);

  int getCost() {return cost;}
  void setCost(int pcost) {cost = pcost;}

  void setDark(bool b) {dark = b;}
  bool isDark() {return dark;}

  vec2 getPos() {return m_pos;}
  vec2 getDim() {return m_dim;}

  std::array<int, 8> getCol() {return col;}
  std::array<int, 8> getTCol() {return tcol;}

  std::string getLabel() {return m_label;}

  void reset();
};

#endif
