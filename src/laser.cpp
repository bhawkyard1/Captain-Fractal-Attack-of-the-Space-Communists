#include "laser.hpp"

laser::laser(vec3 _p,
        vec3 _v,
        float _ang,
        std::array<float, WEAPS_W> _data,
        aiTeam _team,
        uniqueID _parent)
{
    float temp_angle = _ang + randNum(-_data[1], _data[1]);
    vec3 vAdd = tovec3(vec(temp_angle));
    vAdd.m_z = sin(randNum(-_data[1], _data[1]));

    setVel({_v.m_x + vAdd.m_x * _data[3], _v.m_y + vAdd.m_y * _data[3], _v.m_z + vAdd.m_z * _data[3] * 0.1f});
    setPos(_p);
    setPPos(_p);
	
    m_ang = temp_angle;
    m_dmg = _data[2];

    m_col[0] = _data[4];
    m_col[1] = _data[5];
    m_col[2] = _data[6];
	
    m_spd = _data[3];
	
    m_team = _team;
	
    m_power = 255.0f;

    m_stop = _data[9];

    m_ownerID = _parent;
}

void laser::update(float _dt)
{
    m_power -= 64.0f * _dt;
    updatePos(_dt);
}
