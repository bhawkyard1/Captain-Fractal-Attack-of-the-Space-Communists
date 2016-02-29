#ifndef UI_HPP
#define UI_HPP

#include "SDL2/SDL.h"
#include "common.hpp"
#include <vector>
#include "ui_classes.hpp"


void drawMap();
void statusBars();
void drawCircle(int x, int y, int radius);
void drawText(int score, int missiles);
void energyChoice();
void drawUpgrades(int score);
void drawWeap();

std::vector<enemy>* ep;
std::vector<laser>* lp;
std::vector<missile>* mp;
std::vector<ship>* ap;
player* ply;
int* scorept;

selection energy_menu;
selection upgrades_menu;

std::string roman_nums[] = {"I","II","III","IV","V","VI","VII","IX","X"};

void menus_init()
{
	int arr7[] = {20,20,20,200,100,100,100,255};
	int arr8[] = {100,100,100,200,250,250,250,255};
	button energy_menu_neutral("BALANCED",arr7,arr8,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.35f},{128.0f,64.0f});
	energy_menu_neutral.set(true);
	energy_menu.add(energy_menu_neutral);
	
	int arr1[] = {12,24,26,200,27,95,232,255};
	int arr2[] = {45,67,188,200,119,156,238,255};
	button energy_menu_shields("SHIELDS",arr1,arr2,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.45f},{128.0f,64.0f});
	energy_menu.add(energy_menu_shields);
	
	int arr3[] = {14,35,20,200,36,204,52,255};
	int arr4[] = {65,127,64,200,129,241,127,255};
	button energy_menu_engines("ENGINES",arr3,arr4,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.55f},{128.0f,64.0f});
	energy_menu.add(energy_menu_engines);
	
	int arr5[] = {35,23,23,200,232,31,31,255};
	int arr6[] = {124,33,33,200,217,116,116,255};
	button energy_menu_guns("GUNS",arr5,arr6,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.65f},{128.0f,64.0f});
	energy_menu.add(energy_menu_guns);	
}

void upgrades_init()
{
	float w = 150.0f, h = 50.0f;
	int arr1[] = {100,50,50,200,250,200,200,255};
	int arr2[] = {100,50,50,200,250,200,200,255};
	button upgrades_lasers("LASERS I (4)",arr1,arr2,{WIN_WIDTH * 0.0f, WIN_HEIGHT * 0.85f},{w,h},4);
	upgrades_menu.add(upgrades_lasers);
	
	int arr3[] = {50,50,100,200,200,200,250,255};
	int arr4[] = {50,50,100,200,200,200,250,255};
	button upgrades_shields("SHIELDS I (4)",arr3,arr4,{WIN_WIDTH * 0.15f, WIN_HEIGHT * 0.85f},{w,h},4);
	upgrades_menu.add(upgrades_shields);
	
	int arr5[] = {50,100,50,200,200,250,200,255};
	int arr6[] = {50,100,50,200,200,250,200,255};
	button upgrades_generators("GENERATORS I (4)",arr5,arr6,{WIN_WIDTH * 0.3f, WIN_HEIGHT * 0.85f},{w,h},4);
	upgrades_menu.add(upgrades_generators);
	
	int arr7[] = {50,50,80,200,200,200,220,255};
	int arr8[] = {50,50,80,200,200,200,220,255};
	button upgrades_thrusters("THRUSTERS I (4)",arr7,arr8,{WIN_WIDTH * 0.45f, WIN_HEIGHT * 0.85f},{w,h},4);
	upgrades_menu.add(upgrades_thrusters);
	
	int arr9[] = {255,210,0,200,255,253,100,255};
	int arr10[] = {255,210,0,200,255,253,100,255};
	button upgrades_missiles("MISSILE (4)",arr9,arr10,{WIN_WIDTH * 0.6f, WIN_HEIGHT * 0.85f},{w,h},4);
	upgrades_menu.add(upgrades_missiles);
	
	int arr11[] = {100,210,255,200,180,220,255,255};
	int arr12[] = {100,210,255,200,180,220,255,255};
	button upgrades_miner("MINER (16)",arr11,arr12,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.85f},{w,h},16);
	upgrades_menu.add(upgrades_miner);

	button upgrades_wingman("WINGMAN (32)",arr11,arr12,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.79f},{w,h},32);
	upgrades_menu.add(upgrades_wingman);

	button upgrades_turret("TURRET (32)",arr11,arr12,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.73f},{w,h},32);
	upgrades_menu.add(upgrades_turret);
	
	button upgrades_gravwell("GRAVWELL (512)",arr11,arr12,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.67f},{w,h},512);
	upgrades_menu.add(upgrades_gravwell);
	
	button upgrades_barracks("BARRACKS (1024)",arr11,arr12,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.61f},{w,h},1024);
	upgrades_menu.add(upgrades_barracks);
	
	button upgrades_station("STATION (1024)",arr11,arr12,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.55f},{w,h},1024);
	upgrades_menu.add(upgrades_station);
}

