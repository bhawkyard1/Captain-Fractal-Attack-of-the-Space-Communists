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
#include "src/sim_time.cpp"

//Used for all vector math
#include "vectors.hpp"

//General utility library
#include "include/util.hpp"
//#include "utility/rendering.hpp"

//Contains program-wide variables
#include "common.hpp"

//Sound effects
#include "sfx.hpp"

//Universe class, main class in game
#include "src/universe.cpp"

//All the UI code
#include "ui_classes.hpp"
#include "ui.hpp"

//Contains funtions to save/load the game
#include "file.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

SDL_Window * gameInit();
void handleUserKeyDownInput(int,player*,universe*,int*);
void handleUserKeyUpInput(int,int*);
void handleUserMouseDownInput(int,int,player*,universe*);
void handleUserMouseUpInput(int,int,player*,universe*);
void handleUserScroll(int,player*);
double diffClock(clock_t clock1, clock_t clock2);

void setDifficulty();

int main(int argc, char* argv[])
{
	SDL_Window * window = gameInit();
	if(!window) return 1;
	
	universe uni;
	transmitPointers(&uni);
	menus_init();
	upgrades_init();
	loadTextures();
	loadShips();
	int loopCount = 0;
	
	sim_time clock(60.0f);
	
	bool active = true;
	
	int mod = 0;
	while(active == true)
  {
    sim_time profiler(0.0f);

    SDL_Event incomingEvent;
    while( SDL_PollEvent( &incomingEvent ) )
    {
      switch( incomingEvent.type )
      {
        case SDL_QUIT:
          active = false;
          break;
      }

      if(GAME_OVER) break;

      switch( incomingEvent.type )
      {
        if( uni.isPaused() and incomingEvent.type != SDL_KEYDOWN and incomingEvent.button.button != SDLK_SPACE ) break;
        case SDL_MOUSEBUTTONDOWN:
          handleUserMouseDownInput(incomingEvent.button.button, mod, uni.getPly(), &uni);
          break;
        case SDL_MOUSEBUTTONUP:
          handleUserMouseUpInput(incomingEvent.button.button, mod, uni.getPly(), &uni);
          break;
        case SDL_MOUSEWHEEL:
          handleUserScroll(incomingEvent.wheel.y, uni.getPly());
          break;
        case SDL_KEYDOWN:
          handleUserKeyDownInput(incomingEvent.key.keysym.sym, uni.getPly(), &uni, &mod);
          break;
        case SDL_KEYUP:
          handleUserKeyUpInput(incomingEvent.key.keysym.sym, &mod);
          break;
      }
    }
    SDL_SetRenderDrawColor( renderer, 4, 1, 8, 255);
    SDL_RenderClear(renderer);

    clock.setCur();

    /*if(DEV_MODE)
    {
      profiler.setCur();
    }*/

    while(clock.getAcc() > clock.getFrame())
    {
      uni.update(loopCount, clock.getDiff() * TIME_SCALE);
      clock.incrAcc( -clock.getDiff() );
    }

    /*if(DEV_MODE)
    {
      profiler.setCur();
      cout << std::fixed << "Game updates complete: " << profiler.getDiff() << std::endl;
      profiler.setCur();
    }*/

    ZOOM_LEVEL += (TARG_ZOOM_LEVEL - ZOOM_LEVEL) * 0.125f;

    float diff_clamped = clock.getDiff();
    if(diff_clamped == 0.0f) diff_clamped = 0.01f;
    uni.draw( clock.getAcc() / diff_clamped * TIME_SCALE );

    /*if(DEV_MODE)
    {
      profiler.setCur();
      cout << std::fixed << "Draw updates complete: " << profiler.getDiff() << std::endl;
      profiler.setCur();
    }*/

    if(!GAME_OVER) drawUI( uni.getScore() );

    /*if(DEV_MODE)
    {
      profiler.setCur();
      cout << std::fixed << "UI draw updates complete: " << profiler.getDiff() << std::endl;
      profiler.setCur();
    }*/

    SDL_RenderPresent(renderer);

    loopCount++;
    if(loopCount > 100)
    {
      loopCount = 0;
    }
  }
  SDL_DestroyWindow( window );
  SDL_Quit();

  return 0;
}

