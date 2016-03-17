#ifndef LASER_HPP
#define LASER_HPP

#include "weapons.hpp"
#include "base.hpp"
#include "enemy.hpp"

class laser: public base
{
    int m_dmg;
    int m_spd;
    aiTeam m_team;
    float m_ang;
    std::array<float, 3> m_col;
    float m_power;
    float m_stop;
public:
    laser(vec2 _p, vec2 _v, float _ang, std::array<float, WEAPS_W> _data, aiTeam _team);
    int getDmg() const {return m_dmg;}
    float getStop() const {return m_stop;}
    void update(float _dt);
    aiTeam getTeam() const  {return m_team;}
    std::array<float, 4> getCol() const {return {m_col[0], m_col[1], m_col[2], 255.0f};}
    float getCol(int _index) const {return m_col[_index];}
    float getPower() const {return m_power;}
    float getAng() const {return m_ang;}
};

#endif