void transmitPointers(universe * uni)
{
	ep = uni->getEnemies();
	lp = uni->getShots();
	mp = uni->getMissiles();
	ap = uni->getAsteroids();
	ply = uni->getPly();
	scorept = uni->getScorePt();
}

void drawUI(int score)
{
	drawMap();
	statusBars();
	drawText(score,ply->getMissiles());
	energyChoice();
	drawUpgrades(score);
	drawWeap();
}

void drawMap()
{
	SDL_Rect map;
	map.w = 256;
	map.h = 256;
	map.x = WIN_WIDTH - 256;
	map.y = 0;
	
	SDL_SetRenderDrawColor(renderer,200,200,255,100);
	SDL_RenderFillRect(renderer,&map);
	
	SDL_SetRenderDrawColor(renderer,0,0,0,255);
	SDL_RenderDrawLine(renderer,WIN_WIDTH-128,124,WIN_WIDTH-128,132);
	SDL_RenderDrawLine(renderer,WIN_WIDTH-124,128,WIN_WIDTH-132,128);
	
	SDL_SetRenderDrawColor(renderer,0,0,255,255);
	for(unsigned int i = 0; i < lp->size(); i++)
	{
		vec2 lpp = lp->at(i).getPos();
		
		double x = clamp(lpp.x / 156.0f + WIN_WIDTH - 128.0f, WIN_WIDTH - 256.0f, static_cast<float>(WIN_WIDTH));
		double y = clamp(lpp.y / 156.0f + 128.0f, 0.0f, 256.0f);
		
		SDL_RenderDrawPoint(renderer,x,y);
	}
	
	SDL_SetRenderDrawColor(renderer,255,0,0,255);
	for(unsigned int i = 0; i < mp->size(); i++)
	{
		vec2 mpp = mp->at(i).getPos();
		
		double x = clamp(mpp.x / 156.0f + WIN_WIDTH - 128.0f, WIN_WIDTH - 256.0f, static_cast<float>(WIN_WIDTH));
		double y = clamp(mpp.y / 156.0f + 128.0f, 0.0f, 256.0f);
		
		SDL_RenderDrawPoint(renderer,x,y);
	}
	
	SDL_SetRenderDrawColor(renderer,200,200,200,255);
	for(unsigned int i = 0; i < ap->size(); i++)
	{
		vec2 app = ap->at(i).getPos();
		
		double x = clamp(app.x / 156.0f + WIN_WIDTH - 128.0f, WIN_WIDTH - 256.0f, static_cast<float>(WIN_WIDTH));
		double y = clamp(app.y / 156.0f + 128.0f, 0.0f, 256.0f);
		
		int radius = 1;
		if(ap->at(i).getClassification() == ASTEROID_MID) radius = 2;
		else if(ap->at(i).getClassification() == ASTEROID_LARGE) radius = 3;
		
		drawCircle(x,y,radius);
	}
	
	for(unsigned int i = 0; i < ep->size(); i++)
	{
		vec2 epp = ep->at(i).getPos();
		int radius = clamp( ep->at(i).getRadius() / 16.0f, 1.0f, 5.0f );
		
		if(ep->at(i).getTeam() == TEAM_PLAYER) SDL_SetRenderDrawColor(renderer,0,255,0,255);
		else if(ep->at(i).getTeam() == TEAM_PLAYER_MINER) SDL_SetRenderDrawColor(renderer,0,255,0,255);
		else if(ep->at(i).getTeam() == GALACTIC_FEDERATION or ep->at(i).getTeam() == SPOOKY_SPACE_PIRATES) SDL_SetRenderDrawColor(renderer,255,0,0,255);
				
		float x = clamp(epp.x / 156.0f + WIN_WIDTH - 128.0f, WIN_WIDTH - 256.0f, static_cast<float>(WIN_WIDTH));
		float y = clamp(epp.y / 156.0f + 128.0f, 0.0f, 256.0f);
		
		drawCircle(x,y,radius);
	}
}

