#include "player.hpp"

player::player(
        vec3 _p
        )
    :
      ship(g_ship_templates[PLAYER_SHIP], _p)
{
    setWeap(0);
    setMissiles(3);
}

void player::ctrlUpdate()
{
    if(g_GAME_OVER) return;

    /*vec3 v = getVel();
    setPos( v );
    setPPos( v );*/
    setPos(vec3());
    setPPos(vec3());

    vec2 p = toScreenSpace( tovec2( getPos() ) );

    setTAng( deg( -ang(p - getMousePos() ) ) );
}
