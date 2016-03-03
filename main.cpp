//General headers
#include <iostream>
#include <math.h>
#include <vector>
#include <time.h>
#include <cstdlib>

//Used for string/file operations
#include <string>
#include <fstream>
#include <sstream>

//openGL headers
//#include <GL/glew.h>
//#include <GL/glu.h>

//SDL headers
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_opengl.h>

//Keeps track of time
#include "sim_time.hpp"

//Used for all vector math
#include "vectors.hpp"

//General utility library
#include "util.hpp"
//#include "utility/rendering.hpp"

//Contains program-wide variables
#include "common.hpp"

//Sound effects
#include "sfx.hpp"

//Universe class, main class in game
#include "universe.hpp"

#include "ui/interface.hpp"

//Contains funtions to save/load the game
#include "file.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

//Function prototypes.
void gameInit();
void handleUserKeyDownInput(int,player*,universe*,int*);
void handleUserKeyUpInput(int,int*);
void handleUserMouseDownInput(int,int,player*,universe*);
void handleUserMouseUpInput(int,int,player*,universe*);
void handleUserScroll(int,player*);
double diffClock(clock_t clock1, clock_t clock2);

int main(int argc, char* argv[])
{
    gameInit();

    //Create the universe.
    universe uni;

    //Initialise menus, textures, ship upgrades, and create the default ship presets.
    //menus_init();
    //upgrades_init();
    //loadTextures();
    loadShips();
	
    //Timer used to keep track of game time.
    //The argument is the fps of the updates, higher = more detailed.
    sim_time clock(120.0f);
	
    //Is the game running?
	bool active = true;
	
  //Keypress modifiers (shift, ctrl etc).
  int keymod = 0;
	while(active == true)
  {
    //Event handling.
    SDL_Event incomingEvent;
    while( SDL_PollEvent( &incomingEvent ) )
    {
      //Quit event.
      switch( incomingEvent.type )
      {
        case SDL_QUIT:
          active = false;
          break;
      }

      //Ignore if the player is dead.
      if(GAME_OVER) break;

      //Input events.
      switch( incomingEvent.type )
      {
        if( uni.isPaused() and incomingEvent.type != SDL_KEYDOWN and incomingEvent.button.button != SDLK_SPACE ) break;
        case SDL_MOUSEBUTTONDOWN:
          handleUserMouseDownInput(incomingEvent.button.button, keymod, uni.getPly(), &uni);
          break;
        case SDL_MOUSEBUTTONUP:
          handleUserMouseUpInput(incomingEvent.button.button, keymod, uni.getPly(), &uni);
          break;
        case SDL_MOUSEWHEEL:
          handleUserScroll(incomingEvent.wheel.y, uni.getPly());
          break;
        case SDL_KEYDOWN:
          handleUserKeyDownInput(incomingEvent.key.keysym.sym, uni.getPly(), &uni, &keymod);
          break;
        case SDL_KEYUP:
          handleUserKeyUpInput(incomingEvent.key.keysym.sym, &keymod);
          break;
      }
    }
    //Set current time (timer keeps track of time since cur time was last set).
    clock.setCur();

    //Update the game in small time-steps (dependant on the timers fps).
    while(clock.getAcc() > clock.getFrame())
    {
      uni.update(clock.getDiff() * TIME_SCALE);
      clock.incrAcc( -clock.getDiff() );
    }

    //Update the zoom level.
    ZOOM_LEVEL += (TARG_ZOOM_LEVEL - ZOOM_LEVEL) * 0.125f;

    //Draw the game.
    float diff_clamped = clock.getDiff();
    if(diff_clamped == 0.0f) diff_clamped = 0.01f;
    uni.draw( clock.getAcc() / diff_clamped * TIME_SCALE );
  }
  SDL_Quit();

  return 0;
}

