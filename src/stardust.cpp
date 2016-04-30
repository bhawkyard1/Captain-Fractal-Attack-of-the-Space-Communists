#include <array>
#include "stardust.hpp"
#include "util.hpp"
#include "common.hpp"

stardust::stardust(const std::array<float, 3> &_col)
{
  gen(false, _col);
}

stardust::stardust(float _alph)
{
  m_col[0] = 255.0f;
  m_col[1] = 255.0f;
  m_col[2] = 255.0f;
  m_col[3] = _alph;
}

void stardust::updatePos(float _dt)
{
  setPPos(getPos());
  setPos(getPos() + (getVel()+getWVel()) * getZ() * g_PIXEL_UNIT_CONVERSION * _dt);
}

void stardust::gen(
    bool _regen,
    const std::array<float, 3> &_col
    )
{
  addPos({0.0f, 0.0f, randNum(0.05f, 1.1f)});

  int colProb = rand()%100;

  if(colProb < _col[0])
  {
    m_col[0] = rand() % 26 + 230;
    m_col[1] = rand() % 40 + 60;
    m_col[2] = rand() % 40 + 30;
  }
  else if(colProb < _col[1])
  {
    m_col[0] = rand() % 40 + 150;
    m_col[1] = rand() % 40 + 140;
    m_col[2] = rand() % 26 + 230;
  }
  else
  {
    m_col[0] = rand() % 56 + 200;
    m_col[1] = rand() % 40 + 155;
    m_col[2] = rand() % 30 + 30;
  }

  m_col[3] = ( getZ() + 0.25 ) * 150;

  if(_regen == false)
  {
    setVel( {randNum(-0.1f, 0.1f), randNum(-0.1f, 0.1f)} );
    setPos({randNum(-g_WIN_WIDTH / 2.0f, g_WIN_WIDTH / 2.0f), randNum(-g_WIN_HEIGHT / 2.0f, g_WIN_HEIGHT / 2.0f)});
    setPPos(getPos());
  }
  else
  {
    int side = rand()%4;
    vec3 vec;

    if(side == 0)
    {
      //TOP
      vec = randVec3({-g_HALFWIN.m_x - 100.0f, -g_HALFWIN.m_y - 200.0f}, {g_HALFWIN.m_x + 100.0f, -g_HALFWIN.m_y});
      vec *= g_BG_DENSITY;
      setPos(vec);
      setPPos(getPos());
    }
    else if(side == 1)
    {
      //RIGHT
      vec = randVec3({g_HALFWIN.m_x, -g_HALFWIN.m_y - 100.0f, 0.0f}, {g_HALFWIN.m_x + 200.0f, g_HALFWIN.m_y + 100.0f, 0.0f});
      vec *= g_BG_DENSITY;
      vec.m_x += g_WIN_WIDTH;
      setPos(vec);
      setPPos(getPos());
    }
    else if(side == 2)
    {
      //BOTTOM
      vec = randVec3({-g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y, 0.0f}, {g_HALFWIN.m_x + 100.0f, g_HALFWIN.m_y + 200.0f, 0.0f});
      vec *= g_BG_DENSITY;
      vec.m_y += g_WIN_HEIGHT;
      setPos(vec);
      setPPos(getPos());
    }
    else if(side == 3)
    {
      //LEFT
      vec = randVec3({-g_HALFWIN.m_x - 200.0f, -g_HALFWIN.m_y - 100.0f, 0.0f}, {-g_HALFWIN.m_x, g_HALFWIN.m_y + 100.0f, 0.0f});
      vec *= g_BG_DENSITY;
      setPos(vec);
      setPPos(getPos());
    }
  }
}
