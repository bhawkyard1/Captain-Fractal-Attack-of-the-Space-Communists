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

void stardust::updatePos(float dt)
{
	setPPos(getPos()); 
	setPos(getPos() + (getVel()+getWVel())* z);
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
		setPos({randFloat(-WIN_WIDTH / 2.0f, WIN_WIDTH / 2.0f), randFloat(-WIN_HEIGHT / 2.0f, WIN_HEIGHT / 2.0f)});
		setPPos(getPos());
	}
	else
	{
		int side = rand()%4;
		vec2 vec;
		
		if(side == 0)
		{
			//TOP
			vec = randVec({-HALFWIN.x - 100.0f, -HALFWIN.y - 200.0f}, {HALFWIN.x + 100.0f, -HALFWIN.y});
			vec *= BG_DENSITY;
			setPos(vec);
			setPPos(getPos());
		}
		else if(side == 1)
		{
			//RIGHT
			vec = randVec({HALFWIN.x, -HALFWIN.y - 100.0f}, {HALFWIN.x + 200.0f, HALFWIN.y + 100.0f});			
			vec *= BG_DENSITY;
			vec.x += WIN_WIDTH;
			setPos(vec);
			setPPos(getPos());
		}
		else if(side == 2)
		{
			//BOTTOM
			vec = randVec({-HALFWIN.x - 100.0f, HALFWIN.y}, {HALFWIN.x + 100.0f, HALFWIN.y + 200.0f});
			vec *= BG_DENSITY;
			vec.y += WIN_HEIGHT;
			setPos(vec);
			setPPos(getPos());
		}
		else if(side == 3)
		{
			//LEFT
			vec = randVec({-HALFWIN.x - 200.0f, -HALFWIN.y - 100.0f}, {-HALFWIN.x, HALFWIN.y + 100.0f});
			vec *= BG_DENSITY;
			setPos(vec);
			setPPos(getPos());
		}
	}
}
