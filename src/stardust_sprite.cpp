#include "stardust_sprite.hpp"
#include "util.hpp"
#include "common.hpp"

stardust_sprite::stardust_sprite(
    const std::string identifier,
    const std::array<float, 3> _col,
    const int _w,
    const int _h
    )
    :
    stardust(_col)

{	
  m_identifier = identifier;

  m_dim = std::max(_w, _h) / 2;
  m_ang = randNum(0.0f,360.0f);
  m_angVel = randNum(-0.12f, 0.12f);

  setPos( {(rand() % (g_WIN_WIDTH + _w) - _w) * g_BG_DENSITY,
           (rand() % (g_WIN_HEIGHT + _h) - _h) * g_BG_DENSITY} );
}

stardust_sprite::stardust_sprite(
    const std::string _identifier,
    const float _alph,
    const int _w,
    const int _h
    )
    :
    stardust(_alph)
{	
  m_identifier = _identifier;

  m_dim = std::max(_w, _h) / 2;

  m_ang = randNum(0.0f,360.0f);
  m_angVel = randNum(-0.12f, 0.12f);

}

void stardust_sprite::spriteGen(
    const std::array<float, 3> &_col,
    const int _w,
    const int _h)
{
  m_ang = randNum(0.0f, 360.0f);
  m_angVel = randNum(-0.12f, 0.12f);

  int m = std::max(_w, _h);
  m *= g_ZOOM_LEVEL;

  int colProb = rand()%100;

  if(colProb < _col[0])
  {
    setCol(0, rand()%26+230);
    setCol(1, rand()%40+30);
    setCol(2, rand()%40+10);
  }
  else if(colProb < _col[1])
  {
    setCol(0, rand()%40+150);
    setCol(1, rand()%40+140);
    setCol(2, rand()%26+230);
  }
  else
  {
    setCol(0, rand()%56+200);
    setCol(1, rand()%40+155);
    setCol(2, rand()%30+30);
  }

  int side = rand()%4;
  vec3 vec;
  if(side == 0)
  {
    //Top
    vec = { randNum(0, (g_WIN_WIDTH + 2 * m) ) - m, randNum(0, m) - 2 * m, 0.0f };
    vec *= g_BG_DENSITY;
    setPos(vec);
    setPPos(getPos());
  }
  else if(side == 1)
  {
    //Left
    vec = { randNum(0, m) - 2 * m, randNum(0, g_WIN_HEIGHT + 2 * m) - m, 0.0f };
    vec *= g_BG_DENSITY;
    setPos(vec);
    setPPos(getPos());
  }
  else if(side == 2)
  {
    //Right
    vec = { 2*m - randNum(0, m), randNum(0, g_WIN_HEIGHT + 2 * m) - m, 0.0f };
    vec *= g_BG_DENSITY;
    vec.m_x += g_WIN_WIDTH;
    setPos(vec);
    setPPos(getPos());
  }
  else if(side == 3)
  {
    //Bottom
    vec = { randNum(0, g_WIN_WIDTH + 2 * m) - m, 2 * m - randNum(0, m), 0.0f };
    vec *= g_BG_DENSITY;
    vec.m_y += g_WIN_HEIGHT;
    setPos(vec);
    setPPos(getPos());
  }
}

void stardust_sprite::updateSprite(const float _dt)
{
  m_ang += m_angVel;
  updatePos(_dt);
}

void stardust_sprite::incrDim(float _dt)
{
  if(m_identifier == "SMOKE")
  {
    m_dim += 32.0f * _dt;
  }
  else if(m_identifier == "EXPLOSION")
  {
    m_dim -= 32.0f * _dt;
  }
}
