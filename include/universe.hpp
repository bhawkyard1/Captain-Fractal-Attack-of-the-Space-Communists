#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <array>

#include "common.hpp"
#include "enemy.hpp"
#include "faction.hpp"
#include "laser.hpp"
#include "missile.hpp"
#include "pfx.hpp"
#include "player.hpp"

#if RENDER_MODE == 0
#include "renderer.hpp"
#elif RENDER_MODE == 1
#include "renderer_opengl.hpp"
#endif

#include "renderer_opengl.hpp"

#include "ship.hpp"
#include "squad.hpp"
#include "stardust.hpp"
#include "stardust_sprite.hpp"
#include "ui/user_interface.hpp"
#include "weapons.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file universe.hpp
/// \brief This file is huge, contains the biggest class in the game, and a utility struct.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
/// \class col_partition
/// \brief A simple struct, designed to hold a group of entities in close proximity to one another,
/// who can interact in some way.
//----------------------------------------------------------------------------------------------------------------------

struct col_partition
{
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of ship references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector< std::vector<enemy*> > ships;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of laser references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector< std::vector<laser*> > lasers;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of missile references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector< std::vector<missile*> > rockets;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of asteroid references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector< std::vector<ship*> > rocks;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Used only for some debug views with the SDL Renderer.
    //----------------------------------------------------------------------------------------------------------------------
#if RENDER_MODE == 0
    std::vector<SDL_Rect> rects;
#endif
};

//----------------------------------------------------------------------------------------------------------------------
/// \class universe
/// \brief This class is huge, and touches almost every part of the game.
//----------------------------------------------------------------------------------------------------------------------

