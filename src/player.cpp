#include "player.hpp"

player::player(
        vec3 _p,
        float _r)
    :
      ship(_p, PLAYER_SHIP, _r)
{
    setWeap(0);
    setMissiles(3);
}

void player::ctrlUpdate()
{
    if(g_GAME_OVER) return;
    vec3 v = getVel();
    setPos( v );
    setPPos( v );
    vec3 p = getPos();
    p *= g_ZOOM_LEVEL;
    p += tovec3(g_HALFWIN);
    int mx = 0, my = 0;
    SDL_GetMouseState(&mx,&my);
    vec3 t = {p.m_x - mx, p.m_y - my, 0.0f};
    setTAng(deg(-ang(tovec2(t))));
}
