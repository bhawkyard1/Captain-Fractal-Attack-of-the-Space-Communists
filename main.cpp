//General headers
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>

#include <ciso646>

#undef main
#include <cstdlib>

//Used for string/file operations
#include <string>
#include <fstream>
#include <sstream>

//SDL headers
#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//Including this causes conflicts with ngl, since this includes gl.h before it is supposed to be included or something.
//#include <SDL2/SDL_opengl.h>

#include "common.hpp"
#include "file.hpp"
#include "sfx.hpp"
#include "sim_time.hpp"
#include "ui/user_interface.hpp"
#include "universe.hpp"
#include "util.hpp"
#include "vectors.hpp"
#include "weapons.hpp"

#undef userInterface

//Function prototypes.
void gameInit();
void handleUserKeyDownInput(int, player*, universe*, int*);
void handleUserKeyUpInput(int,int*);
void handleUserMouseDownInput(int, int *, player*, universe*);
void handleUserMouseUpInput(int,int,player*,universe*);
void handleUserScroll(int,player*);

void mainMenu(universe &uni);
void playGame(universe &uni);
void playTutorial(universe &uni);
void sandbox(universe &uni);

int main(int argc, char* argv[])
{
	std::cout << "CAPTAIN FRACTAL INITIALISING..." << std::endl;
	loadConfig();
	loadWeapons();
	std::cout << "weapons size : " << g_weapons.size() << '\n';
	universe uni;

	while(g_GAME_STATE != MODE_QUIT)
	{
		if(g_GAME_STATE == MODE_MENU)
		{
			gameInit();
			mainMenu(uni);
		}
		else if(g_GAME_STATE == MODE_TUTORIAL)
		{
			gameInit();
			playTutorial(uni);
		}
		else if(g_GAME_STATE == MODE_GAME)
		{
			gameInit();
			playGame(uni);
		}
		else if(g_GAME_STATE == MODE_SANDBOX)
		{
			gameInit();
			sandbox(uni);
		}
	}
	return 0;
}