class universe
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief ctor for the universe class, sets up the game and all rendering functionality.
    //----------------------------------------------------------------------------------------------------------------------
    universe();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Initialises the UI to default values.
    //----------------------------------------------------------------------------------------------------------------------
    void initUI();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter for the player.
    //----------------------------------------------------------------------------------------------------------------------
    player * getPly() {return &m_ply;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Velocity setter.
    //----------------------------------------------------------------------------------------------------------------------
    void setVel(const vec3 _v) {m_vel = _v;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a laser to the universe.
    /// \param _p position
    /// \param _v velocity
    /// \param _angle initial angle
    /// \param _weap array of data such as damage
    /// \param _team source of the shot
    //----------------------------------------------------------------------------------------------------------------------
    void addShot(const vec3 _p, const vec3 _v, const float _angle, const std::array<float, WEAPS_W> _weap, const aiTeam _team);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a missile to the universe.
    /// \param _p position
    /// \param _v velocity
    /// \param _angle initial angle
    /// \param _team source of the missile
    //----------------------------------------------------------------------------------------------------------------------
    void addMissile(const vec3 _p, const vec3 _v, const float _angle, const aiTeam _team);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a ship to the universe, with a random faction and type.
    /// \param _t team of the ship
    //----------------------------------------------------------------------------------------------------------------------
    void spawnShip(const aiTeam _t);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a ship to the universe, with a specified position and type.
    /// \param _type ship class
    /// \param _t team of the ship
    /// \param _p position
    //----------------------------------------------------------------------------------------------------------------------
    void spawnShip(const ship_spec _type, const aiTeam _t, const vec3 _p);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns a (weighted) random ship type from a given faction.
    /// \param _t ship faction.
    //----------------------------------------------------------------------------------------------------------------------
    ship_spec getRandomShipType(const aiTeam _t);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Spawns a series of ships.
    /// \param _t team of the ships
    /// \param _min minumum distance from origin
    /// \param _max maximum distance from origin
    /// \param _i number of ships to spawn
    //----------------------------------------------------------------------------------------------------------------------
    void spawnSquad(const aiTeam _t, const float _min, const float _max, const int _i);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief This function only actually incremements the max number of wingmen available, which
    /// are randomly spawned into the scene in the update function.
    //----------------------------------------------------------------------------------------------------------------------
    void addWingman() {m_factionMaxCounts[TEAM_PLAYER]++;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief This function only actually incremements the max number of miners available, which
    /// are randomly spawned into the scene in the update function.
    //----------------------------------------------------------------------------------------------------------------------
    void addMiner() {m_factionMaxCounts[TEAM_PLAYER_MINER]++;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Used to place static structures into the world, although under the hood it just spawns a ship with a given classification.
    /// \param _p position
    /// \param _type ship classification
    //----------------------------------------------------------------------------------------------------------------------
    void addBuild(const vec3 _p, const ship_spec _type);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Used to place static structures in random places. Could be useful for spawning enemy space stations in the future.
    /// \param _type ship classification.
    //----------------------------------------------------------------------------------------------------------------------
    void addBuild(const ship_spec _type);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the universe by a given timestep. Physics/AI and rendering is decoupled, so this
    /// may be called multiple times in between each draw-call.
    /// \param _dt time difference.
    //----------------------------------------------------------------------------------------------------------------------
    void update(const float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Clears the renderer.
    //----------------------------------------------------------------------------------------------------------------------
    void clear() {m_drawer.clear();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws the universe to the screen.
    /// \param _dt time difference, we can linearly interpolate objects being drawn to ensure that their
    /// movement is continuous.
    //----------------------------------------------------------------------------------------------------------------------
    void draw(const float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Draws the user interface to the screen. Does not handle user input.
    //----------------------------------------------------------------------------------------------------------------------
    void drawUI();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the renderer.
    //----------------------------------------------------------------------------------------------------------------------
    void swap() {m_drawer.finalise();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A broad phase, this is used to detect entities which are close to one another.
    /// \param _box Bounding box enclosing the entities to be sorted,
    /// \param _ships references to ships to be sorted
    /// \param _lasers references to lasers to be sorted
    /// \param _rockets references  to missiles to be sorted
    /// \param _rocks references to asteroids to be sorted
    /// \param _lvl current level of recursion
    //----------------------------------------------------------------------------------------------------------------------
    void detectCollisions(const SDL_Rect _box, std::vector<enemy*> _ships, std::vector<laser*> _lasers, std::vector<missile*> _rockets, std::vector<ship*> _rocks, unsigned short int _lvl);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A narrow phase, this checks through the partitioned entities in detail.
    //----------------------------------------------------------------------------------------------------------------------
    void checkCollisions();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a particle system to the universe.
    /// \param _type ship classification.
    //----------------------------------------------------------------------------------------------------------------------
    void addpfx(const vec3 _p, const vec3 _v, const vec3 _wv, const int _no, const float _f);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a particle sprite to the universe. This is a bit of a misnomer, since the opengl renderer uses billboards + glsl.
    /// \param _p position
    /// \param _v velocity
    /// \param _dim size
    /// \param _tex surface identifier ie "SMOKE".
    //----------------------------------------------------------------------------------------------------------------------
    void addParticleSprite(const vec3 _p, const vec3 _v, const float _dim, const std::string _tex);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets a pointer to the agents vector. Mostly a quick and dirty way of doing stuff in the tutorial, and for file operations.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<enemy>* getAgents() {return &m_agents;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets a pointer to the laser vector. Mostly used for saving and loading the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<laser>* getShots() {return &m_shots;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets a pointer to the missile vector. Mostly used for saving and loading the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<missile>* getMissiles() {return &m_missiles;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets a pointer to the asteroid vector. Mostly used for saving and loading the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ship>* getAsteroids() {return &m_asteroids;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets the closest enemy to a given point in space, mostly used for missile tracking.
    /// \param _p position
    /// \param _t team to check against
    //----------------------------------------------------------------------------------------------------------------------
    ship * closestEnemy(const vec3 _p, const aiTeam _t);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for the score.
    //----------------------------------------------------------------------------------------------------------------------
    void setScore(const int _s) {m_score = _s; m_ui.update(m_score); m_ui.update(m_score);}
    void addScore(const int _s) {m_score += _s; m_ui.update(m_score); m_ui.update(m_score);}
    int getScore() const  {return m_score;}
    int * getScorePt() {return &m_score;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the max enemy count.
    //----------------------------------------------------------------------------------------------------------------------
    int getMaxEnemyCount(aiTeam _t) const {return m_factionMaxCounts[_t];}
    void setMaxEnemyCount(const int m, const aiTeam _t) {m_factionMaxCounts[_t] = m;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter for the enemy count.
    //----------------------------------------------------------------------------------------------------------------------
    int getEnemyCount(aiTeam _t) const {return m_factionCounts[_t];}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter for the max wingman count.
    //----------------------------------------------------------------------------------------------------------------------
    int getMaxWingmanCount() const {return m_factionMaxCounts[TEAM_PLAYER];}
    void setMaxWingmanCount(const int m) {m_factionMaxCounts[TEAM_PLAYER] = m;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter for the max miner count.
    //----------------------------------------------------------------------------------------------------------------------
    int getMaxMinerCount() const {return m_factionMaxCounts[TEAM_PLAYER_MINER];}
    void setMaxMinerCount(const int m) {m_factionMaxCounts[TEAM_PLAYER_MINER] = m;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Check to see if a given faction has reached its max count.
    //----------------------------------------------------------------------------------------------------------------------
    bool atMaxCount(aiTeam _t) const {return m_factionCounts[_t] < m_factionMaxCounts[_t];}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Resets the universe, and clears containers. Used to start a new game.
    /// \param _newGame whether we are starting a new game.
    //----------------------------------------------------------------------------------------------------------------------
    void reload(const bool _newGame);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Fills g_ship_templates, which we can copy from later on.
    //----------------------------------------------------------------------------------------------------------------------
    void loadShips();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setters for game pause.
    //----------------------------------------------------------------------------------------------------------------------
    void pause() {m_paused = !m_paused; if(!m_paused) g_TIME_SCALE = 1;}
    void setPause(bool _p) {m_paused = _p;}
    bool isPaused() const {return m_paused;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the UI.
    //----------------------------------------------------------------------------------------------------------------------
    ui::userInterface * getUI() {return &m_ui;}
    ui::userInterface setUI(ui::userInterface _i) {m_ui = _i;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A function to tell whether we are allowed to upgrade (compares cost vs score etc).
    /// \param _sel selection menu triggered
    /// \param _btn button triggered
    //----------------------------------------------------------------------------------------------------------------------
    bool upgradeCallback(const int _sel, const int _btn);
    void upgradeSetLabels(const int _sel, const int _btn, const int _plvl);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the mouse state.
    //----------------------------------------------------------------------------------------------------------------------
    void setMouseState(const int _i) {m_mouse_state = _i;}
    int getMouseState() const {return m_mouse_state;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a given enemy to a given squad.
    /// \param _e enemy to add
    /// \param _s squad to add to
    //----------------------------------------------------------------------------------------------------------------------
    void addToSquad(enemy * _e, squad * _s) {_e->setSquadID(_s->m_id); _s->m_size++;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Removes a given enemy from a given squad. Requires you know which squad it is in.
    /// \param _e enemy to remove
    /// \param _s squad to remove from
    //----------------------------------------------------------------------------------------------------------------------
    void removeFromSquad(enemy * _e, squad * _s) {_e->setSquadID(-1); if(_s != nullptr) _s->m_size--;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Creates the factions and stores them in m_factions.
    //----------------------------------------------------------------------------------------------------------------------
    void createFactions();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Given a squad ID, returns the associated squad.
    //----------------------------------------------------------------------------------------------------------------------
    squad * getSquadFromID(int _id);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns the renderer. Mostly used to hack text into the tutorial.
    //----------------------------------------------------------------------------------------------------------------------
#if RENDER_MODE == 0
    renderer * getRenderer() {return &m_drawer;}
#elif RENDER_MODE == 1
    renderer_ngl * getRenderer() {return &m_drawer;}
#endif

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the boolean associated with the in game options menu display.
    //----------------------------------------------------------------------------------------------------------------------
    bool isEscMenuShown() {return m_escMenuShown;}
    void escMenuTog() {m_escMenuShown = !m_escMenuShown;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for UI visibility.
    //----------------------------------------------------------------------------------------------------------------------
    bool UIVisible() const {return showUI;}
    void setUIVisible(const bool _b) {showUI = _b;}
    void toggleUIVisible() {showUI = !showUI;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gives a ship a parent, and an offset from said parent.
    /// \param _parent parent ship
    /// \param _child child ship
    /// \param _offset childs offset from the parent
    //----------------------------------------------------------------------------------------------------------------------
    void shipAddParent(ship *_parent, ship *_child, vec3 _offset);
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief If this is true, the UI will be displayed.
    //----------------------------------------------------------------------------------------------------------------------
    bool showUI;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Contains all of the data for the user interface.
    //----------------------------------------------------------------------------------------------------------------------
    ui::userInterface m_ui;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Renderer specific members.
    //----------------------------------------------------------------------------------------------------------------------
#if RENDER_MODE == 0
    renderer m_drawer;
    std::vector<stardust> m_dots;
    std::vector<stardust_sprite> m_sparkles;
#elif RENDER_MODE == 1
    renderer_ngl m_drawer;
#endif

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all lasers in the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<laser> m_shots;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all AI controlled ships in the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<enemy> m_agents;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all particle systems in the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<pfx> m_particles;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all missiles in the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<missile> m_missiles;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all asteroids in the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ship> m_asteroids;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all particle sprites in the game (in the NGL version, these actually use
    /// procedural shaders, rather than bitmaps).
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<stardust_sprite> m_passiveSprites;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all squads.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<squad> m_squads;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all factions.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<faction> m_factions;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Stores the number of enemies spawned of each faction.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<int> m_factionCounts;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The maximum number of permitted spawned ships per faction. Rises as the player fights them.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<int> m_factionMaxCounts;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The player.
    //----------------------------------------------------------------------------------------------------------------------
    player m_ply;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The velocity of the universe (the negative of the player's velocity). In this way, the
    /// player's engines do not move the ship, they move the universe.
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_vel;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief For the opengl background shader, I must track the position of the universe, even though
    /// it has no real practical meaning.
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_pos;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Stores partitions of entities, separated in a broad phase, to reduce On^2 checks.
    //----------------------------------------------------------------------------------------------------------------------
    col_partition m_partitions;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The player's score.
    //----------------------------------------------------------------------------------------------------------------------
    int m_score;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The current background colour.
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 3> m_cCol;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The target background colour.
    //----------------------------------------------------------------------------------------------------------------------
    std::array<float, 3> m_tCol;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gameplay intensity fluctuates over time, to create quieter and more hectic periods. This
    /// affects ship spawning frequency.
    //----------------------------------------------------------------------------------------------------------------------
    float m_gameplay_intensity = 1;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Whether the game is paused.
    //----------------------------------------------------------------------------------------------------------------------
    bool m_paused;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Used for visualising placement of structures in the world, this int tracks what the player
    /// is about to build.
    //----------------------------------------------------------------------------------------------------------------------
    int m_mouse_state;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Whether the in-game options menu is shown.
    //----------------------------------------------------------------------------------------------------------------------
    bool m_escMenuShown;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Stores time since the game was started.
    //----------------------------------------------------------------------------------------------------------------------
    double m_time_elapsed;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Controls enemy spawn rate.
    //----------------------------------------------------------------------------------------------------------------------
    int m_enemySpawnRate;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Resolves a collision between two ships.
    //----------------------------------------------------------------------------------------------------------------------
    void resolveCollision(ship * _a, ship * _b);
};

#endif
