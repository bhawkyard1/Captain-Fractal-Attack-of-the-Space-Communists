#ifndef SHIP_HPP
#define SHIP_HPP

#include <vector>
#include <array>
#include <unordered_map>

#include "SDL2/SDL.h"

#include "weapons.hpp"
#include "common.hpp"
#include "util.hpp"
#include "base.hpp"

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

extern std::vector<std::string> texture_keys;


std::string getTextureKey(ship_spec s);
ship_spec decrSpec(ship_spec s);

class ship: public base
{
  int targetAngle, curWeap, missiles;
  float angVel;
  float inertia, health, maxHealth, shield, maxShield, angle, energy, maxEnergy, coolDown, radius, damageTimer;
  std::string m_identifier;

  std::vector< std::array<float, 10> > m_weapons;

  ship_spec classification;

  bool shooting, accelerating, canMove, canShoot;

  enum energyPriority {NONE, SHIELDS, ENGINES, GUNS};
  energyPriority priority;

  //Cosmetics
  size_t engineGlow, steeringGlow, drawShot, shieldGlow;

  size_t upgrades[UPGRADES_LEN];
  float shieldMul, generatorMul;
public:
  ship(vec2, ship_spec, float);
  ship(ship&, vec2);

  void addVelS(vec2 v) {if(canMove) addVel(v);}
  void accelerate(float mult);
  void accelerate(vec2 _dir, float _mult);
  void dodge(float side);

  void update(float dt);
  void setTAng(float ang) {targetAngle = ang;}
  float getTAng() {return targetAngle;}
  void setAng(float ang) {angle = ang;}
  float getAng() {return angle;}
  void setShooting() {drawShot = 255;}
  void setWeap(int val) {curWeap = val;}
  void incrWeap(int val) {curWeap = clampRoll(curWeap + val, 0, static_cast<int>(m_weapons.size()) - 1 );}
  float getCurWeapStat(WEAPON_STAT stat) {return m_weapons[curWeap][stat];}
  std::vector<std::array<float, 10>> getWeaps() {return m_weapons;}
  std::array<float, 10> getWeap() {return m_weapons[curWeap];}
  int getCurWeap() {return curWeap;}
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

  bool getCanMove() {return canMove;}
  bool getCanShoot() {return canShoot;}

  bool inCombat() {return damageTimer > 0.0f;}

  std::string getIdentifier() {return m_identifier;}
  std::array<float, 4> getAlphaStats() {std::array<float, 4> ret = {static_cast<float>(engineGlow), static_cast<float>(steeringGlow), static_cast<float>(drawShot), static_cast<float>(shieldGlow)}; return ret;}
};

#endif