void mainMenu(universe &uni)
{
	g_GAME_OVER = true;

	uni.playMus(1);
	uni.reload(true);
	uni.getPly()->setHealth(-1);
	uni.update(0.1f);
	uni.getUI()->clear();

	selection mainMenuSelection = loadSelection("mainMenuSelection.txt");
	std::cout << "MAIN MENU LOADED! Len : " << mainMenuSelection.getButtons()->size() << '\n';
	if(g_DEV_MODE)
	{
		button sandboxMode(
					"SANDBOX",
		{{0, 255, 255, 255}},
		{{255, 255, 255, 255}},
					vec2(g_HALFWIN.m_x + 256.0f, g_HALFWIN.m_y),
					vec2(256.0f, 256.0f)
					);
		mainMenuSelection.add(sandboxMode);
	}
	uni.getUI()->add(mainMenuSelection);

	selection optionsHeader = loadSelection("optionsHeaderMenu.txt");
	optionsHeader.setVisible(false);
	uni.getUI()->add(optionsHeader);

	selection resolutionOptions = loadSelection("optionsResolutionMenu.txt");
	resolutionOptions.setVisible(false);
	uni.getUI()->add(resolutionOptions);

	selection qualityOptions = loadSelection("optionsGraphicsMenu.txt");
	qualityOptions.setVisible(false);
	uni.getUI()->add(qualityOptions);

	selection difficultyOptions = loadSelection("optionsDifficultyMenu.txt");
	difficultyOptions.setVisible(false);
	uni.getUI()->add(difficultyOptions);

	g_TARG_ZOOM_LEVEL = 0.3f;
	g_DIFFICULTY = 20;

	vec3 scrollVel = tovec3(randVec2(0.5f, 2.0f));
	uni.setVel(scrollVel);
	uni.setMaxWingmanCount(10);

	uni.addBuild(vec3(-2000.0f, -900.0f, 0.0f), PLAYER_GRAVWELL, TEAM_PLAYER);
	uni.addBuild(vec3(-1500.0f, 600.0f, 0.0f), PLAYER_BARRACKS, TEAM_PLAYER);
	uni.addBuild(vec3(600.0f, 300.0f, 0.0f), PLAYER_STATION, TEAM_PLAYER);

	for(int i = 0; i < 360; i += 12)
	{
		vec3 pos = {static_cast<float>(cos(rad(i))), static_cast<float>(sin(rad(i))), 0.0f};
		pos *= 1100.0f;
		uni.addBuild(pos + vec3(600.0f, 300.0f, 0.0f), PLAYER_TURRET, TEAM_PLAYER);
	}
	for(int i = 6; i < 360; i += 12)
	{
		vec3 pos = {static_cast<float>(cos(rad(i))), static_cast<float>(sin(rad(i))), 0.0f};
		pos *= 1150.0f;
		uni.addBuild(pos + vec3(600.0f, 300.0f, 0.0f), PLAYER_TURRET, TEAM_PLAYER);
	}

	for(int i = 0; i < 10; ++i)
	{
		aiTeam t = static_cast<aiTeam>(randNum(0,4));
		int r = randNum(5, 10);
		uni.spawnSquad(t, 3000.0f, 8000.0f, r);
	}

	uni.spawnShip(PLAYER_SHIP, TEAM_PLAYER, vec3());
	uni.getAgents()->back().setMaxHealth(10000.0f, true);
	uni.getAgents()->back().setMaxShield(10000.0f, true);
	uni.getAgents()->back().setMaxEnergy(10000.0f, true);

	uni.getAgents()->back().setGrade(0, 8);
	uni.getAgents()->back().setGrade(1, 12);
	uni.getAgents()->back().setGrade(2, 16);
	uni.getAgents()->back().setGrade(3, 4);
	uni.getAgents()->back().setWeap(1);

	//Timer used to keep track of game time.
	//The argument is the fps of the updates, higher = more detailed.
	sim_time clock(120.0f);
	//Keypress modifiers (shift, ctrl etc).
	//int keymod = 0;
	while(g_GAME_STATE == MODE_MENU)
	{
		//Event handling.
		SDL_Event incomingEvent;
		while( SDL_PollEvent( &incomingEvent ) )
		{
			//Quit event.
			switch( incomingEvent.type )
			{
			case SDL_QUIT:
				g_GAME_STATE = MODE_QUIT;
				break;
			}

			//Input events.
			//This is different to the main game, we only want the user to be able to select from the main menu, i.e no shooting etc.
			switch( incomingEvent.type )
			{
			if( uni.isPaused() and incomingEvent.type != SDL_KEYDOWN and incomingEvent.button.button != SDLK_SPACE ) break;
			case SDL_MOUSEBUTTONDOWN:
				switch(incomingEvent.button.button)
				{
				case SDL_BUTTON_LEFT:
					selectionReturn mainMenuSelected = uni.handleMouseDownInput( getMousePos() );
					std::cout << "CLICK : " << mainMenuSelected.m_sel_val << ", " << mainMenuSelected.m_button_val << std::endl;
					if(mainMenuSelected.m_sel_val == 0)
					{
						switch(mainMenuSelected.m_button_val)
						{
						case 0:
							g_GAME_STATE = MODE_GAME;
							return;
						case 1:
							g_GAME_STATE = MODE_TUTORIAL;
							return;
						case 2:
							for(size_t i = 0; i < uni.getUI()->getElements()->size(); ++i)
							{
								(*uni.getUI()->getElements())[i].toggleVisible();
							}
							break;
						case 3:
							g_GAME_STATE = MODE_QUIT;
							return;
						case 4:
							g_GAME_STATE = MODE_SANDBOX;
							return;
						default:
							break;
						}
					}
					else if(mainMenuSelected.m_sel_val == 1)
					{
						if(mainMenuSelected.m_button_val == 4)
						{
							for(size_t i = 0; i < uni.getUI()->getElements()->size(); ++i)
							{
								(*uni.getUI()->getElements())[i].toggleVisible();
							}
						}
					}
					else if(mainMenuSelected.m_sel_val == 2)
					{
						switch(mainMenuSelected.m_button_val)
						{
						case 0:
							setConfigValue("resolution", vec2(1280, 720));
							break;
						case 1:
							setConfigValue("resolution", vec2(1280, 1024));
							break;
						case 2:
							setConfigValue("resolution", vec2(1366, 768));
							break;
						case 3:
							setConfigValue("resolution", vec2(1440, 900));
							break;
						case 4:
							setConfigValue("resolution", vec2(1600, 900));
							break;
						case 5:
							setConfigValue("resolution", vec2(1920, 1080));
							break;
						case 6:
							setConfigValue("resolution", vec2(1920, 1200));
							break;
						default:
							break;
						}
					}
					else if(mainMenuSelected.m_sel_val == 3)
					{
						setConfigValue("graphical_detail", mainMenuSelected.m_button_val);
					}
					else if(mainMenuSelected.m_sel_val == 4)
					{
						setConfigValue("difficulty", mainMenuSelected.m_button_val + 1);
					}
					break;
				}
				break;
			}
		}

		//Set current time (timer keeps track of time since cur time was last set).
		clock.setCur();

		//Update the game in small time-steps (dependant on the timers fps).
		while(clock.getAcc() > clock.getFrame())
		{
			uni.update(clock.getDiff() * g_TIME_SCALE);
			clock.incrAcc( -clock.getDiff() );
		}

		//Update the zoom level.
		g_ZOOM_LEVEL += (g_TARG_ZOOM_LEVEL - g_ZOOM_LEVEL) * 0.125f;

		//Draw the game.
		float diff_clamped = clock.getDiff();
		if(diff_clamped == 0.0f) diff_clamped = 0.01f;

		uni.clear();
		uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );
		uni.getRenderer()->drawText("CAPTAIN FRACTAL:", "pix90", {g_HALFWIN.m_x - 180.0f, g_HALFWIN.m_y - 350.0f}, false, 1.0f);
		uni.getRenderer()->drawText("ATTACK OF THE SPACE COMMUNISTS", "pix90", {g_HALFWIN.m_x - 340.0f, g_HALFWIN.m_y - 300.0f}, false, 1.0f);
		uni.swap( );
	}
}

