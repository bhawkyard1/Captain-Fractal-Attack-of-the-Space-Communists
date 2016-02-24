#include "weapons.hpp"
#include "src/stardust.cpp"
#include "src/stardust_sprite.cpp"
#include "src/ship.cpp"
#include "src/ship_presets.cpp"
#include "src/enemy.cpp"
#include "src/laser.cpp"
#include "src/player.cpp"
#include "src/pfx.cpp"
#include "src/missile.cpp"

using namespace std;

struct col_partition
{
	vector< vector<enemy*> > ships;
	vector< vector<laser*> > lasers;
	vector< vector<missile*> > rockets;
	vector< vector<ship*> > rocks;
	vector<SDL_Rect> rects;
};

class universe
{
	vector<stardust> dots;
	vector<stardust_sprite> sparkles;
	vector<laser> shots;
	vector<enemy> enemies;
	vector<pfx> particles;
	vector<missile> missiles;
	vector<ship> asteroids;
	vector<stardust_sprite> passive_sprites;
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
	
	void detectCollisions(SDL_Rect, vector<enemy*>, vector<laser*>, vector<missile*>, vector<ship*>, unsigned short int);
	void checkCollisions();
	
	void addpfx(vec2,vec2,vec2,int,float);
	void addParticleSprite(vec2,vec2, float, SDL_Texture*);
	vector<enemy>* getEnemies() {return &enemies;}
	vector<laser>* getShots() {return &shots;}
	vector<missile>* getMissiles() {return &missiles;}
	vector<ship>* getAsteroids() {return &asteroids;}
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
