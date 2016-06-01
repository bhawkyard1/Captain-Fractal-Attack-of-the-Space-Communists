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

    vec2 p = toScreenSpace( tovec2( getPos() ) );

    setTAng(deg(-ang(p - getMousePos() )));
}