void playGame(universe &uni)
{
	//universe uni;
	std::cout << "LAUNCHING MAIN GAME..." << std::endl;

	g_GAME_OVER = false;
	uni.playMus(0);
	uni.reload(true);
	g_ZOOM_LEVEL = 0.0f;
	g_TARG_ZOOM_LEVEL = 0.8f;
	//Timer used to keep track of game time.
	//The argument is the fps of the updates, higher = more detailed.
	sim_time clock(120.0f);

	//uni.spawnBase(ALLIANCE, vec3(2000.0f, 2000.0f, 0.0f), 2, 180.0f);

	//Keypress modifiers (shift, ctrl etc).
	//int keymod = 0;
	std::vector<SDL_Event> events;
	while(g_GAME_STATE == MODE_GAME)
	{
		events = getEvents();
		uni.updateInputs(events);
		//Event handling.
		/*SDL_Event incomingEvent;
				while( SDL_PollEvent( &incomingEvent ) )
				{
						//Quit event.
						switch( incomingEvent.type )
						{
						case SDL_QUIT:
								g_GAME_STATE = MODE_QUIT;
								break;
						}

						//Ignore if the player is dead.
						if(g_GAME_OVER) break;

						//Input events.
						switch( incomingEvent.type )
						{
						if( uni.isPaused() and incomingEvent.type != SDL_KEYDOWN and incomingEvent.button.button != SDLK_SPACE ) break;
						case SDL_MOUSEBUTTONDOWN:
								handleUserMouseDownInput(incomingEvent.button.button, &keymod, uni.getPly(), &uni);
								break;
						case SDL_MOUSEBUTTONUP:
								handleUserMouseUpInput(incomingEvent.button.button, keymod, uni.getPly(), &uni);
								break;
						case SDL_MOUSEWHEEL:
								handleUserScroll(incomingEvent.wheel.y, uni.getPly());
								break;
						case SDL_KEYDOWN:
								//handleUserKeyDownInput(incomingEvent.key.keysym.sym, uni.getPly(), &uni, &keymod);
								break;
						case SDL_KEYUP:
								//handleUserKeyUpInput(incomingEvent.key.keysym.sym, &keymod);
								break;
						}
				}*/
		//Set current time (timer keeps track of time since cur time was last set).
		clock.setCur();

		//Update the game in small time-steps (dependant on the timers fps).
		while(clock.getAcc() > clock.getFrame())
		{
			uni.update(clock.getDiff() * g_TIME_SCALE);
			clock.incrAcc( -clock.getDiff() );
		}

		//Update the zoom level.
		g_ZOOM_LEVEL += (g_TARG_ZOOM_LEVEL - g_ZOOM_LEVEL) * 0.125f;

		//Draw the game.
		float diff_clamped = clock.getDiff();
		if(diff_clamped == 0.0f) diff_clamped = 0.01f;

		uni.clear();
		uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );
		uni.swap();
	}
}

void handleUserMouseDownInput(int btn, int * keymod, player *ply, universe *uni)
{
	if(*keymod == 0)
	{
		if(btn == SDL_BUTTON_LEFT)
		{
			ply->setFiring(true);
		}
		else if(btn == SDL_BUTTON_RIGHT and ply->getMissiles() > 0)
		{
			if(ply->getMissiles() > 0)
			{
				uni->addMissile(ply->getPos(), ply->getVel(), ply->getAng(), TEAM_PLAYER);
				ply->incrMissiles(-1);
			}
		}
	}
	else if(*keymod == 1)
	{
		if(btn == SDL_BUTTON_LEFT)
		{
			selectionReturn ret = uni->handleMouseDownInput( getMousePos() );
			uni->processUIInput(ret/*, *keymod*/);

			/*if(ret.m_sel_val == 0)
						{
								ply->setEnergyPriority(ret.m_button_val);
								uni->getUI()->setDark(ret.m_sel_val, true);
								uni->getUI()->setDark(ret.m_sel_val, ret.m_button_val, false);
						}
						else if(ret.m_sel_val == 1)
						{
								if( !uni->upgradeCallback(ret.m_sel_val, ret.m_button_val) ) return;

								if(ret.m_button_val == 4) ply->incrMissiles(1);
						}
						else if(ret.m_sel_val == 2)
						{
								if( !uni->upgradeCallback(ret.m_sel_val, ret.m_button_val) ) return;

								if(ret.m_button_val == 0) uni->addMiner();
								else if(ret.m_button_val == 1) uni->addWingman();
								else if(ret.m_button_val == 2) uni->setMouseState( PLAYER_TURRET );
								else if(ret.m_button_val == 3) uni->setMouseState( PLAYER_GRAVWELL );
								else if(ret.m_button_val == 4) uni->setMouseState( PLAYER_BARRACKS );
								else if(ret.m_button_val == 5) uni->setMouseState( PLAYER_STATION );
								else if(ret.m_button_val == 6) uni->setMouseState( PLAYER_CAPITAL );
						}
						else if(ret.m_sel_val == 3)
						{
								//Trade menu
								switch(ret.m_button_val)
								{
								case 0:
										break;
								case 1:
										break;
								default:
										break;
								}
						}
						else if(ret.m_sel_val == 4)
						{
								switch(ret.m_button_val)
								{
								case 0:
										*keymod = 0;
										uni->setPause(false);
										uni->escMenuTog();
										uni->getUI()->pop();
										break;
								case 1:
										*keymod = 0;
										saveGame(uni);
										uni->setPause(false);
										uni->escMenuTog();
										uni->getUI()->pop();
										break;
								case 2:
										*keymod = 0;
										uni->reload(true);
										loadGame(uni);
										break;
								case 3:
										g_GAME_STATE = MODE_MENU;
										break;
								case 4:
										g_GAME_STATE = MODE_QUIT;
										break;
								default:
										break;

								}
						}*/
		}
		return;
	}
}

