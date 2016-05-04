#ifndef POPUP_HPP
#define POPUP_HPP

#include <array>
#include <string>

#include "base.hpp"
#include "vectors.hpp"

enum popup_type {POPUP_GOOD, POPUP_BAD, POPUP_NEUTRAL};
extern std::array< std::array<float, 4>, 3 > g_popupCols;

class popup: public base
{
public:
    popup(const std::string _label, vec3 _pos, vec3 _vel, float _smul, std::array<float, 4> _col);
    void update(float _dt);
    float getCol(int _i) {return m_col[_i];}
    std::array<float, 4> getCol() {return m_col;}
    float getSize() {return m_smul;}
    std::string getLabel() {return m_label;}
private:
    std::string m_label;
    std::array<float, 4> m_col;
    float m_smul;
};

#endif
