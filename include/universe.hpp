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

struct col_partition
{
    std::vector< std::vector<enemy*> > ships;
    std::vector< std::vector<laser*> > lasers;
    std::vector< std::vector<missile*> > rockets;
    std::vector< std::vector<ship*> > rocks;
    std::vector<SDL_Rect> rects;
};

class universe
{
    bool showUI;
    ui::userInterface m_ui;
    #if RENDER_MODE == 0
      renderer m_drawer;
      std::vector<stardust> m_dots;
      std::vector<stardust_sprite> m_sparkles;
    #elif RENDER_MODE == 1
      renderer_ngl m_drawer;
    #endif

    std::vector<laser> m_shots;
    std::vector<enemy> m_agents;
    std::vector<pfx> m_particles;
    std::vector<missile> m_missiles;
    std::vector<ship> m_asteroids;
    std::vector<stardust_sprite> m_passiveSprites;
    std::vector<squad> m_squads;
    std::vector<faction> m_factions;

    std::vector<int> m_factionCounts;
    std::vector<int> m_factionMaxCounts;

    player m_ply;
    vec3 m_vel;
    vec3 m_pos;

    col_partition m_partitions;

    int m_score;
    std::array<float, 3> m_cCol;
    std::array<float, 3> m_tCol;
    float m_gameplay_intensity = 1;
    bool m_paused;
    int m_mouse_state;
    bool m_escMenuShown;
    double m_time_elapsed;
public:
    universe();
    void initUI();
    player * getPly() {return &m_ply;}
    void setVel(const vec3 _v) {m_vel = _v;}
    void addShot(const vec3 _p, const vec3 _v, const float _angle, const std::array<float, WEAPS_W> _weap, const aiTeam _team);
    void addMissile(const vec3 _p, const vec3 _v, const float _angle, const aiTeam _team);
    void spawnShip(const aiTeam t);
    void spawnShip(const aiTeam _t, const vec3 _p);
    void spawnSquad(const aiTeam _t, const float _min, const float _max, const int _i);
    void addWingman() {m_factionMaxCounts[TEAM_PLAYER]++;}
    void addMiner() {m_factionMaxCounts[TEAM_PLAYER_MINER]++;}
    void addBuild(const vec3 _p, const ship_spec _type);
    void addBuild(const ship_spec _type);

    void update(const float _dt);

    void clear() {m_drawer.clear();}
    void draw(const float _dt);
    void drawUI();
    void swap() {m_drawer.finalise();}

    void detectCollisions(const SDL_Rect _box, std::vector<enemy*> _ships, std::vector<laser*> _lasers, std::vector<missile*> _rockets, std::vector<ship*> _rocks, unsigned short int _lvl);
    void checkCollisions();

    void addpfx(const vec3 _p, const vec3 _v, const vec3 _wv, const int _no, const float _f);
    void addParticleSprite(const vec3 _p, const vec3 _v, const std::string _tex);
    std::vector<enemy>* getAgents() {return &m_agents;}
    std::vector<laser>* getShots() {return &m_shots;}
    std::vector<missile>* getMissiles() {return &m_missiles;}
    std::vector<ship>* getAsteroids() {return &m_asteroids;}
    ship * closestEnemy(const vec3 _p, const aiTeam _t);
    void setScore(const int _s) {m_score = _s; m_ui.update(m_score);}
    void addScore(const int _s) {m_score += _s; m_ui.update(m_score);}
    int getScore() const  {return m_score;}
    int * getScorePt() {return &m_score;}

    int getMaxEnemyCount(aiTeam _t) const {return m_factionMaxCounts[_t];}
    int getEnemyCount(aiTeam _t) const {return m_factionCounts[_t];}

    void setMaxEnemyCount(const int m, const aiTeam _t) {m_factionMaxCounts[_t] = m;}
    int getMaxWingmanCount() const {return m_factionMaxCounts[TEAM_PLAYER];}
    void setMaxWingmanCount(const int m) {m_factionMaxCounts[TEAM_PLAYER] = m;}
    int getMaxMinerCount() const {return m_factionMaxCounts[TEAM_PLAYER_MINER];}
    void setMaxMinerCount(const int m) {m_factionMaxCounts[TEAM_PLAYER_MINER] = m;}
    bool atMaxCount(aiTeam _t) const {return m_factionCounts[_t] < m_factionMaxCounts[_t];}

    void reload(const bool _newGame);
    void loadShips();

    void pause() {m_paused = !m_paused; if(!m_paused) g_TIME_SCALE = 1;}
    void setPause(bool _p) {m_paused = _p;}
    bool isPaused() const {return m_paused;}

    ui::userInterface * getUI() {return &m_ui;}
    ui::userInterface setUI(ui::userInterface _i) {m_ui = _i;}
    bool upgradeCallback(const int _sel, const int _btn);
    void upgradeSetLabels(const int _sel, const int _btn, const int _plvl);

    void setMouseState(const int _i) {m_mouse_state = _i;}
    int getMouseState() const {return m_mouse_state;}

    void addToSquad(enemy * _e, squad * _s) {_e->setSquadID(_s->m_id); _s->m_size++;}
    void removeFromSquad(enemy * _e, squad * _s) {_e->setSquadID(-1); if(_s != nullptr) _s->m_size--;}
    //ship& getShipTemplate(ship_spec i) {return m_ship_templates[i];}
    void createFactions();
    std::vector<faction> * getFactions() {return &m_factions;}
    squad * getSquadFromID(int _id);

#if RENDER_MODE == 0
    renderer * getRenderer() {return &m_drawer;}
#elif RENDER_MODE == 1
    renderer_ngl * getRenderer() {return &m_drawer;}
#endif
    bool isEscMenuShown() {return m_escMenuShown;}
    void escMenuTog() {m_escMenuShown = !m_escMenuShown;}

    bool UIVisible() const {return showUI;}
    void setUIVisible(const bool _b) {showUI = _b;}
    void toggleUIVisible() {showUI = !showUI;}

    void shipAddParent(ship *_parent, ship *_child, vec3 _offset);
};

#endif