void handleUserMouseDownInput(int btn, int keymod, player *ply, universe *uni)
{
  if(keymod == 0)
  {
    if(btn == SDL_BUTTON_LEFT)
    {
      ply->setFiring(true);

    }
    else if(btn == SDL_BUTTON_RIGHT and ply->getMissiles() > 0)
    {
      int x = 0, y = 0;
      SDL_GetMouseState(&x,&y);
      uni->addMissile(ply->getPos(), ply->getVel(), ply->getAng(), 1);
    }
  }
  else if(keymod == 1)
  {
    if(btn == SDL_BUTTON_LEFT)
    {
      int x = 0, y = 0;
      SDL_GetMouseState(&x,&y);

      selectionReturn ret = uni->getUI()->handleInput({static_cast<float>(x), static_cast<float>(y)});

      if(ret.m_sel_val == 0)
      {
        ply->setEnergyPriority(ret.m_button_val);
      }
      else if(ret.m_sel_val == 1)
      {
        if( !uni->upgradeCallback(ret.m_sel_val, ret.m_button_val) ) return;
        ply->upgrade(ret.m_button_val);
        //playerUpgrade( ply->getUpgrade(ret.m_button_val) );
        if(ret.m_button_val == 5) uni->addMiner();
        else if(ret.m_button_val == 6) uni->addWingman();
        else if(ret.m_button_val == 7) uni->addBuild(ply->getPos(), PLAYER_TURRET);
        else if(ret.m_button_val == 8) uni->addBuild(ply->getPos(), PLAYER_GRAVWELL);
        else if(ret.m_button_val == 9) uni->addBuild(ply->getPos(), PLAYER_BARRACKS);
        else if(ret.m_button_val == 10) uni->addBuild(PLAYER_STATION);
      }
    }
    return;
  }
}

void gameInit()
{
	#ifdef _WIN32
    AllocConsole() ;
    AttachConsole( GetCurrentProcessId() );
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stdin );
    freopen( "CON", "w", stderr );
	#endif
	
	loadConfig();

	loadSounds();
	
	srand (static_cast <unsigned> (time(0)));
}

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
  //int ang = ply->getAng();

  switch (sym)
  {
    case SDLK_w:
      ply->accelerate(1);
      uni->setVel(-ply->getVel());
      break;
    case SDLK_s:
      if(*keymod == 1) saveGame(uni);
      else
      {
        ply->accelerate(-1);
        uni->setVel(-ply->getVel());
        uni->addParticleSprite( ply->getPos(), ply->getVel(), 0.0f, "EXPLOSION");
      }
      break;
    case SDLK_a:
      ply->dodge(1);
      uni->setVel(-ply->getVel());
      break;
    case SDLK_d:
      ply->dodge(-1);
      uni->setVel(-ply->getVel());
      break;
    case SDLK_ESCAPE:
      if(!DEV_MODE) break;
      ply->setVel({0,0});
      ply->setPos({WIN_WIDTH/2.0f,WIN_HEIGHT/2.0f});
      uni->setVel(-ply->getVel());
      break;
    case SDLK_EQUALS:
      ply->incrWeap(1);
      break;
    case SDLK_MINUS:
      ply->incrWeap(-1);
      break;
    case SDLK_p:
      TARG_ZOOM_LEVEL = clamp(TARG_ZOOM_LEVEL + 0.1f, 0.1f, 2.0f);
      break;
    case SDLK_o:
      TARG_ZOOM_LEVEL = clamp(TARG_ZOOM_LEVEL - 0.1f, 0.1f, 2.0f);
      break;
    case SDLK_LCTRL:
      *keymod = 1;
      break;
    case SDLK_g:
      if(DEV_MODE) *uni->getScorePt() += 100;
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
        for(int i = 0; i < UPGRADES_LEN; ++i)
        {
          uni->upgradeSetLabels(0, i);
        }
      }
      break;
    case SDLK_SPACE:
      uni->pause();
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
}


/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11

g++ -Wall -o "%e"  -L usr\bin\lib "%f" -I usr\bin\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11
*/

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include  C:\Users\Ben\Documents\Programming\projects\CA1\ed2\src C:\Users\Ben\Documents\Programming\projects\CA1\ed2\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11 
*/
