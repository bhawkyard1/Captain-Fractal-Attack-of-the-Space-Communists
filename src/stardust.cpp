#include "stardust.hpp"
#include "util.hpp"
#include "common.hpp"

stardust::stardust(float colp[])
{
	gen(false, colp);
}

stardust::stardust(float alph)
{
	col[0] = 255.0f;
	col[1] = 255.0f;
	col[2] = 255.0f;
	col[3] = alph;
}

void stardust::updatePos(float _dt)
{
	setPPos(getPos()); 
    setPos(getPos() + (getVel()+getWVel()) * z * g_PIXEL_UNIT_CONVERSION * _dt);
}

void stardust::gen(bool regen, float colp[])
{
	z = randFloat(0.05f, 1.1f);

	int colProb = rand()%100;

	if(colProb < colp[0])
	{
		col[0] = rand()%26+230;
		col[1] = rand()%40+60;
		col[2] = rand()%40+30;
	}
	else if(colProb < colp[1])
	{
		col[0] = rand()%40+150;
		col[1] = rand()%40+140;
		col[2] = rand()%26+230;
	}
	else
	{
		col[0] = rand()%56+200;
		col[1] = rand()%40+155;
		col[2] = rand()%30+30;
	}
	
	col[3] = (z+0.25)*150;
	
	if(regen == false)
	{
		setVel( {randFloat(-0.1f, 0.1f), randFloat(-0.1f, 0.1f)} );
        setPos({randFloat(-g_WIN_WIDTH / 2.0f, g_WIN_WIDTH / 2.0f), randFloat(-g_WIN_HEIGHT / 2.0f, g_WIN_HEIGHT / 2.0f)});
		setPPos(getPos());
	}
	else
	{
		int side = rand()%4;
		vec2 vec;
		
		if(side == 0)
		{
			//TOP
            vec = randVec({-g_HALFWIN.x - 100.0f, -g_HALFWIN.y - 200.0f}, {g_HALFWIN.x + 100.0f, -g_HALFWIN.y});
            vec *= g_BG_DENSITY;
			setPos(vec);
			setPPos(getPos());
		}
		else if(side == 1)
		{
			//RIGHT
            vec = randVec({g_HALFWIN.x, -g_HALFWIN.y - 100.0f}, {g_HALFWIN.x + 200.0f, g_HALFWIN.y + 100.0f});
            vec *= g_BG_DENSITY;
            vec.x += g_WIN_WIDTH;
			setPos(vec);
			setPPos(getPos());
		}
		else if(side == 2)
		{
			//BOTTOM
            vec = randVec({-g_HALFWIN.x - 100.0f, g_HALFWIN.y}, {g_HALFWIN.x + 100.0f, g_HALFWIN.y + 200.0f});
            vec *= g_BG_DENSITY;
            vec.y += g_WIN_HEIGHT;
			setPos(vec);
			setPPos(getPos());
		}
		else if(side == 3)
		{
			//LEFT
            vec = randVec({-g_HALFWIN.x - 200.0f, -g_HALFWIN.y - 100.0f}, {-g_HALFWIN.x, g_HALFWIN.y + 100.0f});
            vec *= g_BG_DENSITY;
			setPos(vec);
			setPPos(getPos());
		}
	}
}
