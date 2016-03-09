#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "ship.hpp"
#include "vectors.hpp"

enum aiGoal{GOAL_IDLE, GOAL_CONGREGATE, GOAL_AVOID, GOAL_ATTACK, GOAL_FLEE, GOAL_TURRET, GOAL_SPACE_STATION};
enum aiTeam{TEAM_PLAYER, TEAM_PLAYER_MINER, GALACTIC_FEDERATION, SPOOKY_SPACE_PIRATES, NEUTRAL, NONE};

class enemy: public ship
{
  vec2 m_tPos, m_tVel;
  aiGoal m_squadGoal;
  aiGoal m_curGoal;
  ship * m_target;
  float m_stopDist;
  float m_confidence;
  aiTeam m_team;
  int m_squadID;
public:
  enemy(const vec2 _p, const vec2 _v, const ship_spec _type, const aiTeam _team);

  void setGoal(aiGoal g) {m_curGoal = g;}
  aiGoal getGoal() {return m_curGoal;}

  void behvrUpdate();
  void steering();

  aiTeam getTeam() const {return m_team;}
  void setTeam(const aiTeam _t) {m_team = _t;}

  ship * getTarget() const {return m_target;}
  void setTarget(ship * _t) {m_target = _t;}

  float getConfidence() {return m_confidence;}
  void setConfidence(float c) {m_confidence = c;}
  void decrConfidence(float d) {m_confidence -= d;}

  int getSquadID() const {return m_squadID;}
  void setSquadID(int _id) {m_squadID = _id;}

  void setTPos(vec2 _tPos) {m_tPos = _tPos;}
  void setTVel(vec2 _tVel) {m_tVel = _tVel;}
};

#endif