void handleUserMouseDownInput(int btn, int mod, player *ply, universe *uni)
{
  if(mod == 0)
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
  else if(mod == 1)
  {
    if(btn == SDL_BUTTON_LEFT)
    {
      int x = 0, y = 0;
      SDL_GetMouseState(&x,&y);

      selectionReturn ret1 = energy_clicked({static_cast<float>(x), static_cast<float>(y)});
      selectionReturn ret2 = upgrades_clicked({static_cast<float>(x), static_cast<float>(y)});

      if(ret1.sel)
      {
        ply->setEnergyPriority(ret1.val);
      }
      else if(ret2.sel)
      {
        ply->upgrade(ret2.val);
        playerUpgrade( ply->getUpgrade(ret2.val) );
        if(ret2.val == 5) uni->addMiner();
        else if(ret2.val == 6) uni->addWingman();
        else if(ret2.val == 7) uni->addBuild(ply->getPos(), PLAYER_TURRET);
        else if(ret2.val == 8) uni->addBuild(ply->getPos(), PLAYER_GRAVWELL);
        else if(ret2.val == 9) uni->addBuild(ply->getPos(), PLAYER_BARRACKS);
        else if(ret2.val == 10) uni->addBuild(PLAYER_STATION);
      }
    }
    return;
  }
}

SDL_Window * gameInit()
{
	#ifdef _WIN32
	AllocConsole() ;
	AttachConsole( GetCurrentProcessId() );
	freopen( "CON", "w", stdout );
	freopen( "CON", "w", stdin );
	freopen( "CON", "w", stderr );
	#endif
	
	loadConfig();
	
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
    std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}
	
	if(TTF_Init() != 0)
	{
    std::cerr << "TTF_Init() failed: " << TTF_GetError() << std::endl;
		SDL_Quit();
		return nullptr;
	}
	
	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
	{
    std::cerr << "Mix_OpenAudio() failed! " << SDL_GetError() << std::endl;
		//SDL_Quit();
		//return 1;
	}
	
	loadSounds();
	
	srand (static_cast <unsigned> (time(0)));
	
	SDL_Window *window = SDL_CreateWindow("Elite: Dangerous v2.0",
    WIN_POS_X, WIN_POS_Y,
    WIN_HEIGHT, WIN_WIDTH,
    SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED );

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

	return window;
}

void handleUserMouseUpInput(int btn, int mod, player *ply, universe *uni)
{
  if(mod == 0)
  {
    if(btn == SDL_BUTTON_LEFT)
    {
      ply->setFiring(false);

    }
  }
  else if(mod == 1)
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

void handleUserKeyDownInput(int sym, player *ply, universe *uni, int * mod)
{
  //int ang = ply->getAng();

  switch (sym)
  {
    case SDLK_w:
      ply->accelerate(1);
      uni->setVel(-ply->getVel());
      break;
    case SDLK_s:
      if(*mod == 1) saveGame(uni);
      else
      {
        ply->accelerate(-1);
        uni->setVel(-ply->getVel());
        uni->addParticleSprite( ply->getPos(), ply->getVel(), 0.0f, XPLO_TEXT);
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
      *mod = 1;
      break;
    case SDLK_g:
      if(DEV_MODE) *uni->getScorePt() += 100;
      break;
    case SDLK_n:
      if(*mod == 1)
      {
        uni->reload(true);
        for(int i = 0; i < UPGRADES_LEN; ++i) setUpgradeTextures(0, i);
      }
      break;
    case SDLK_l:
      if(*mod == 1)
      {
        uni->reload(true);
        loadGame(uni);
        for(int i = 0; i < UPGRADES_LEN; ++i) setUpgradeTextures(uni->getPly()->getUpgrade(i), i);
      }
      break;
    case SDLK_SPACE:
      uni->pause();
      break;
  }
}

void handleUserKeyUpInput(int sym, int * mod)
{
  switch (sym)
  {
    case SDLK_LCTRL:
      *mod = 0;
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
