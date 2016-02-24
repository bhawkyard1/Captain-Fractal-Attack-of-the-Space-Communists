#include "base.cpp"

#define UPGRADES_LEN 4

enum ship_spec {
        FEDERATION_MKI, FEDERATION_MKII, FEDERATION_MKIII, FEDERATION_MKIV, FEDERATION_GUNSHIP,
				PIRATE_GNAT, PIRATE_CRUISER, PIRATE_WRANGLER, PIRATE_MARAUDER, PIRATE_GUNSHIP,
				PLAYER_MINER_DROID, PLAYER_TURRET, PLAYER_STATION, PLAYER_GRAVWELL, PLAYER_BARRACKS,
				PLAYER_SHIP,
				PLAYER_HUNTER, PLAYER_DEFENDER, PLAYER_DESTROYER,
				ION_MISSILE_MKI,
				ASTEROID_SMALL, ASTEROID_MID, ASTEROID_LARGE,
				SHIPS_END
				};

ship_spec decrSpec(ship_spec s) {return static_cast<ship_spec>( static_cast<int>(s) - 1 );}

class ship: public base
{
	int targetAngle, curWeap, missiles;
	float angVel;
	float inertia, health, maxHealth, shield, maxShield, angle, energy, maxEnergy, coolDown, radius, damageTimer;
	SDL_Rect dstrect;
	vector<SDL_Texture*> shipTex;
	
	ship_spec classification;
	
	bool shooting, accelerating, canMove, canShoot;
	
	enum energyPriority {NONE, SHIELDS, ENGINES, GUNS};
	energyPriority priority;
	
	//Cosmetics
	size_t engineGlow, steeringGlow, drawShot, shieldGlow;
	
	size_t upgrades[UPGRADES_LEN];
	float shieldMul, generatorMul;
public:
	ship(vec2, ship_spec);
	ship(ship&, vec2);
	void memFree();
	vector<SDL_Texture*> getTex() {return shipTex;}
	
  void addVelS(vec2 v) {if(canMove) addVel(v);}
	void accelerate(double mult);
	void dodge(float side);
	
	SDL_Rect* getRekt() {return &dstrect;}
	
	void update(float dt);
	void setTAng(float ang) {targetAngle = ang;}
	float getTAng() {return targetAngle;}
	void setAng(float ang) {angle = ang;}
	float getAng() {return angle;}
	void draw(float);
	void setShooting() {drawShot = 255;}
	void setWeap(int val) {curWeap = val;}
	void incrWeap(int val) {if(DEV_MODE) curWeap = clampRoll(curWeap + val,0,16); else curWeap = clampRoll(curWeap + val,0,2);}
	int getWeap() {return curWeap;}
	bool isFiring() {return shooting;}
	void setFiring(bool v) {shooting = v;}
	
	void setMaxHealth(float h, bool match) {maxHealth = h; if(match) health = h;}
	void setMaxShield(float s, bool match) {maxShield = s; if(match) shield = s;}
	void setMaxEnergy(float e, bool match) {maxEnergy = e; if(match) energy = e;}
	float getMaxHealth() {return maxHealth;}
	float getMaxShield() {return maxShield;}
	float getMaxEnergy() {return maxEnergy;}
	
	void setHealth(float h) {health = h;}
	float getHealth() {return health;}
	void incrHealth(float v) {health += v;}
	void setShield(float s) {shield = s;}
	float getShield() {return shield;}
	void setEnergy(float e) {energy = e;}
	float getEnergy() {return energy;}
	float getCooldown() {return coolDown;}
	void setCooldown(float f) {coolDown = f;}
	
	void setMissiles(int m) {missiles = m;}
	void incrMissiles(int m) {missiles += m;}
	int getMissiles() {return missiles;}
	
	void setEnergyPriority(int v) {priority = static_cast<energyPriority>(v);}
	
	void damage(float d);
	
	ship * getSelf() {return this;}
	
	int getUpgrade(int index) {if(index < UPGRADES_LEN) return upgrades[index]; return -1;}
	void setGradeArr(int i, int v) {upgrades[i] = v;}
	void setGrade(int i, int v);
	int upgrade(int i);
	
	void setInertia(float in) {inertia = in;}
	float getInertia() {return inertia;}
	
	ship_spec getClassification() {return classification;}
	
	float getRadius() {return radius;}
	
	int getScore();
	
	float getAngVel() {return angVel;}
	
	bool isStatic() {return !canMove;}
	bool isLaserless() {return !canShoot;}
	
	bool inCombat() {return damageTimer > 0.0f;}
};