void statusBars()
{
	SDL_Rect health_base;
	health_base.w = 256;
	health_base.h = 16;
	health_base.x = 0;
	health_base.y = 0;//WIN_HEIGHT-32;
	
	SDL_SetRenderDrawColor(renderer,100,20,20,255);
	SDL_RenderFillRect(renderer,&health_base);
	
	SDL_Rect health;
	health.w = ply->getHealth()*2.56;
	health.h = 16;
	health.x = 0;
	health.y =0;// WIN_HEIGHT-32;
	
	SDL_SetRenderDrawColor(renderer,230,50,50,255);
	SDL_RenderFillRect(renderer,&health);
	
	SDL_Rect shield_base;
	shield_base.w = 256;
	shield_base.h = 16;
	shield_base.x = 0;
	shield_base.y = 16;//WIN_HEIGHT-32;
	
	SDL_SetRenderDrawColor(renderer,20,20,100,255);
	SDL_RenderFillRect(renderer,&shield_base);
	
	SDL_Rect shield;
	shield.w = ply->getShield()*2.56;
	shield.h = 16;
	shield.x = 0;
	shield.y =16;// WIN_HEIGHT-32;
	
	SDL_SetRenderDrawColor(renderer,50,50,230,255);
	SDL_RenderFillRect(renderer,&shield);

	SDL_Rect energy_base;
	energy_base.w = 256;
	energy_base.h = 16;
	energy_base.x = 0;
	energy_base.y = 32;//WIN_HEIGHT-32;
	
	SDL_SetRenderDrawColor(renderer,20,100,20,255);
	SDL_RenderFillRect(renderer,&energy_base);
	
	SDL_Rect energy;
	energy.w = ply->getEnergy()*2.56;
	energy.h = 16;
	energy.x = 0;
	energy.y =32;// WIN_HEIGHT-32;
	
	SDL_SetRenderDrawColor(renderer,50,230,50,255);
	SDL_RenderFillRect(renderer,&energy);
}

void drawCircle(int x, int y, int radius)
{
	for(int j = y - radius; j < y + radius; j++)
	{
		for(int i = x - radius; i < x + radius; i++)
		{
			if((i-x)*(i-x)+(j-y)*(j-y)<radius*radius)
			{
				SDL_RenderDrawPoint(renderer,i,j);
			}
		}
	}
}

void drawText(int score, int missiles)
{
  TTF_Font * Font_Pixelade = TTF_OpenFont("../resources/fonts/pix.TTF", 18);
  if(!Font_Pixelade) std::cout << "Font not loaded! " << TTF_GetError() << std::endl;
	
  std::string scoreText = "SCORE : ";
	
  scoreText += std::to_string(score);
	
	SDL_Texture * scoreTxt = renderText(
		scoreText.c_str(),
		Font_Pixelade,
		{255,255,255,255},
		renderer,
		I_MAX
		);
	
	SDL_Rect sdst = {260,2,86,32};
	
	SDL_RenderCopy(renderer, scoreTxt, NULL, &sdst);
	
  std::string missilesText = "MISSILES : ";

  missilesText += std::to_string(missiles);
	
	SDL_Texture * missilesTxt = renderText(
		missilesText.c_str(),
		Font_Pixelade,
		{255,255,255,255},
		renderer,
		I_MAX
		);
	
	SDL_Rect mdst = {260,40,86,32};
	
	SDL_RenderCopy(renderer, missilesTxt, NULL, &mdst);
	
	TTF_CloseFont(Font_Pixelade);
	SDL_DestroyTexture(scoreTxt);
	SDL_DestroyTexture(missilesTxt);
}

