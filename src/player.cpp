#include "player.hpp"

player::player(
        vec2 _p,
        float _r)
        :
        ship(_p, PLAYER_SHIP, _r)
{
    setWeap(0);
    setMissiles(3);
}

void player::ctrlUpdate()
{
	vec2 p = getPos();
    p *= g_ZOOM_LEVEL;
    p += g_HALFWIN;
	vec2 v = getVel();
	setPos({v.m_x, v.m_y});
	setPPos({v.m_x, v.m_y});
	int mx = 0, my = 0;
	SDL_GetMouseState(&mx,&my);
    vec2 t = {p.m_x - mx, p.m_y - my};
    setTAng(deg(-ang(t)));
}
