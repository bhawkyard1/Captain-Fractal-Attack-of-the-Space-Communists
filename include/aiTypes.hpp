#ifndef AI_TYPES_HPP
#define AI_TYPES_HPP

//----------------------------------------------------------------------------------------------------------------------
/// \brief The goals that a ship can have
//----------------------------------------------------------------------------------------------------------------------
enum aiGoal{GOAL_IDLE, GOAL_CONGREGATE, GOAL_AVOID, GOAL_ATTACK, GOAL_FLEE_FROM, GOAL_FLEE_TO, GOAL_TURRET, GOAL_SPACE_STATION, GOAL_WANDER, GOAL_TRADE, GOAL_RETREAT, GOAL_GOTO};

//----------------------------------------------------------------------------------------------------------------------
/// \brief The teams that a ship can be on
//----------------------------------------------------------------------------------------------------------------------
enum aiTeam{TEAM_PLAYER, GALACTIC_FEDERATION, SPOOKY_SPACE_PIRATES, SPACE_COMMUNISTS, ALLIANCE, NEUTRAL, NONE};

enum diplomaticStatus {DIPLOMACY_ENEMY, DIPLOMACY_NEUTRAL, DIPLOMACY_FRIEND, DIPLOMACY_SELF};

#endif