void gameInit()
{
	srand (static_cast <unsigned> (time(0)));

	g_GAME_OVER = false;
}
/*
void handleUserMouseUpInput(int btn, int keymod, player *ply, universe *uni)
{
		if(keymod == 0)
		{
				if(btn == SDL_BUTTON_LEFT)
				{
						ply->setFiring(false);
				}
		}
		else if(keymod == 1)
		{

		}

		selectionReturn ret = uni->getUI()->handleInput( getMousePos() );
		if(ret.m_sel_val > 0) uni->setMouseState(-1);

		vec3 pos = tovec3( toWorldSpace( getMousePos() ) );

		int ms = uni->getMouseState();
		if(ms > -1 and ms != PLAYER_CAPITAL)
		{
				uni->addBuild(pos, static_cast<ship_spec>(uni->getMouseState()), TEAM_PLAYER);
		}
		else if(ms == PLAYER_CAPITAL)
		{
				uni->spawnShip(PLAYER_CAPITAL, TEAM_PLAYER, pos);
		}
		uni->setMouseState(-1);
		uni->mouseUp();
}

void handleUserScroll(int y, player * ply)
{
		if(y < 0)
		{
				ply->incrWeap(1);
		}
		else if(y > 0)
		{
				ply->incrWeap(-1);
		}
}

void handleUserKeyDownInput(int sym, player *ply, universe *uni, int * keymod)
{
		//The menu to appear when the user presses escape.
		//Hard coded so it is fast.
		//Or something like that.
		selection escMenuSelection;
		std::array<int, 4> btncol = {20, 200, 255, 220};
		std::array<int, 4> quitcol = {255, 5, 30, 220};
		std::array<int, 4> white = {255, 255, 255, 255};
		button resume ("RESUME", btncol, white, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 300.0f}, {200.0f, 80.0f});
		button sGame ("SAVE GAME", btncol, white, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f}, {200.0f, 80.0f});
		button lGame ("LOAD GAME", btncol, white, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 100.0f}, {200.0f, 80.0f});
		button mm ("MAIN MENU", quitcol, white, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y}, {200.0f, 80.0f});
		button quit ("QUIT", quitcol, white, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 100.0f}, {200.0f, 80.0f});

		escMenuSelection.add(resume);
		escMenuSelection.add(sGame);
		escMenuSelection.add(lGame);
		escMenuSelection.add(mm);
		escMenuSelection.add(quit);

		switch (sym)
		{
		case SDLK_w:
				ply->accelerate(1);
				uni->addShake(0.5f);
				break;
		case SDLK_s:
				if(*keymod == 1) saveGame(uni);
				else
				{
						ply->accelerate(-1);
						uni->addShake(0.5f);
				}
				break;
		case SDLK_a:
				ply->dodge(1);
				break;
		case SDLK_d:
				ply->dodge(-1);
				break;
		case SDLK_ESCAPE:
				uni->setPause(true);
				uni->escMenuTog();
				if(uni->isEscMenuShown())
				{
						*keymod = 1;
						uni->getUI()->add(escMenuSelection);
				}
				else
				{
						uni->setPause(false);
						*keymod = 0;
						uni->getUI()->pop();
				}
				break;
		case SDLK_EQUALS:
				ply->incrWeap(1);
				break;
		case SDLK_MINUS:
				ply->incrWeap(-1);
				break;
		case SDLK_p:
				g_TARG_ZOOM_LEVEL = clamp(g_TARG_ZOOM_LEVEL + 0.1f, 0.1f, 2.0f);
				break;
		case SDLK_o:
				g_TARG_ZOOM_LEVEL = clamp(g_TARG_ZOOM_LEVEL - 0.1f, 0.1f, 2.0f);
				break;
		case SDLK_LCTRL:
				*keymod = 1;
				break;
		case SDLK_g:
				if(g_DEV_MODE) uni->addScore(100);
				break;
		case SDLK_n:
				if(*keymod == 1)
				{
						uni->reload(true);
						//for(int i = 0; i < UPGRADES_LEN; ++i) setUpgradeTextures(0, i);
				}
				break;
		case SDLK_l:
				if(*keymod == 1)
				{
						uni->reload(true);
						loadGame(uni);
				}
				break;
		case SDLK_m:
				uni->toggleMapMode();
				break;
		case SDLK_SPACE:
				uni->pause();
				break;
		case SDLK_LEFTBRACKET:
				g_TIME_SCALE = clamp(g_TIME_SCALE - 0.05f, 0.0f, 2.0f);
				std::cout << "Timescale " << g_TIME_SCALE << std::endl;
				break;
		case SDLK_RIGHTBRACKET:
				g_TIME_SCALE = clamp(g_TIME_SCALE + 0.05f, 0.0f, 2.0f);
				std::cout << "Timescale " << g_TIME_SCALE << std::endl;
				break;
		case SDLK_i:
				uni->getPly()->toggleInventory();
				break;
		case SDLK_u:
				uni->toggleUIVisible();
				break;
		case SDLK_e:
		{
				ply->decelerate();
				break;
		}
		case SDLK_q:
				ply->accelerate(25.0f);
		case SDLK_z:
		{
				debris temp({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, RESOURCE_IRON);
				uni->getPly()->addItem(temp);
				break;
		}
		case SDLK_k:
				uni->debug_lockPlayer();
				break;
		default:
				break;
		}
}

void handleUserKeyUpInput(int sym, int * keymod)
{
		switch (sym)
		{
		case SDLK_LCTRL:
				*keymod = 0;
				break;
		}
}*/

