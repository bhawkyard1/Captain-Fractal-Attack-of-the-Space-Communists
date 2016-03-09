#include "stardust_sprite.hpp"
#include "util.hpp"
#include "common.hpp"

stardust_sprite::stardust_sprite(std::string identifier, float colp[], int w, int h): stardust(colp)
{	
  m_identifier = identifier;

  m_dim = std::max(w, h) / 2;
  m_ang = randFloat(0.0f,360.0f);
  m_angVel = randFloat(-0.12f, 0.12f);
	
    setPos({(rand()%(g_WIN_WIDTH+w)-w)*g_BG_DENSITY,(rand()%(g_WIN_HEIGHT+h)-h)*g_BG_DENSITY});
}

stardust_sprite::stardust_sprite(std::string identifier, float alph, int w, int h): stardust(alph)
{	
  m_identifier = identifier;

  m_dim = std::max(w, h) / 2;
		
  m_ang = randFloat(0.0f,360.0f);
  m_angVel = randFloat(-0.12f, 0.12f);

}

void stardust_sprite::spriteGen(float colp[], int w, int h)
{
  m_ang = randFloat(0.0f, 360.0f);
  m_angVel = randFloat(-0.12f, 0.12f);
		
  int m = std::max(w,h);
	//m *= getZ();
    m *= g_ZOOM_LEVEL;
	
	int colProb = rand()%100;

	if(colProb < colp[0])
	{
		setCol(0, rand()%26+230);
		setCol(1, rand()%40+30);
		setCol(2, rand()%40+10);
	}
	else if(colProb < colp[1])
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
	vec2 vec;
	if(side == 0)
	{
		//Top
        vec = { randFloat(0, (g_WIN_WIDTH + 2 * m) ) - m, randFloat(0, m) - 2 * m};
        vec *= g_BG_DENSITY;
		setPos(vec);
		setPPos(getPos());
	}
	else if(side == 1)
	{
		//Left
        vec = { randFloat(0, m) - 2 * m, randFloat(0, g_WIN_HEIGHT + 2 * m) - m };
        vec *= g_BG_DENSITY;
		setPos(vec);
		setPPos(getPos());
	}
	else if(side == 2)
	{
		//Right
        vec = { 2*m - randFloat(0, m), randFloat(0, g_WIN_HEIGHT + 2 * m) - m };
        vec *= g_BG_DENSITY;
        vec.x += g_WIN_WIDTH;
		setPos(vec);
		setPPos(getPos());
	}
	else if(side == 3)
	{
		//Bottom
        vec = { randFloat(0, g_WIN_WIDTH + 2 * m) - m, 2 * m - randFloat(0, m) };
        vec *= g_BG_DENSITY;
        vec.y += g_WIN_HEIGHT;
		setPos(vec);
		setPPos(getPos());
	}
}

void stardust_sprite::updateSprite(float dt)
{
  m_ang += m_angVel;
	updatePos(dt);
}

void stardust_sprite::incrDim()
{
  if(m_identifier == "SMOKE")
	{
    m_dim += 2.0f;
	}
  else if(m_identifier == "EXPLOSION")
	{
    m_dim -= 2.0f;
	}
}
