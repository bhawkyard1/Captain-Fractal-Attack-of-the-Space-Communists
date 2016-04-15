#ifndef STARDUST_HPP
#define STARDUST_HPP

#include "base.hpp"
#include <array>

class stardust: public base
{
  float m_z;
  std::array<float, 4> m_col;
public:
  stardust(const std::array<float, 3> &_col);
  stardust(float _alpha);
  void gen(bool _regen, const std::array<float, 3> &_col);

  void updatePos(float _dt);

  void setZ(float _z) {m_z = _z;}
  float getZ() {return m_z;}
  float getCol(int _i) {return m_col[_i];}
  void setCol(int _i, float _v) {m_col[_i] = _v;}
};

#endif
