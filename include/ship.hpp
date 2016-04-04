#ifndef SHIP_HPP
#define SHIP_HPP

#include <vector>
#include <array>
#include <unordered_map>

#include <SDL.h>

#include "weapons.hpp"
#include "common.hpp"
#include "util.hpp"
#include "base.hpp"

#define UPGRADES_LEN 4

enum ship_spec {
  COMMUNIST_1, COMMUNIST_2,
  FEDERATION_MKI, FEDERATION_MKII, FEDERATION_MKIII, FEDERATION_MKIV, FEDERATION_GUNSHIP,
  PIRATE_GNAT, PIRATE_CRUISER, PIRATE_WRANGLER, PIRATE_MARAUDER, PIRATE_GUNSHIP,
  PLAYER_MINER_DROID, PLAYER_TURRET, PLAYER_STATION, PLAYER_GRAVWELL, PLAYER_BARRACKS,
  PLAYER_SHIP,
  PLAYER_HUNTER, PLAYER_DEFENDER, PLAYER_DESTROYER,
  ION_MISSILE_MKI,
  ASTEROID_SMALL, ASTEROID_MID, ASTEROID_LARGE,
  SHIPS_END
};

struct tinfo
{
  std::string m_name;
  float m_radius;
};

extern std::vector<tinfo> g_texture_keys;

std::string getTextureKey(ship_spec _s);
ship_spec decrSpec(ship_spec _s);

class ship: public base
{
  int m_targetAngle;
  int m_curWeap;
  int m_missiles;
  float m_angVel;
  float m_inertia;
  float m_enginePower;
  float m_health;
  float m_maxHealth;
  float m_shield;
  float m_maxShield;
  float m_angle;
  float m_energy;
  float m_maxEnergy;
  float m_coolDown;
  float m_radius;
  float m_damageTimer;
  std::string m_identifier;

  std::vector< std::array<float, 10> > m_weapons;

  ship_spec m_classification;

  bool m_shooting;
  bool m_accelerating;
  bool m_canMove;
  bool m_canShoot;

  enum energyPriority {NONE, SHIELDS, ENGINES, GUNS};
  energyPriority m_priority;

  //Cosmetics
  size_t m_engineGlow;
  size_t m_steeringGlow;
  float m_drawShot;
  size_t m_shieldGlow;

  size_t m_upgrades[UPGRADES_LEN];
  float m_shieldMul;
  float m_generatorMul;
public:
  ship(const vec2 _p, const ship_spec _type, const float);
  ship(const ship& _src, const vec2 _p);
  ship(const ship_spec _type);

  void addVelS(const vec2 _v) {if(m_canMove) addVel(_v * m_inertia * m_enginePower);}
  void accelerate(const float _mult);
  void accelerate(const vec2 _dir, const float _mult);
  void setAccelerating(const bool _b) {m_accelerating = _b;}

  void dodge(const float _side);

  void update(const float _dt);
  void setTAng(const float _ang) {m_targetAngle = _ang;}
  float getTAng() const {return m_targetAngle;}
  void setAng(const float _ang) {m_angle = _ang;}
  float getAng() const {return m_angle;}
  void setWeapData(const int _val, const int _src) {m_weapons[_val] = g_weapons[_src];}
  void setWeap(const int _val) {m_curWeap = _val;}
  void incrWeap(const int _val) {m_curWeap = clampRoll(m_curWeap + _val, 0, static_cast<int>(m_weapons.size()) - 1 );}
  float getCurWeapStat(WEAPON_STAT _stat) const;
  std::array<float, 4> getCurWeapCol() const;
  std::array<float, 4> getShieldCol() const;
  std::vector<std::array<float, 10>> getWeaps() const {return m_weapons;}
  std::array<float, 10> getWeap() const {return m_weapons[m_curWeap];}
  int getCurWeap() const {return m_curWeap;}
  bool isFiring() const {return m_shooting;}
  void shoot() {m_drawShot = 255.0f;}
  void setFiring(const bool _v);

  void setMaxHealth(const float _h, const bool _match) {m_maxHealth = _h; if(_match) m_health = _h;}
  void setMaxShield(const float _s, const bool _match) {m_maxShield = _s; if(_match) m_shield = _s;}
  void setMaxEnergy(const float _e, const bool _match) {m_maxEnergy = _e; if(_match) m_energy = _e;}
  float getMaxHealth() const {return m_maxHealth;}
  float getMaxShield() const {return m_maxShield;}
  float getMaxEnergy() const {return m_maxEnergy;}

  void setHealth(const float _h) {m_health = _h;}
  float getHealth() const {return m_health;}
  void incrHealth(const float _v) {m_health += _v;}
  void setShield(const float _s) {m_shield = _s;}
  float getShield() const {return m_shield;}
  void setEnergy(const float _e) {m_energy = _e;}
  float getEnergy() const {return m_energy;}
  float getCooldown() const {return m_coolDown;}
  void setCooldown(const float _f);

  void setMissiles(const int _m) {m_missiles = _m;}
  void incrMissiles(const int _m) {m_missiles += _m;}
  int getMissiles() const {return m_missiles;}

  void setEnergyPriority(const int _v) {m_priority = static_cast<energyPriority>(_v);}
  int getEnergyPriority() const {return static_cast<int>(m_priority);}

  void damage(const float _d);
  void damage(const float _d, const vec2 _v);

  ship * getSelf() {return this;}

  int getUpgrade(const int _index) {if(_index < UPGRADES_LEN) return m_upgrades[_index]; return -1;}
  void setGradeArr(const int _i, const int _v) {m_upgrades[_i] = _v;}
  void setGrade(const int _i, const int _v);
  int upgrade(const int _i);

  void setInertia(const float _in) {m_inertia = _in;}
  float getInertia() const {return m_inertia;}

  void setEnginePower(const float _val) {m_enginePower = _val;}
  float getEnginePower() const {return m_enginePower;}

  ship_spec getClassification() const {return m_classification;}

  float getRadius() const {return m_radius;}

  int getScore() const;

  float getAngVel() const {return m_angVel;}

  bool getCanMove() const {return m_canMove;}
  bool getCanShoot() const {return m_canShoot;}

  bool inCombat() {return m_damageTimer > 0.0f;}

  std::string getIdentifier() const {return m_identifier;}
  std::array<float, 4> getAlphaStats() const {std::array<float, 4> ret = {static_cast<float>(m_engineGlow), static_cast<float>(m_steeringGlow), static_cast<float>(m_drawShot), static_cast<float>(m_shieldGlow)}; return ret;}
  float getShieldGlow() const {return m_shieldGlow;}
  float getLaserGlow() const {return m_drawShot / 255.0f;}
};

extern std::vector<ship> g_ship_templates;
#endif
