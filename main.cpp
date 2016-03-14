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
void handleUserMouseDownInput(int, int *, player*, universe*);
void handleUserMouseUpInput(int,int,player*,universe*);
void handleUserScroll(int,player*);
double diffClock(clock_t clock1, clock_t clock2);

void mainMenu(universe &uni);
void playGame(universe &uni);
void playTutorial(universe &uni);

int main(int argc, char* argv[])
{
    std::cout << "ELITE DANGEROUS v2.0 INITIALISING..." << std::endl;
    loadConfig();
    universe uni;
#ifdef _WIN32
    AllocConsole() ;
    AttachConsole( GetCurrentProcessId() );
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stdin );
    freopen( "CON", "w", stderr );
#endif

    sfxInit();
    loadSounds();
    playMusic(0);
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
    }
    deleteSounds();
    return 0;
}

void mainMenu(universe &uni)
{
    playSnd(MENU_SELECT_SND);
    //universe uni;
    //Create the universe.
    g_GAME_OVER = true;

    uni.reload();
    uni.getPly()->setHealth(-1);
    uni.update(0.1f);
    uni.getUI()->clear();

    std::array<int, 8> btncol = {20, 220, 255, 220, 20, 200, 255, 220};
    std::array<int, 8> quitcol = {255, 25, 40, 220, 255, 5, 30, 220};
    std::array<int, 8> blank = {0, 0, 0, 0, 0, 0, 0, 0};

    ui::selection mainMenuSelection;
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
    ui::button optionsHeaderBtn("OPTIONS", blank, blank, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f}, {200.0f, 20.0f});
    ui::button resolutionHeaderBtn("RESOLUTION", blank, blank, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 100.0f}, {200.0f, 20.0f});
    ui::button graphicsHeaderBtn("QUALITY", blank, blank, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y}, {200.0f, 20.0f});
    ui::button difficultyHeaderBtn("DIFFICULTY", blank, blank, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 100.0f}, {200.0f, 20.0f});
    ui::button backToMainMenu("RETURN", quitcol, quitcol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 200.0f}, {200.0f, 80.0f});
    optionsHeader.add(optionsHeaderBtn);
    optionsHeader.add(resolutionHeaderBtn);
    optionsHeader.add(graphicsHeaderBtn);
    optionsHeader.add(difficultyHeaderBtn);
    optionsHeader.add(backToMainMenu);
    optionsHeader.setVisible(false);
    uni.getUI()->add(optionsHeader);

    float w = 90.0f, h = 50.0f;
    ui::selection resolutionOptions;
    resolutionOptions.add(ui::button("1280 x 720", btncol, btncol, {g_HALFWIN.m_x - 400.0f, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.add(ui::button("1280 x 1024", btncol, btncol, {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.add(ui::button("1366 x 768", btncol, btncol, {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.add(ui::button("1440 x 900", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.add(ui::button("1600 x 900", btncol, btncol, {g_HALFWIN.m_x, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.add(ui::button("1920 x 1080", btncol, btncol, {g_HALFWIN.m_x + 100.0f, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.add(ui::button("1920 x 1200", btncol, btncol, {g_HALFWIN.m_x + 200.0f, g_HALFWIN.m_y - 80.0f}, {w, h}));
    resolutionOptions.setVisible(false);
    uni.getUI()->add(resolutionOptions);

    ui::selection qualityOptions;
    qualityOptions.add(ui::button("Potato", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 30.0f}, {w, h}));
    qualityOptions.add(ui::button("Medium Rare", btncol, btncol, {g_HALFWIN.m_x, g_HALFWIN.m_y + 30.0f}, {w, h}));
    qualityOptions.add(ui::button("Da Vinci", btncol, btncol, {g_HALFWIN.m_x + 100.0f, g_HALFWIN.m_y + 30.0f}, {w, h}));
    qualityOptions.setVisible(false);
    uni.getUI()->add(qualityOptions);

    w = 140.0f;
    h = 50.0f;
    ui::selection difficultyOptions;
    difficultyOptions.add(ui::button("Cadet", btncol, btncol, {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y + 130.0f}, {w, h}));
    difficultyOptions.add(ui::button("Cosmonaut", btncol, btncol, {g_HALFWIN.m_x - 150.0f, g_HALFWIN.m_y + 130.0f}, {w, h}));
    difficultyOptions.add(ui::button("Captain", btncol, btncol, {g_HALFWIN.m_x, g_HALFWIN.m_y + 130.0f}, {w, h}));
    difficultyOptions.add(ui::button("Commander", btncol, btncol, {g_HALFWIN.m_x + 150.0f, g_HALFWIN.m_y + 130.0f}, {w, h}));
    difficultyOptions.add(ui::button("Computing for Animation 1", btncol, btncol, {g_HALFWIN.m_x + 300.0f, g_HALFWIN.m_y + 130.0f}, {w, h}));
    difficultyOptions.setVisible(false);
    uni.getUI()->add(difficultyOptions);

    g_TARG_ZOOM_LEVEL = 0.4f;
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
                    std::cout << "CLICK : " << mainMenuSelected.m_sel_val << ", " << mainMenuSelected.m_button_val << std::endl;
                    if(mainMenuSelected.m_sel_val == 0)
                    {
                        playSnd(MENU_SELECT_SND);
                        switch(mainMenuSelected.m_button_val)
                        {
                        case 0:
                            g_GAME_STATE = MODE_GAME;
                            return;
                        case 1:
                            g_GAME_STATE = MODE_TUTORIAL;
                            return;
                        case 2:
                            //std::cout << "OPTIONS MENU YO" << std::endl;
                            for(size_t i = 0; i < uni.getUI()->getElements()->size(); ++i)
                            {
                                (*uni.getUI()->getElements())[i].toggleVisible();
                            }
                            break;
                        case 3:
                            g_GAME_STATE = MODE_QUIT;
                            return;
                        default:
                            break;
                        }
                    }
                    else if(mainMenuSelected.m_sel_val == 1)
                    {
                        playSnd(MENU_SELECT_SND);
                        if(mainMenuSelected.m_button_val == 4)
                        {
                            for(size_t i = 0; i < uni.getUI()->getElements()->size(); ++i)
                            {
                                (*uni.getUI()->getElements())[i].toggleVisible();
                            }
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

void playGame(universe &uni)
{
    playSnd(MENU_SELECT_SND);
    //universe uni;
    std::cout << "LAUNCHING MAIN GAME..." << std::endl;

    g_GAME_OVER = false;
    uni.reload();
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
                handleUserMouseDownInput(incomingEvent.button.button, &keymod, uni.getPly(), &uni);
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
            int x = 0, y = 0;
            SDL_GetMouseState(&x,&y);
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
            int x = 0, y = 0;
            SDL_GetMouseState(&x,&y);
            ui::selectionReturn ret = uni->getUI()->handleInput({static_cast<float>(x), static_cast<float>(y)});
            if(ret.m_sel_val == 0)
            {
                playSnd(MENU_SELECT_SND);
                ply->setEnergyPriority(ret.m_button_val);
            }
            else if(ret.m_sel_val == 1)
            {
                playSnd(MENU_SELECT_SND);
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
                playSnd(MENU_SELECT_SND);
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
                    uni->reload();
                    uni->setPause(false);
                    uni->escMenuTog();
                    uni->getUI()->pop();
                    uni->initUI();
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
            }
        }
        return;
    }
}

void gameInit()
{
    loadConfig();

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
    ui::button resume ("RESUME", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 300.0f}, {200.0f, 80.0f});
    ui::button sGame ("SAVE GAME", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f}, {200.0f, 80.0f});
    ui::button lGame ("LOAD GAME", btncol, btncol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 100.0f}, {200.0f, 80.0f});
    ui::button mm ("MAIN MENU", quitcol, quitcol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y}, {200.0f, 80.0f});
    ui::button quit ("QUIT", quitcol, quitcol, {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 100.0f}, {200.0f, 80.0f});

    escMenuSelection.add(resume);
    escMenuSelection.add(sGame);
    escMenuSelection.add(lGame);
    escMenuSelection.add(mm);
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
            uni->reload();
            //for(int i = 0; i < UPGRADES_LEN; ++i) setUpgradeTextures(0, i);
        }
        break;
    case SDLK_l:
        if(*keymod == 1)
        {
            uni->reload();
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

void playTutorial(universe &uni)
{
    enum stage {
        STAGE_BEGIN, STAGE_BEGIN_UNPAUSED,
        STAGE_MOVEMENT_INTRO,
        STAGE_SHOOTING_INTRO, STAGE_SHOOTING,
        STAGE_ENERGY_LEVELS_PAUSE, STAGE_ENERGY_LEVELS,
        STAGE_COMPLETE
    };
    stage tutStage = STAGE_BEGIN;
    playSnd(MENU_SELECT_SND);
    //universe uni;
    std::cout << "LAUNCHING TUTORIAL..." << std::endl;

    g_DIFFICULTY = 0;

    g_GAME_OVER = false;
    uni.reload();
    g_ZOOM_LEVEL = 0.0f;
    g_TARG_ZOOM_LEVEL = 0.8f;
    //Timer used to keep track of game time.
    //The argument is the fps of the updates, higher = more detailed.
    sim_time clock(120.0f);

    std::array<bool, 4> directionsTravelled = {false, false, false, false};
    float timer = 0.0f;
    //Keypress modifiers (shift, ctrl etc).
    int keymod = 0;
    while(g_GAME_STATE == MODE_TUTORIAL)
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
                switch(incomingEvent.button.button)
                {
                case SDL_BUTTON_LEFT:
                    if(tutStage == STAGE_SHOOTING)
                    {
                        timer = 0.0f;
                        tutStage = STAGE_ENERGY_LEVELS;
                    }
                default:
                    break;
                }
                handleUserMouseDownInput(incomingEvent.button.button, &keymod, uni.getPly(), &uni);
                break;
            case SDL_MOUSEBUTTONUP:
                handleUserMouseUpInput(incomingEvent.button.button, keymod, uni.getPly(), &uni);
                break;
            case SDL_MOUSEWHEEL:
                handleUserScroll(incomingEvent.wheel.y, uni.getPly());
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
                handleUserKeyDownInput(incomingEvent.key.keysym.sym, uni.getPly(), &uni, &keymod);
                break;
            case SDL_KEYUP:
                handleUserKeyUpInput(incomingEvent.key.keysym.sym, &keymod);
                break;
            }
        }
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

        //Draw the game.
        float diff_clamped = clock.getDiff();
        if(diff_clamped == 0.0f) diff_clamped = 0.01f;
        uni.draw( clock.getAcc() / diff_clamped * g_TIME_SCALE );

        if(tutStage == STAGE_BEGIN and timer > 1.0f)
        {
            uni.setPause(true);
            tutStage = STAGE_BEGIN_UNPAUSED;
        }
        else if(tutStage == STAGE_BEGIN_UNPAUSED)
        {
            uni.getRenderer()->drawText("WELCOME TO ELITE DANGEROUS V2.0", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false);
            uni.getRenderer()->drawText("THIS TUTORIAL WILL GUIDE YOU THROUGH SOME OF THE GAMES SYSTEMS", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false);
            uni.getRenderer()->drawText("PRESS 'SPACE' TO UNPAUSE", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false);
            uni.getRenderer()->finalise();
        }
        else if(tutStage == STAGE_MOVEMENT_INTRO)
        {
            vec2 ppos = uni.getPly()->getPos();
            float ang = uni.getPly()->getAng();
            uni.getRenderer()->drawText("OKAY, FIRST UP, MOVEMENT:", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false);
            uni.getRenderer()->drawText("YOU ALWAYS CHASE THE MOUSE", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false);
            uni.getRenderer()->drawText("TRY MOVING IN ALL FOUR DIRECTIONS", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false);
            if(!directionsTravelled[0]) uni.getRenderer()->drawText("W", "pix", ppos + vec(ang + 90) * 100.0f, true);
            if(!directionsTravelled[1]) uni.getRenderer()->drawText("A", "pix", ppos + vec(ang) * 100.0f, true);
            if(!directionsTravelled[2]) uni.getRenderer()->drawText("S", "pix", ppos + vec(ang - 90) * 100.0f, true);
            if(!directionsTravelled[3]) uni.getRenderer()->drawText("D", "pix", ppos + vec(ang + 180) * 100.0f, true);
            uni.getRenderer()->finalise();
            if(std::all_of(
                        std::begin(directionsTravelled),
                        std::end(directionsTravelled),
                        [](bool i)
                        {
                           return i;
                        }) and timer > 2.0f)
            {
                tutStage = STAGE_SHOOTING_INTRO;
            }
        }
        else if(tutStage == STAGE_SHOOTING_INTRO)
        {
            uni.setPause(true);
            tutStage = STAGE_SHOOTING;
        }
        else if(tutStage == STAGE_SHOOTING)
        {
            uni.getRenderer()->drawText("SO YOU CAN MOVE, BUT CAN YOU SHOOT?", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false);
            uni.getRenderer()->drawText("PRESSING LMB WILL SHOOT A LASER IN THE DIRECTION YOU ARE FACING", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false);
            uni.getRenderer()->drawText("TRY IT OUT!", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false);
            uni.getRenderer()->finalise();
        }
        else if(tutStage == STAGE_ENERGY_LEVELS_PAUSE)
        {
            uni.setPause(true);
            tutStage = STAGE_ENERGY_LEVELS;
        }
        else if(tutStage == STAGE_ENERGY_LEVELS and timer > 2.0f)
        {
            uni.getRenderer()->drawText("PRETTY COOL, RIGHT?", "pix", {g_HALFWIN.m_x - 300.0f, g_HALFWIN.m_y - 200.0f}, false);
            uni.getRenderer()->drawText("YOU CAN'T JUST SHOOT FOREVER THOUGH, YOU HAVE TO THINK ABOUT YOUR HEALTH, SHIELDS AND ENERGY", "pix", {g_HALFWIN.m_x - 200.0f, g_HALFWIN.m_y - 160.0f}, false);
            uni.getRenderer()->drawText("YOU CAN SEE ALL THIS IN THE TOP-LEFT", "pix", {g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 120.0f}, false);
            uni.getRenderer()->finalise();
        }
    }
}

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11

g++ -Wall -o "%e"  -L usr\bin\lib "%f" -I usr\bin\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11
*/

/*
g++ -Wall -o "%e" -lmingw32  -mwindows -L C:\SDL2\x86_64-w64-mingw32\lib "%f" -I C:\SDL2\x86_64-w64-mingw32\include  C:\Users\Ben\Documents\Programming\projects\CA1\ed2\src C:\Users\Ben\Documents\Programming\projects\CA1\ed2\include -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lSDL2main -lSDL2 -std=c++11 
*/
