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
#undef interface
#endif


//Function prototypes.
void gameInit();
void handleUserKeyDownInput(int, player*, universe*, int*);
void handleUserKeyUpInput(int,int*);
void handleUserMouseDownInput(int,int,player*,universe*);
void handleUserMouseUpInput(int,int,player*,universe*);
void handleUserScroll(int,player*);
double diffClock(clock_t clock1, clock_t clock2);

void mainMenu();
void playGame();
void playTutorial();

int main(int argc, char* argv[])
{
    std::cout << "ELITE DANGEROUS v2.0 INITIALISING..." << std::endl;

#ifdef _WIN32
    AllocConsole() ;
    AttachConsole( GetCurrentProcessId() );
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stdin );
    freopen( "CON", "w", stderr );
#endif

    while(g_GAME_STATE != MODE_QUIT)
    {
        if(g_GAME_STATE == MODE_MENU)
        {
            gameInit();
            mainMenu();
            deleteSounds();
        }
        else if(g_GAME_STATE == MODE_TUTORIAL)
        {
            gameInit();
            playTutorial();
            deleteSounds();
        }
        else if(g_GAME_STATE == MODE_GAME)
        {
            gameInit();
            playGame();
            deleteSounds();
        }
    }
    return 0;
}

void mainMenu()
{
    //Create the universe.
    universe uni;
    g_GAME_OVER = true;
    uni.getPly()->setHealth(-1);
    uni.update(0.1f);
    uni.getUI()->clear();

    ui::selection mainMenuSelection;
    std::array<int, 8> btncol = {20, 200, 255, 220, 20, 200, 255, 220};
    std::array<int, 8> quitcol = {255, 5, 30, 220, 255, 5, 30, 220};
    ui::button play ("PLAY GAME", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f}, {200.0f, 80.0f});
    ui::button tut ("PLAY TUTORIAL", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 50.0f}, {200.0f, 80.0f});
    ui::button opt ("OPTIONS", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 100.0f}, {200.0f, 80.0f});
    ui::button quit ("QUIT", quitcol, quitcol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 250.0f}, {200.0f, 80.0f});
    mainMenuSelection.add(play);
    mainMenuSelection.add(tut);
    mainMenuSelection.add(opt);
    mainMenuSelection.add(quit);
    uni.getUI()->add(mainMenuSelection);

    ui::selection optionsHeader;
    std::array<int, 8> blank = {0, 0, 0, 0, 0, 0, 0, 0};
    ui::button optionsHeaderBtn("OPTIONS", blank, blank, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f}, {200.0f, 80.0f});

    g_TARG_ZOOM_LEVEL = 0.3f;
    g_DIFFICULTY = 20;

    vec2 scrollVel = randVec(0.5f, 2.0f);
    uni.setVel(scrollVel);
    uni.setMaxEnemyCount(20);
    uni.setMaxWingmanCount(10);

    uni.addBuild({600.0f, 300.0f}, PLAYER_STATION);

    for(int i = 0; i < 360; i += 6)
    {
        vec2 pos = {static_cast<float>(cos(rad(i))), static_cast<float>(sin(rad(i)))};
        pos *= 1100.0f;
        pos += {600.0f, 300.0f};
        uni.addBuild(pos, PLAYER_TURRET);
    }
    for(int i = 3; i < 360; i += 6)
    {
        vec2 pos = {static_cast<float>(cos(rad(i))), static_cast<float>(sin(rad(i)))};
        pos *= 1150.0f;
        pos += {600.0f, 300.0f};
        uni.addBuild(pos, PLAYER_TURRET);
    }

    for(auto i = uni.getAgents()->begin(); i != uni.getAgents()->end(); ++i) i->setVel(-scrollVel);

    for(int i = 0; i < 10; ++i)
    {
        aiTeam t = static_cast<aiTeam>(randNum(0,4));
        if(t = TEAM_PLAYER_MINER) t = static_cast<aiTeam>(static_cast<int>(t) + randNum(1,3));
        int r = randNum(5, 10);
        uni.spawnSquad(t, 6000.0f, 12000.0f, r);
    }

    //Timer used to keep track of game time.
    //The argument is the fps of the updates, higher = more detailed.
    sim_time clock(120.0f);

    //Keypress modifiers (shift, ctrl etc).
    int keymod = 0;
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
                    int mx = 0, my = 0;
                    SDL_GetMouseState(&mx, &my);
                    ui::selectionReturn mainMenuSelected = uni.getUI()->handleInput({static_cast<float>(mx), static_cast<float>(my)});
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
                            g_GAME_STATE = MODE_QUIT;
                            return;
                        case 3:
                            g_GAME_STATE = MODE_QUIT;
                            return;
                        default:
                            break;
                        }
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
        uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );
    }
}

void playGame()
{
    std::cout << "LAUNCHING MAIN GAME..." << std::endl;
    //Create the universe.
    universe uni;
    g_ZOOM_LEVEL = 0.0f;
    g_TARG_ZOOM_LEVEL = 0.8f;
    //Timer used to keep track of game time.
    //The argument is the fps of the updates, higher = more detailed.
    sim_time clock(120.0f);

    //Keypress modifiers (shift, ctrl etc).
    int keymod = 0;
    while(g_GAME_STATE == MODE_GAME)
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
            ui::selectionReturn ret = uni->getUI()->handleInput({static_cast<float>(x), static_cast<float>(y)});
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
            else if(ret.m_sel_val == 2)
            {
                switch(ret.m_button_val)
                {
                case 0:
                    g_GAME_STATE = MODE_MENU;
                    break;
                case 1:
                    break;
                case 2:
                    g_GAME_STATE = MODE_QUIT;
                    break;
                default:
                    break;

                }
            }
        }
        return;
    }
}

void gameInit()
{
    loadConfig();

    sfxInit();
    loadSounds();

    srand (static_cast <unsigned> (time(0)));

    g_GAME_OVER = false;
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

    ui::selectionReturn ret = uni->getUI()->handleInput({static_cast<float>(mx), static_cast<float>(my)});
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
    //The menu to appear when the user presses escape.
    ui::selection escMenuSelection;
    std::array<int, 8> btncol = {20, 200, 255, 220, 20, 200, 255, 220};
    std::array<int, 8> quitcol = {255, 5, 30, 220, 255, 5, 30, 220};
    ui::button resume ("MAIN MENU", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f}, {200.0f, 80.0f});
    ui::button opt ("OPTIONS", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 50.0f}, {200.0f, 80.0f});
    ui::button quit ("QUIT", quitcol, quitcol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 100.0f}, {200.0f, 80.0f});
    escMenuSelection.add(resume);
    escMenuSelection.add(opt);
    escMenuSelection.add(quit);

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

void playTutorial()
{

}

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11

g++ -Wall -o "%e"  -L usr\bin\lib "%f" -I usr\bin\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11
*/

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include  C:\Users\Ben\Documents\Programming\projects\CA1\ed2\src C:\Users\Ben\Documents\Programming\projects\CA1\ed2\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11 
*/
