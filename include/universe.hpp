#ifndef UNIVERSE_HPP
#define UNIVERSE_HPP

#include "weapons.hpp"
#include "stardust.hpp"
#include "stardust_sprite.hpp"
#include "ship.hpp"
#include "ship_presets.hpp"
#include "enemy.hpp"
#include "laser.hpp"
#include "player.hpp"
#include "pfx.hpp"
#include "missile.hpp"

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
  std::vector<stardust> dots;
  std::vector<stardust_sprite> sparkles;
  std::vector<laser> shots;
  std::vector<enemy> enemies;
  std::vector<pfx> particles;
  std::vector<missile> missiles;
  std::vector<ship> asteroids;
  std::vector<stardust_sprite> passive_sprites;
	player ply;
	vec2 vel;
	
	col_partition partitions;
	
	int score, max_enemies_count, max_wingmen_count, max_miner_count, enemy_count, wingmen_count, miner_count;
	float cColP[3];
	float tColP[3];
	float gameplay_intensity = 1;
	bool paused;
	SDL_Texture * sky;
public:
	universe();
	player * getPly() {return &ply;}
	void setVel(vec2 v) {vel = v;}
	void addShot(vec2 p, vec2 v, float ang, int weap, ai_team team);
	void addMissile(vec2 p, vec2 v, float angle, bool team);
	void spawnShip(ai_team t);
	void spawnShip(ai_team t, vec2 p);
	void addWingman() {max_wingmen_count++;}
	void addMiner() {max_miner_count++;}
	void addBuild(vec2,ship_spec);
	void addBuild(ship_spec);
	void update(int,float);
	void draw(float);
	
  void detectCollisions(SDL_Rect, std::vector<enemy*>, std::vector<laser*>, std::vector<missile*>, std::vector<ship*>, unsigned short int);
	void checkCollisions();
	
	void addpfx(vec2,vec2,vec2,int,float);
	void addParticleSprite(vec2,vec2, float, SDL_Texture*);
  std::vector<enemy>* getEnemies() {return &enemies;}
  std::vector<laser>* getShots() {return &shots;}
  std::vector<missile>* getMissiles() {return &missiles;}
  std::vector<ship>* getAsteroids() {return &asteroids;}
	ship * closestEnemy(vec2 p, ai_team t);
	void setScore(int s) {score = s;}
	int getScore() {return score;}
	int * getScorePt() {return &score;}
	
	int getMaxEnemyCount() {return max_enemies_count;}
	void setMaxEnemyCount(int m) {max_enemies_count = m;}
	int getMaxWingmanCount() {return max_wingmen_count;}
	void setMaxWingmanCount(int m) {max_wingmen_count = m;}
	int getMaxMinerCount() {return max_miner_count;}
	void setMaxMinerCount(int m) {max_miner_count = m;}
	
	void reload(bool);
	
	void pause() {paused = !paused;}
	bool isPaused() {return paused;}
};

#endif