void playTutorial(universe &uni)
{
	enum stage {
		STAGE_BEGIN, STAGE_BEGIN_UNPAUSED,
		STAGE_MOVEMENT_INTRO, STAGE_MOVEMENT_SLOW,
		STAGE_ZOOM_OUT, STAGE_ZOOM_IN,
		STAGE_SHOOTING,
		STAGE_ENERGY_LEVELS_PAUSE, STAGE_ENERGY_LEVELS, STAGE_ENERGY_LEVELS_2,
		STAGE_ASTEROID_1, STAGE_ASTEROID_2,
		STAGE_UPGRADES,
		STAGE_COMBAT_1, STAGE_COMBAT_2,
		STAGE_CONSTRUCTION_1, STAGE_CONSTRUCTION_2,
		STAGE_COMBAT_3, STAGE_COMBAT_4, STAGE_COMBAT_5,
		STAGE_COMPLETE,
		STAGE_COMBAT_2_DIED, STAGE_4_COMBAT_DIED,
	};
	stage tutStage = STAGE_BEGIN;

	//universe uni;
	std::cout << "LAUNCHING TUTORIAL..." << std::endl;

	g_DIFFICULTY = 0;

	g_GAME_OVER = false;
	uni.playMus(0);
	uni.reload(true);
	uni.setScore(0);
	g_ZOOM_LEVEL = 0.0f;
	g_TARG_ZOOM_LEVEL = 0.8f;
	//Timer used to keep track of game time.
	//The argument is the fps of the updates, higher = more detailed.
	sim_time clock(120.0f);
	std::array<bool, 4> directionsTravelled = {{false, false, false, false}};
	float timer = 0.0f;
	std::vector<SDL_Event> events;
	//Keypress modifiers (shift, ctrl etc).
	//int keymod = 0;
	while(g_GAME_STATE == MODE_TUTORIAL)
	{
		events = getEvents();
		for( auto &incomingEvent : events )
		{

			//Quit event.
			/*switch( incomingEvent.type )
						{
						case SDL_QUIT:
								g_GAME_STATE = MODE_QUIT;
								break;
						}*/

			//Ignore if the player is dead.
			if(g_GAME_OVER) break;

			//Input events.
			switch( incomingEvent.type )
			{
			if( uni.isPaused() and incomingEvent.type != SDL_KEYDOWN and incomingEvent.button.button != SDLK_SPACE ) break;
			case SDL_MOUSEBUTTONDOWN:
				switch(incomingEvent.button.button)
				{
				case SDL_BUTTON_LEFT:
					if(tutStage == STAGE_SHOOTING and !uni.isPaused())
					{
						timer = 0.0f;
						tutStage = STAGE_ENERGY_LEVELS_PAUSE;
					}
				default:
					break;
				}
				//handleUserMouseDownInput(incomingEvent.button.button, &keymod, uni.getPly(), &uni);
				if(tutStage == STAGE_ENERGY_LEVELS_2 and uni.getPly()->getEnergyPriority() == 3)
				{
					tutStage = STAGE_ASTEROID_1;
					timer = 0.0f;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				//handleUserMouseUpInput(incomingEvent.button.button, keymod, uni.getPly(), &uni);
				break;
			case SDL_MOUSEWHEEL:
				//handleUserScroll(incomingEvent.wheel.y, uni.getPly());
				break;
			case SDL_KEYDOWN:
				switch(incomingEvent.key.keysym.sym)
				{
				case SDLK_w:
					if(tutStage == STAGE_MOVEMENT_INTRO) directionsTravelled[0] = true;
					break;
				case SDLK_a:
					if(tutStage == STAGE_MOVEMENT_INTRO) directionsTravelled[1] = true;
					break;
				case SDLK_s:
					if(tutStage == STAGE_MOVEMENT_INTRO) directionsTravelled[2] = true;
					break;
				case SDLK_d:
					if(tutStage == STAGE_MOVEMENT_INTRO) directionsTravelled[3] = true;
					break;
				case SDLK_SPACE:
					if(tutStage == STAGE_BEGIN_UNPAUSED)
					{
						tutStage = STAGE_MOVEMENT_INTRO;
						timer = 0.0f;
					}
					break;
				default:
					break;
				}
				//handleUserKeyDownInput(incomingEvent.key.keysym.sym, uni.getPly(), &uni, &keymod);
				break;
			case SDL_KEYUP:
				//handleUserKeyUpInput(incomingEvent.key.keysym.sym, &keymod);
				break;
			}
		}
		uni.updateInputs(events);

		//Set current time (timer keeps track of time since cur time was last set).
		clock.setCur();

		timer += clock.getDiff() * g_TIME_SCALE;
		//Update the game in small time-steps (dependant on the timers fps).
		while(clock.getAcc() > clock.getFrame())
		{
			uni.update(clock.getDiff() * g_TIME_SCALE);
			clock.incrAcc( -clock.getDiff() );
		}

		//Update the zoom level.
		g_ZOOM_LEVEL += (g_TARG_ZOOM_LEVEL - g_ZOOM_LEVEL) * 0.125f;

		if(tutStage == STAGE_ZOOM_IN or tutStage == STAGE_ZOOM_OUT)
		{
			g_TARG_ZOOM_LEVEL = clamp(g_TARG_ZOOM_LEVEL, 0.3f, 1.0f);
			g_ZOOM_LEVEL = clamp(g_ZOOM_LEVEL, 0.3f, 1.0f);
		}

		//Draw the game.
		float diff_clamped = clock.getDiff();
		if(diff_clamped == 0.0f) diff_clamped = 0.01f;

		uni.clear();
		uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );

		if(tutStage == STAGE_BEGIN and timer > 1.0f)
		{
			uni.setPause(true);
			tutStage = STAGE_BEGIN_UNPAUSED;
		}
		else if(tutStage == STAGE_BEGIN_UNPAUSED)
		{
			uni.getRenderer()->drawText("WELCOME TO CAPTAIN FRACTAL: ATTACK OF THE SPACE COMMUNISTS", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 210.0f}, false, 2.0f);
			uni.getRenderer()->drawText("THIS TUTORIAL WILL GUIDE YOU THROUGH SOME OF THE GAMES SYSTEMS", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
			uni.getRenderer()->drawText("PRESS 'SPACE' TO UNPAUSE", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
		}
		else if(tutStage == STAGE_MOVEMENT_INTRO)
		{
			vec2 ppos = tovec2(uni.getPly()->getPos());
			float ang = uni.getPly()->getAng();
			uni.getRenderer()->drawText("OKAY, FIRST UP, MOVEMENT:", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			uni.getRenderer()->drawText("YOU ALWAYS CHASE THE MOUSE", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
			uni.getRenderer()->drawText("TRY MOVING IN ALL FOUR DIRECTIONS", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
			if(!directionsTravelled[0]) uni.getRenderer()->drawText("W", "pix", ppos + vec(ang + 90) * 100.0f, true, 2.0f);
			if(!directionsTravelled[1]) uni.getRenderer()->drawText("A", "pix", ppos + vec(ang) * 100.0f, true, 2.0f);
			if(!directionsTravelled[2]) uni.getRenderer()->drawText("S", "pix", ppos + vec(ang - 90) * 100.0f, true, 2.0f);
			if(!directionsTravelled[3]) uni.getRenderer()->drawText("D", "pix", ppos + vec(ang + 180) * 100.0f, true, 2.0f);
			if(std::all_of(
					 std::begin(directionsTravelled),
					 std::end(directionsTravelled),
					 [](bool i)
			{
					 return i;
		}) and timer > 2.0f)
			{
				tutStage = STAGE_MOVEMENT_SLOW;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_MOVEMENT_SLOW)
		{
			uni.getRenderer()->drawText("HOLD 'E' TO SLOW DOWN TO A STOP", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			if(magns(uni.getPly()->getVel()) < 0.2f) tutStage = STAGE_ZOOM_OUT;
		}
		else if(tutStage == STAGE_ZOOM_OUT)
		{
			uni.getRenderer()->drawText("HOLD 'O' TO ZOOM OUT", "pix", {200.0f, -300.0f}, true, 2.0f);
			uni.getRenderer()->drawText("AND 'P' TO ZOOM BACK IN", "pix", {-2000.0f, 2000.0f}, true, 20.0f);
			if(g_ZOOM_LEVEL <= 0.4f) tutStage = STAGE_ZOOM_IN;
		}
		else if(tutStage == STAGE_ZOOM_IN)
		{
			uni.getRenderer()->drawText("AND   'P'   TO   ZOOM   BACK   IN", "pix", {-2000.0f, 500.0f}, true, 20.0f);
			if(g_ZOOM_LEVEL >= 0.8f) tutStage = STAGE_SHOOTING;
		}
		else if(tutStage == STAGE_SHOOTING and timer > 1.5f)
		{
			uni.getRenderer()->drawText("OF COURSE, A SPACE GAME WITHOUT LASERS WOULD BE TERRIBLE", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			uni.getRenderer()->drawText("PRESSING LMB WILL SHOOT IN THE DIRECTION YOU ARE FACING", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
			uni.getRenderer()->drawText("TRY IT OUT!", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
		}
		else if(tutStage == STAGE_ENERGY_LEVELS_PAUSE and timer > 1.0f)
		{
			tutStage = STAGE_ENERGY_LEVELS;
		}
		else if(tutStage == STAGE_ENERGY_LEVELS)
		{
			uni.getRenderer()->drawText("PRETTY COOL, RIGHT?", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			uni.getRenderer()->drawText("YOU CAN'T JUST SHOOT FOREVER THOUGH; YOU HAVE TO THINK ABOUT YOUR HEALTH, SHIELDS AND ENERGY", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
			uni.getRenderer()->drawText("YOU CAN SEE ALL THIS IN THE TOP-LEFT", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
			if(timer > 10.0f)
			{
				tutStage = STAGE_ENERGY_LEVELS_2;
			}
		}
		else if(tutStage == STAGE_ENERGY_LEVELS_2)
		{
			uni.getRenderer()->drawText("THE RED BAR IS YOUR HEALTH, BLUE IS SHIELDS, AND GREEN IS ENERGY", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			uni.getRenderer()->drawText("SHOOTING, MOVING AND RECHARGING SHIELDS ALL USE ENERGY, SO YOU HAVE TO BE CAREFUL", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
			uni.getRenderer()->drawText("USE THE buttonS ON THE RIGHT TO PRIORITISE CERTAIN SYSTEMS", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
			uni.getRenderer()->drawText("CTRL CLICK SET THE PRIORITY TO 'GUNS'", "pix", {g_HALFWIN.m_x, g_HALFWIN.m_y - 80.0f}, false, 1.2f);
		}
		else if(tutStage == STAGE_ASTEROID_1 and timer > 1.0f)
		{
			ship asteroid( g_ship_templates[ASTEROID_SMALL] );
			asteroid.setPos(vec3(0.0f - 300.0f, 0.0f, 0.0f));
			asteroid.setVel(uni.getPly()->getVel());
			asteroid.setHealth(10);
			uni.playUISnd(PLACE_SND);
			uni.getAsteroids()->push_back(asteroid);
			tutStage = STAGE_ASTEROID_2;
			timer = 0.0f;
		}
		else if(tutStage == STAGE_ASTEROID_2)
		{
			if(uni.getAsteroids()->size() > 0)
			{
				vec3 tPos = (*uni.getAsteroids())[0].getPos();
				uni.getRenderer()->drawText("NOW BLOW UP THIS ROCK", "pix", {tPos.m_x + 100, tPos.m_y}, true, 2.0f);
			}

			if(uni.getAsteroids()->size() == 0 and uni.getScore() >= 10)
			{
				tutStage = STAGE_UPGRADES;
				timer = 0.0f;
			}
			else if(uni.getAsteroids()->size() == 0 and uni.getScore() < 10)
			{
				tutStage = STAGE_ASTEROID_1;
			}
		}
		else if(tutStage == STAGE_UPGRADES)
		{
			if(timer < 4.0f)
			{
				uni.getRenderer()->drawText("WHOA, YOU GOT TEN POINTS!", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else
			{
				uni.getRenderer()->drawText("YOU CAN USE YOUR POINTS TO IMPROVE YOUR SHIP, AND TO BUY STRUCTURES AND WINGMEN", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.0f);
				uni.getRenderer()->drawText("YOU CAN GET POINTS BY BLOWING UP ASTEROIDS OR ENEMY SHIPS", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
				uni.getRenderer()->drawText("CTRL-CLICK ON THE LOWER MENU TO GET YOURSELF SOME UPGRADES", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
			}

			if(uni.getScore() < 4)
			{
				tutStage = STAGE_COMBAT_1;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_COMBAT_1)
		{
			if(timer < 3.0f)
			{
				uni.getRenderer()->drawText("UH-OH! IT LOOKS LIKE SOME SPACE PIRATES ARE COMING TO STEAL YOUR BOOTY", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else if(timer >= 3.0f and timer < 6.0f)
			{
				uni.getRenderer()->drawText("WE'D BETTER SHOW THEM WHO'S BOSS", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.0f);
			}
			else if(timer > 6.0f)
			{
				uni.spawnSquad(SPOOKY_SPACE_PIRATES, 12000.0f, 18000.0f, 2);
				for(auto &i : (uni.getAgents()->m_objects)) i.setMaxShield(10.0f, true);
				tutStage = STAGE_COMBAT_2;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_COMBAT_2)
		{
			if(timer < 2.0f)
			{
				uni.getRenderer()->drawText("SCROLL TO SWITCH WEAPON!", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.0f);
			}
			else if(timer > 2.0f and timer < 4.5f)
			{
				uni.getRenderer()->drawText("USE THE MINIMAP TO LOCATE ENEMIES!", "pix", {g_WIN_WIDTH - 512.0f, 200.0f}, false, 1.0f);
			}

			if(uni.getPly()->getHealth() <= 0.0f)
			{
				tutStage = STAGE_COMBAT_2_DIED;
				timer = 0.0f;
			}
			else if(uni.getAgents()->size() == 0)
			{
				tutStage = STAGE_CONSTRUCTION_1;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_CONSTRUCTION_1)
		{
			if(timer > 1.5f and uni.getScore() < 5000)
			{
				uni.addScore(10);
			}

			if(timer < 1.5f)
			{
				uni.getRenderer()->drawText("WELL DONE!", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else if(timer > 1.5f and timer < 3.0f)
			{
				uni.getRenderer()->drawText("RIGHT, LET'S BUILD A BASE!", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else if(timer > 3.0f and uni.getScore() < 5000)
			{
				uni.getRenderer()->drawText("ADDING POINTS...", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else
			{
				tutStage = STAGE_CONSTRUCTION_2;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_CONSTRUCTION_2)
		{
			if(uni.getScore() > 16)
			{
				uni.getRenderer()->drawText("YOU CAN CTRL CLICK AND DRAG STRUCTURES FROM THE CYAN MENU INTO THE WORLD", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.0f);
				uni.getRenderer()->drawText("YOU CAN ALSO BUY WINGMEN, AND MINERS, WHO WILL AUTOMATICALLY DESTROY ASTEROIDS", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false, 1.0f);
				uni.getRenderer()->drawText("MAKE YOURSELF A BASE", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false, 1.0f);
			}
			else
			{
				tutStage = STAGE_COMBAT_3;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_COMBAT_3)
		{
			if(timer < 3.0f)
			{
				uni.getRenderer()->drawText("WELL, LET'S SEE HOW WELL YOUR BASE STANDS UP", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else if(timer >= 3.0f and timer < 6.0f)
			{
				uni.getRenderer()->drawText("GET READY TO FIGHT!", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.0f);
			}
			else if(timer > 6.0f)
			{
				for(int i = 0; i < 2; ++i)
				{
					uni.spawnSquad(GALACTIC_FEDERATION, 16000.0f, 20000.0f, rand()%10 + 10);
				}
				tutStage = STAGE_COMBAT_4;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_COMBAT_4)
		{
			if(timer > 120.0f)
			{
				for(int i = 0; i < 2; ++i)
				{
					uni.spawnSquad(SPOOKY_SPACE_PIRATES, 16000.0f, 20000.0f, rand()%10 + 10);
				}
				for(int i = 0; i < 1; ++i)
				{
					uni.spawnSquad(TEAM_PLAYER, 30000.0f, 40000.0f, rand()%10 + 10);
				}
				tutStage = STAGE_COMBAT_5;
			}
		}
		else if(tutStage == STAGE_COMBAT_5)
		{
			if(uni.getFactionCount(GALACTIC_FEDERATION) == 0 and uni.getFactionCount(SPOOKY_SPACE_PIRATES) == 0)
			{
				tutStage = STAGE_COMPLETE;
				timer = 0.0f;
			}
		}
		else if(tutStage == STAGE_COMPLETE)
		{
			if(timer < 5.0f)
			{
				uni.getRenderer()->drawText("YOU HAVE COMPLETED THE TUTORIAL, WELL DONE!", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
				uni.getRenderer()->drawText("SPACE STALIN MAY HAVE FOUND HIS MATCH AT LAST.", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 100.0f}, false, 1.0f);
			}
			else
			{
				g_GAME_STATE = MODE_MENU;
			}
		}
		else if(tutStage == STAGE_COMBAT_2_DIED)
		{
			if(timer < 3.0f)
			{
				uni.getRenderer()->drawText("OH, IT LOOKS LIKE THINGS DIDN'T QUITE WORK OUT", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else if(timer > 3.0f and uni.getPly()->getHealth() <= 0.0f)
			{
				uni.getPly()->setMaxHealth(100.0f, true);
				uni.getPly()->setMaxShield(100.0f, true);
				uni.getPly()->setMaxEnergy(100.0f, true);
				g_GAME_OVER = false;
				uni.reload(false);
			}
			else if(timer < 6.0f)
			{
				uni.getRenderer()->drawText("RIGHT, LET'S GIVE THAT ANOTHER GO, SHALL WE?", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false, 1.2f);
			}
			else
			{
				tutStage = STAGE_COMBAT_1;
				timer = 10.0f;
			}
		}

		uni.swap( );
	}
}

void sandbox(universe &uni)
{

	g_GAME_OVER = true;
	g_DIFFICULTY = 0;
	uni.playMus(0);
	uni.reload(true);
	uni.getAgents()->clear();
	uni.getPly()->setHealth(-1.0f);
	uni.getPly()->setShield(-1.0f);
	uni.getPly()->setMaxEnergy(100000.0f, true);
	uni.getPly()->setGeneratorMul(100000.0f);
	uni.getPly()->setEnginePower(50.0f);
	uni.getPly()->setAng(0.0f);
	uni.update(0.1f);
	uni.getUI()->clear();

	selection ships;
	vec2 pos = {0.0f, 0.0f};
	for(auto &ship : g_ship_templates)
	{
		button temp (ship.getIdentifier(), {{0, 255, 255, 200}}, {{255, 255, 255, 255}}, pos, vec2(128.0f, 64.0f), 0.75f);
		ships.add(temp);

		if(pos.m_y < g_WIN_HEIGHT - 64.0f)
		{
			pos.m_y += 80.0f;
		}
		else
		{
			pos.m_y = 0.0f;
			pos.m_x += 140.0f;
		}
	}

	button temp ("CLEAR", {{255, 0, 0, 200}}, {{255, 255, 255, 255}}, pos, vec2(128.0f, 64.0f), 0.75f);
	if(pos.m_y < g_WIN_HEIGHT - 64.0f)
	{
		pos.m_y += 80.0f;
	}
	else
	{
		pos.m_y = 0.0f;
		pos.m_x += 140.0f;
	}
	button exit ("MAIN MENU", {{255, 0, 0, 200}}, {{255, 255, 255, 255}}, pos, vec2(128.0f, 64.0f), 0.75f);

	ships.add(temp);
	ships.add(exit);
	uni.getUI()->add(ships);

	//Timer used to keep track of game time.
	//The argument is the fps of the updates, higher = more detailed.
	sim_time clock(120.0f);
	//Keypress modifiers (shift, ctrl etc).
	std::vector<SDL_Event> events;
	//int keymod = 0;
	while(g_GAME_STATE == MODE_SANDBOX)
	{
		events = getEvents();
		uni.getPly()->setVel( uni.getPly()->getVel() * 0.95f );

		//Event handling.
		for( auto &incomingEvent : events )
		{
			//Input events.
			//This is different to the main game, we only want the user to be able to select from the main menu, i.e no shooting etc.
			switch( incomingEvent.type )
			{
			if( uni.isPaused() and incomingEvent.type != SDL_KEYDOWN and incomingEvent.button.button != SDLK_SPACE ) break;
			case SDL_MOUSEBUTTONDOWN:
				switch(incomingEvent.button.button)
				{
				case SDL_BUTTON_LEFT:
					int mx = 0, my = 0;
					SDL_GetMouseState(&mx, &my);
					selectionReturn mainMenuSelected = uni.handleMouseDownInput({static_cast<float>(mx), static_cast<float>(my)});
					std::cout << "sandbox click! " << mainMenuSelected.m_button_val << ", " << mainMenuSelected.m_sel_val << '\n';
					if(mainMenuSelected.m_sel_val == 0)
					{
						if(mainMenuSelected.m_button_val < SHIPS_END) uni.setMouseState(mainMenuSelected.m_button_val);
						else if(mainMenuSelected.m_button_val == SHIPS_END) uni.getAgents()->clear();
						else g_GAME_STATE = MODE_MENU;
					}
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
			{
				if(uni.getMouseState() < 0 or uni.getMouseState() > SHIPS_END) break;
				aiTeam team = NONE;
				ship_spec spec = static_cast<ship_spec>(uni.getMouseState());

				if(spec >= COMMUNIST_1 and spec < FEDERATION_MKI) team = SPACE_COMMUNISTS;
				else if(spec >= FEDERATION_MKI and spec < PIRATE_GNAT) team = GALACTIC_FEDERATION;
				else if(spec >= PIRATE_GNAT and spec < ALLIANCE_SCOUT) team = SPOOKY_SPACE_PIRATES;
				else if(spec >= ALLIANCE_SCOUT and spec < PLAYER_HUNTER) team = ALLIANCE;
				else if(spec >= PLAYER_HUNTER and spec < ASTEROID_SMALL) team = TEAM_PLAYER;
				std::cout << "adding spec " << spec << '\n';
				uni.spawnShip(spec, team, tovec3( toWorldSpace(getMousePos()) ));
				std::cout << "post\n";
				uni.setMouseState(-1);
				break;
			}
			}
		}
		uni.updateInputs(events);

		//Set current time (timer keeps track of time since cur time was last set).
		clock.setCur();

		//Update the game in small time-steps (dependant on the timers fps).
		while(clock.getAcc() > clock.getFrame())
		{
			uni.update(clock.getDiff() * g_TIME_SCALE);
			clock.incrAcc( -clock.getDiff() );
		}

		//Update the zoom level.
		g_ZOOM_LEVEL += (g_TARG_ZOOM_LEVEL - g_ZOOM_LEVEL) * 0.125f;

		//Draw the game.
		float diff_clamped = clock.getDiff();
		if(diff_clamped == 0.0f) diff_clamped = 0.01f;

		uni.clear();
		uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );
		uni.swap( );
	}
}

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11

g++ -Wall -o "%e"  -L usr\bin\lib "%f" -I usr\bin\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11
*/

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include  C:\Users\Ben\Documents\Programming\projects\CA1\ed2\src C:\Users\Ben\Documents\Programming\projects\CA1\ed2\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11 
*/