struct selectionReturn
{
	bool sel;
	int val;
};

void energyChoice()
{
	energy_menu.draw();
}

selectionReturn energy_clicked(vec2 p)
{
	bool ret1 = energy_menu.click(p);
	return {ret1, energy_menu.getSelected()};
}

void drawUpgrades(int score)
{
  std::vector<button> * vec = upgrades_menu.getButtons();
	for(size_t i = 0; i < vec->size(); i++)
	{
		if(vec->at(i).getCost() > score or (i < 4 and ply->getUpgrade(i) > 7))
		{ 
			vec->at(i).setDark(true);
		}
		else vec->at(i).setDark(false);
	}
	
	upgrades_menu.draw();
}

selectionReturn upgrades_clicked(vec2 p)
{
	bool ret1 = upgrades_menu.click(p);
	return {ret1, upgrades_menu.getSelected()};
}

void setUpgradeTextures(int lvl, int type)
{
	if(type > 3) return;
	
  std::string s1;
	
	button * up = upgrades_menu.getAt(type);
	up->setCost(pow(4, lvl + 1));
	
	switch(type)
	{
		case 0:
			s1 = "LASERS ";
			break;
		case 1:
			s1 = "SHIELDS ";
			break;
		case 2:
			s1 = "GENERATORS ";
			break;
		case 3:
			s1 = "THRUSTERS ";
			break;
	}

	s1 += roman_nums[lvl];
	
	if(lvl < 8)
	{
		s1 += " (";
    std::stringstream ss;
		ss << up->getCost();
		s1 += ss.str();
		s1 += ")";
	}
	
	if(lvl < 9) up->updateText(s1);
}

void playerUpgrade(int lvl)
{
	int type = upgrades_menu.getSelected();
	button * up = upgrades_menu.getAt(type);
	up->set(false);
	
	if(lvl < 9)
	{
		*scorept -= up->getCost();
		if(type < 4) up->setCost(up->getCost() * 2);
	}
	
	if(type > 3) return;
	
  std::string s1;
	
	switch(type)
	{
		case 0:
			s1 = "LASERS ";
			break;
		case 1:
			s1 = "SHIELDS ";
			break;
		case 2:
			s1 = "GENERATORS ";
			break;
		case 3:
			s1 = "THRUSTERS ";
			break;
	}
	
	s1 += roman_nums[lvl];
	
	if(lvl < 8)
	{
		s1 += " (";
    std::stringstream ss;
		ss << up->getCost();
		s1 += ss.str();
		s1 += ")";
	}
	
	if(lvl < 9) up->updateText(s1);
}

void drawWeap()
{
	SDL_Rect weap;
	weap.w = WIN_WIDTH * 0.1;
	weap.h = WIN_HEIGHT * 0.2;
	weap.x = WIN_WIDTH - weap.w;
	weap.y = WIN_HEIGHT - 1.4 * weap.h;
	
	int wn = ply->getWeap();
	
	SDL_SetRenderDrawColor(renderer,weapons[wn][4],weapons[wn][5],weapons[wn][6],255);
	SDL_RenderFillRect(renderer,&weap);
	
	
  TTF_Font * fnt = TTF_OpenFont("../resources/fonts/minimal.otf", 25);
	
  std::string weapText = "Damage ";

  weapText += std::to_string(weapons[wn][2]);
	
	weapText += "\nLasers ";
  weapText += std::to_string(weapons[wn][0]);
	
	weapText += "\nSpread ";
  weapText += std::to_string(weapons[wn][1]);
	
	weapText += "\nRate ";
	float rate = 1.0f / weapons[wn][8];
  weapText += std::to_string( static_cast<int>( rate ) );
		
	SDL_Texture * weapsTexture = renderText(
		weapText.c_str(),
		fnt,
		{255,255,255,255},
		renderer,
		WIN_WIDTH * 0.1
		);
	
	SDL_RenderCopy(renderer, weapsTexture, NULL, &weap);
	
	TTF_CloseFont(fnt);
	SDL_DestroyTexture(weapsTexture);
}

#endif
