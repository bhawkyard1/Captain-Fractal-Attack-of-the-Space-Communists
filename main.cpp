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

//SDL headers
#include "SDL2/SDL.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

//Including this causes conflicts with ngl, since this includes gl.h before it is supposed to be included or something.
//#include <SDL2/SDL_opengl.h>

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
void handleUserKeyDownInput(int, player*, universe*, int*);
void handleUserKeyUpInput(int,int*);
void handleUserMouseDownInput(int,int,player*,universe*);
void handleUserMouseUpInput(int,int,player*,universe*);
void handleUserScroll(int,player*);
double diffClock(clock_t clock1, clock_t clock2);

int main(int argc, char* argv[])
{
  std::cout << "ELITE DANGEROUS v2.0 INITIALISING..." << std::endl;

  gameInit();

  //Create the universe.
  universe uni;

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
      if(g_GAME_OVER) break;

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
      uni.update(clock.getDiff() * g_TIME_SCALE);
      clock.incrAcc( -clock.getDiff() );
    }

    //Update the zoom level.
    g_ZOOM_LEVEL += (g_TARG_ZOOM_LEVEL - g_ZOOM_LEVEL) * 0.125f;

    //Draw the game.
    float diff_clamped = clock.getDiff();
    if(diff_clamped == 0.0f) diff_clamped = 0.01f;
    uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );
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
      if(ply->getMissiles() > 0)
      {
        uni->addMissile(ply->getPos(), ply->getVel(), ply->getAng(), TEAM_PLAYER);
        ply->incrMissiles(-1);
      }
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
        if(ret.m_button_val == 5) uni->addMiner();
        else if(ret.m_button_val == 6) uni->addWingman();
        else if(ret.m_button_val == 7) uni->setMouseState(7);
        else if(ret.m_button_val == 8) uni->setMouseState(8);
        else if(ret.m_button_val == 9) uni->setMouseState(9);
        else if(ret.m_button_val == 10) uni->setMouseState(10);
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
  int mx = 0, my = 0;
  SDL_GetMouseState(&mx, &my);

  selectionReturn ret = uni->getUI()->handleInput({static_cast<float>(mx), static_cast<float>(my)});
  if(ret.m_sel_val > 0) uni->setMouseState(-1);

  vec2 pos = {static_cast<float>(mx), static_cast<float>(my)};
  pos -= g_HALFWIN;
  pos /= g_ZOOM_LEVEL;

  switch(uni->getMouseState())
  {
  case 7:
    uni->addBuild(pos, PLAYER_TURRET);
    break;
  case 8:
    uni->addBuild(pos, PLAYER_GRAVWELL);
    break;
  case 9:
    uni->addBuild(pos, PLAYER_BARRACKS);
    break;
  case 10:
    uni->addBuild(pos, PLAYER_STATION);
    break;
  default:
    break;
  }
  uni->setMouseState(-1);
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
      //uni->addParticleSprite( ply->getPos(), ply->getVel(), 0.0f, "EXPLOSION");
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
    if(!g_DEV_MODE) break;
    ply->setVel({0,0});
    ply->setPos({g_WIN_WIDTH/2.0f,g_WIN_HEIGHT/2.0f});
    uni->setVel(-ply->getVel());
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
