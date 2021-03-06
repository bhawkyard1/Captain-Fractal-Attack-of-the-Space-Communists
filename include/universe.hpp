#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include <array>

#include "common.hpp"
#include "debris.hpp"
#include "enemy.hpp"
#include "faction.hpp"
#include "input/inputMap.hpp"
#include "laser.hpp"
#include "missile.hpp"
#include "pfx.hpp"
#include "player.hpp"
#include "popup.hpp"

#include "renderer_opengl.hpp"

#include "sfx.hpp"
#include "ship.hpp"
#include "slotmap.hpp"
#include "squad.hpp"
#include "sprite.hpp"
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
    std::vector<enemy*> m_ships;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of laser references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<laser*> m_lasers;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of missile references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<missile*> m_rockets;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of asteroid references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<ship*> m_rocks;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of resource references.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<debris*> m_resources;
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
		void addShot(const vec3 _p, const vec3 _v, const float _angle, const std::array<float, WEAPS_W> _weap, const aiTeam _team, aiTarget _owner, const float _xpModifier);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a raw resource to the universe.
    /// \param _pos position
    /// \param _vel velocity
    /// \param _type type of resource
    //----------------------------------------------------------------------------------------------------------------------
    void addDebris(const vec3 _pos, const vec3 _vel, resourceType _type);

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
    void spawnSquad(const aiTeam _t, const vec3 _p, const float _d, const int _num);

    void spawnBase(const aiTeam _t, const vec3 _center, int _lvl, float _ang);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Spawns a series of ships.
    /// \param _t team of the ships
    /// \param _min minumum distance from origin
    /// \param _max maximum distance from origin
    /// \param _ships ships to spawn
    //----------------------------------------------------------------------------------------------------------------------
    void spawnSquad(const aiTeam _t, const float _min, const float _max, const std::vector<size_t> _ships);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief This function only actually incremements the max number of wingmen available, which
    /// are randomly spawned into the scene in the update function.
    //----------------------------------------------------------------------------------------------------------------------
    void addWingman() {m_maxWingmen++; m_sounds.playUISnd(CLUNK_SND);}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief This function only actually incremements the max number of miners available, which
    /// are randomly spawned into the scene in the update function.
    //----------------------------------------------------------------------------------------------------------------------
    void addMiner() {m_maxMiners++; m_sounds.playUISnd(CLUNK_SND);}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Used to place static structures into the world, although under the hood it just spawns a ship with a given classification.
    /// \param _p position
    /// \param _type ship classification
    //----------------------------------------------------------------------------------------------------------------------
    void addBuild(const vec3 _p, const ship_spec _type, const aiTeam _team);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Used to place static structures in random places. Could be useful for spawning enemy space stations in the future.
    /// \param _type ship classification.
    //----------------------------------------------------------------------------------------------------------------------
    void addBuild(const ship_spec _type, const aiTeam _team);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the universe by a given timestep. Physics/AI and rendering is decoupled, so this
    /// may be called multiple times in between each draw-call.
    /// \param _dt time difference.
    //----------------------------------------------------------------------------------------------------------------------
    void update(float _dt);
    void updateUI();

    void processInputMap();

    void cullAgents();
    void calcSquadPositions();
    //void manageAgents(const float _dt);

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
    void drawUI(const float _dt);
		void drawDebugUI();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the renderer.
    //----------------------------------------------------------------------------------------------------------------------
    void swap() {m_drawer.finalise(m_time_elapsed, tovec2(m_vel));}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A broad phase, this is used to detect entities which are close to one another.
    /// \param _box Bounding box enclosing the entities to be sorted,
    /// \param _ships references to ships to be sorted
    /// \param _lasers references to lasers to be sorted
    /// \param _rockets references  to missiles to be sorted
    /// \param _rocks references to asteroids to be sorted
    /// \param _lvl current level of recursion
    //----------------------------------------------------------------------------------------------------------------------
    void detectCollisions(const SDL_Rect _box, std::vector<enemy*> _ships, std::vector<laser*> _lasers, std::vector<missile*> _rockets, std::vector<ship*> _rocks, std::vector<debris*> _resources, unsigned short int _lvl);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A narrow phase, this checks through the partitioned entities in detail.
    //----------------------------------------------------------------------------------------------------------------------
    void checkCollisions();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a particle system to the universe.
    /// \param _type ship classification.
    //----------------------------------------------------------------------------------------------------------------------
    void addpfx(const vec3 _p, const vec3 _v, const int _no, const float _f, const bool _drawExplosion, const std::array<float, 4> _col);

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
    slotmap<enemy> * getAgents() {return &m_agents;}

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
		slotmap<ship> * getAsteroids() {return &m_asteroids;}

    std::vector<faction>* getFactions() {return &m_factions;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets the closest enemy to a given point in space, mostly used for missile tracking.
    /// \param _p position
    /// \param _t team to check against
    //----------------------------------------------------------------------------------------------------------------------
    ship * closestEnemy(const vec3 _p, const aiTeam _t);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for the score.
    //----------------------------------------------------------------------------------------------------------------------
    void setScore(const float _s) {m_factions[TEAM_PLAYER].setWealth(_s); m_ui.update(m_factions[TEAM_PLAYER].getWealth(), getMousePos());}
    void addScore(const float _s) {m_factions[TEAM_PLAYER].addWealth(_s); m_ui.update(m_factions[TEAM_PLAYER].getWealth(), getMousePos());}
    float getScore() const  {return m_factions[TEAM_PLAYER].getWealth();}
    float * getScorePt() {return m_factions[TEAM_PLAYER].getWealthPt();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter for the max wingman count.
    //----------------------------------------------------------------------------------------------------------------------
    int getMaxWingmanCount() const {return m_maxWingmen;}
    void setMaxWingmanCount(const int _m) {m_maxMiners = _m;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter for the max miner count.
    //----------------------------------------------------------------------------------------------------------------------
    int getMaxMinerCount() const {return m_maxMiners;}
    void setMaxMinerCount(const int _m) {m_maxMiners = _m;}

    size_t getFactionCount(aiTeam _f) {return m_factions[_f].getNumActive();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Resets the universe, and clears containers. Used to start a new game.
    /// \param _newGame whether we are starting a new game.
    //----------------------------------------------------------------------------------------------------------------------
    void reload(const bool _newGame);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setters for game pause.
    //----------------------------------------------------------------------------------------------------------------------
    void pause() {m_paused = !m_paused; if(!m_paused) g_PLAYER_TIME_SCALE = 1.0f;}
    void setPause(bool _p) {m_paused = _p;}
    bool isPaused() const {return m_paused;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the UI.
    //----------------------------------------------------------------------------------------------------------------------
    userInterface * getUI() {return &m_ui;}
    void setUI(userInterface _i) {m_ui = _i;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the UI.
    //----------------------------------------------------------------------------------------------------------------------
    selectionReturn handleMouseDownInput(vec2 _mouse);
    void mouseUp();
    void processUIInput(selectionReturn _sel);

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
    /// \param _s id of squad to add to
    //----------------------------------------------------------------------------------------------------------------------
    void addToSquad(enemy * _e, slot _s);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Removes a given enemy from a given squad. Requires you know which squad it is in.
    /// \param _e enemy to remove
    /// \param _s squad to remove from
    //----------------------------------------------------------------------------------------------------------------------
    void removeFromSquad(enemy * _e);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Creates the factions and stores them in m_factions.
    //----------------------------------------------------------------------------------------------------------------------
    void createFactions();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Given a squad ID, returns the associated squad.
    //----------------------------------------------------------------------------------------------------------------------
    squad * getSquadFromID(aiTeam _t, slot _id);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns the renderer. Mostly used to hack text into the tutorial.
    //----------------------------------------------------------------------------------------------------------------------
    renderer_ngl * getRenderer() {return &m_drawer;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the boolean associated with the in game options menu display.
    //----------------------------------------------------------------------------------------------------------------------
    bool isEscMenuShown() {return m_escMenuShown;}
    void escMenuTog() {m_escMenuShown = !m_escMenuShown;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for UI visibility.
    //----------------------------------------------------------------------------------------------------------------------
		bool UIVisible() const {return m_showUI;}
		void setUIVisible(const bool _b) {m_showUI = _b;}
		void toggleUIVisible() {m_showUI = !m_showUI;}
		void toggleDebugUI() {m_showDebugUI = !m_showDebugUI;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gives a ship a parent, and an offset from said parent.
    /// \param _parent parent ship
    /// \param _child child ship
    /// \param _offset childs offset from the parent
    //----------------------------------------------------------------------------------------------------------------------
    void shipAddParent(size_t _index, ship *_child, vec3 _offset);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a text popup to the world.
    /// \param _dmg damage dealt
    /// \param _team recipient team
    /// \param _pos position
    /// \param _vel velocity
    //----------------------------------------------------------------------------------------------------------------------
    void addDamagePopup(int _dmg, aiTeam _team, vec3 _pos, vec3 _vel);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Adds a text popup to the world.
    /// \param _label text label
    /// \param _type controls colour
    /// \param _smul size multiplier
    /// \param _pos position
    /// \param _vel velocity
    //----------------------------------------------------------------------------------------------------------------------
    void addPopup(std::string _label, popup_type _type, float _smul, vec3 _pos, vec3 _vel);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Plays a sound. I don't like this function much, but I need it to play sounds externally.
    /// \param _sound to play
    //----------------------------------------------------------------------------------------------------------------------
    void playSnd(sound _sound, vec3 _pos, float _vol) {m_sounds.playSnd(_sound, _pos, _vol);}
    void playUISnd(sound _sound) {m_sounds.playUISnd(_sound);}
    void playMus(int _music) {m_sounds.playMusic(_music);}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Toggles the map mode between big and small.
    //----------------------------------------------------------------------------------------------------------------------
    void toggleMapMode() {m_mapExpanded = !m_mapExpanded;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Returns diplomatic status between two factions.
    /// \param _a Team A.
    /// \param _b Team B.
    //----------------------------------------------------------------------------------------------------------------------
    bool emnityCheck(const aiTeam _a, const aiTeam _b);
    bool friendshipCheck(const aiTeam _a, const aiTeam _b);
    bool neutralityCheck(const aiTeam _a, const aiTeam _b);
    bool sameTeam(const aiTeam _a, const aiTeam _b);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Gets the universe position.
    //----------------------------------------------------------------------------------------------------------------------
    vec3 getPos() const {return m_pos;}

    //ship * getByID(const unsigned long _i);
		void addFrag(ship * _i);

    vec3 getCameraPosition() {return m_drawer.getCamera()->getPos();}
    void setCameraPosition(const vec3 _pos) {m_drawer.getCamera()->setPos(_pos);}

    void conductTrade(enemy &_buyer, enemy &_seller);
		void dock(int _pidx, ship &_parent, ship &_child);
		void undock(ship &_s);

    void debug_lockPlayer() {g_PLAYER_MOVEMENT_LOCKED = !g_PLAYER_MOVEMENT_LOCKED;}

    void addShake(const float _force) {m_drawer.addShake(_force);}

    float getTime() {return m_time_elapsed;}

    void updateInputs(const std::vector<SDL_Event> &_events) {m_inputs.update(_events);}

		void movePlayer(const vec3 _position) {m_positioning = -_position; m_pos += m_positioning;}
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief If this is true, the UI will be displayed.
    //----------------------------------------------------------------------------------------------------------------------
		bool m_showUI;
		bool m_showDebugUI;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Contains all of the data for the user interface.
    //----------------------------------------------------------------------------------------------------------------------
    userInterface m_ui;
    selection m_escMenuSelection;

    inputMap m_inputs;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Renderer specific members.
    //----------------------------------------------------------------------------------------------------------------------
    renderer_ngl m_drawer;

    soundPlayer m_sounds;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all lasers in the game.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<laser> m_shots;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all nearby AI controlled ships in the game.
    //----------------------------------------------------------------------------------------------------------------------
    slotmap<enemy> m_agents;

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
    slotmap<ship> m_asteroids;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all particle sprites in the game (in the NGL version, these actually use
    /// procedural shaders, rather than bitmaps).
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<sprite> m_passiveSprites;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all squads.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<squad> m_squads;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Container for all factions.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<faction> m_factions;

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
		/// \brief This vector is used in conjunction with m_vel. It is zeroed out each frame, so we can use it to move the
		/// player
		//----------------------------------------------------------------------------------------------------------------------
		vec3 m_positioning;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief For the opengl background shader, I must track the position of the universe, even though
    /// it has no real practical meaning.
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_pos;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Stores partitions of entities, separated in a broad phase, to reduce On^2 checks.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<col_partition> m_partitions;

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
    void resolveCollision(slot _a, slot _b);
    void resolveCollision(ship * _a, ship * _b);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Text popups.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<popup> m_popups;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Represents balance of power between different factions.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<float> m_balanceOfPower;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Calculates power balance.
    //----------------------------------------------------------------------------------------------------------------------
    void calcPowerBalance();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Whether the map is in large (1) or small (0) mode.
    //----------------------------------------------------------------------------------------------------------------------
    bool m_mapExpanded;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of resources.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<debris> m_resources;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Ship ID that the context menu is bound to.
    //----------------------------------------------------------------------------------------------------------------------
    slot m_contextShip;

    int m_maxMiners;
    int m_maxWingmen;
    int m_minerCount;
    int m_wingmenCount;

    std::vector<debris> m_selectedItems;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Destroys agent at index _i of m_agents.
    /// \param _i Index of enemy to be destroyed.
    //----------------------------------------------------------------------------------------------------------------------
    void destroyAgent(size_t _i);
};

#endif
