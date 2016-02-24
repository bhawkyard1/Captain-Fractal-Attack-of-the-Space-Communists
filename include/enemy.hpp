#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "ship.hpp"
#include "vectors.hpp"

enum ai_goal{GOAL_IDLE, GOAL_CONGREGATE, GOAL_AVOID, GOAL_ATTACK, GOAL_FLEE, GOAL_TURRET, GOAL_SPACE_STATION};
enum ai_team{TEAM_PLAYER, TEAM_PLAYER_MINER, GALACTIC_FEDERATION, SPOOKY_SPACE_PIRATES, NEUTRAL, NONE};

class enemy: public ship
{
	vec2 tPos, tVel;
	ai_goal curGoal;
	ship * target;
	float stopDist, confidence;
	ai_team team;
public:
	enemy(vec2, vec2, ship_spec, ai_team);
	
	void setGoal(ai_goal g) {curGoal = g;}
	ai_goal getGoal() {return curGoal;}
	
	void behvrUpdate();
	void steering();
	
	ai_team getTeam() {return team;}
	void setTeam(ai_team t) {team = t;}
		
	ship * getTarget() {return target;}
	void setTarget(ship * t) {target = t;}
	
	float getConfidence() {return confidence;}
	void setConfidence(float c) {confidence = c;}
	void decrConfidence(float d) {confidence -= d;}
};

#endif
