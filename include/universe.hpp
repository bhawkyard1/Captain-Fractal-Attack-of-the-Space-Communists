#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "weapons.hpp"
#include "stardust.hpp"
#include "stardust_sprite.hpp"
#include "ship.hpp"
#include "enemy.hpp"
#include "laser.hpp"
#include "player.hpp"
#include "pfx.hpp"
#include "missile.hpp"
#include "renderer.hpp"
#include "ui/interface.hpp"
#include "squad.hpp"
#include "faction.hpp"

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
    interface m_ui;
    renderer m_drawer;
    std::vector<stardust> dots;
    std::vector<stardust_sprite> sparkles;
    std::vector<laser> shots;
    std::vector<enemy> enemies;
    std::vector<pfx> particles;
    std::vector<missile> missiles;
    std::vector<ship> asteroids;
    std::vector<stardust_sprite> passive_sprites;
    std::vector<squad> m_squads;
    std::vector<faction> m_factions;

    std::vector<int> m_factionCounts;
    std::vector<int> m_factionMaxCounts;

    player ply;
    vec2 vel;

    col_partition partitions;

    int score;
    float cColP[3];
    float tColP[3];
    float gameplay_intensity = 1;
    bool paused;
    int m_mouse_state;
public:
    universe();
    void initUI();
    player * getPly() {return &ply;}
    void setVel(vec2 v) {vel = v;}
    void addShot(vec2 p, vec2 v, float ang, std::array<float, WEAPS_W> weap, aiTeam team);
    void addMissile(vec2 p, vec2 v, float angle, aiTeam _team);
    void spawnShip(aiTeam t);
    void spawnShip(aiTeam t, vec2 p);
    void addWingman() {m_factionMaxCounts[TEAM_PLAYER]++;}
    void addMiner() {m_factionMaxCounts[TEAM_PLAYER_MINER]++;}
    void addBuild(vec2,ship_spec);
    void addBuild(ship_spec);
    void update(float);
    void draw(float);
    void drawUI();

    void detectCollisions(SDL_Rect _box, std::vector<enemy*> _ships, std::vector<laser*> _lasers, std::vector<missile*> _rockets, std::vector<ship*> _rocks, unsigned short int _lvl);
    void checkCollisions();

    void addpfx(vec2,vec2,vec2,int,float);
    void addParticleSprite(vec2,vec2, float, std::string);
    std::vector<enemy>* getEnemies() {return &enemies;}
    std::vector<laser>* getShots() {return &shots;}
    std::vector<missile>* getMissiles() {return &missiles;}
    std::vector<ship>* getAsteroids() {return &asteroids;}
    ship * closestEnemy(vec2 p, aiTeam t);
    void setScore(int _s) {score = _s; m_ui.update(score);}
    void addScore(int _s) {score += _s; m_ui.update(score);}
    int getScore() {return score;}
    int * getScorePt() {return &score;}

    int getMaxEnemyCount() {return m_factionMaxCounts[GALACTIC_FEDERATION];}
    void setMaxEnemyCount(int m) {m_factionMaxCounts[GALACTIC_FEDERATION] = m; m_factionMaxCounts[SPOOKY_SPACE_PIRATES] = m;}
    int getMaxWingmanCount() {return m_factionMaxCounts[TEAM_PLAYER];}
    void setMaxWingmanCount(int m) {m_factionMaxCounts[TEAM_PLAYER] = m;}
    int getMaxMinerCount() {return m_factionMaxCounts[TEAM_PLAYER_MINER];}
    void setMaxMinerCount(int m) {m_factionMaxCounts[TEAM_PLAYER_MINER] = m;}
    bool atMaxCount(aiTeam _t) {return m_factionCounts[_t] < m_factionMaxCounts[_t];}

    void reload(bool);
    void loadShips();

    void pause() {paused = !paused; if(!paused) g_TIME_SCALE = 1;}
    bool isPaused() {return paused;}

    interface * getUI() {return &m_ui;}
    bool upgradeCallback(int _sel, int _btn);
    void upgradeSetLabels(int _sel, int _btn, int _plvl);

    void setMouseState(int _i) {m_mouse_state = _i;}
    int getMouseState() {return m_mouse_state;}

    void addToSquad(enemy * _e, squad * _s) {_e->setSquadID(_s->m_id); _s->m_size++;}
    void removeFromSquad(enemy * _e, squad * _s) {_e->setSquadID(-1); _s->m_size--;}
    //ship& getShipTemplate(ship_spec i) {return m_ship_templates[i];}
    void createFactions();
    std::vector<faction> * getFactions() {return &m_factions;}
};

#endif
