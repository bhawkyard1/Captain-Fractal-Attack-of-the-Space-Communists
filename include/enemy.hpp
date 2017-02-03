#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "aiTarget.hpp"
#include "aiTypes.hpp"
#include "faction.hpp"
#include "player.hpp"
#include "ship.hpp"
#include "vectors.hpp"

//----------------------------------------------------------------------------------------------------------------------
/// \file enemy.hpp
/// \brief This class is a ship with a brain, able to steer and react to input stimuli.
/// \author Ben Hawkyard
/// \version 1.0
/// \date 11/04/16
/// Revision History :
/// This is an initial version used for the game.
/// \class enemy
/// \brief Builds upon the ship class, adding variables like team, goal, targets etc.
//----------------------------------------------------------------------------------------------------------------------

class enemy: public ship
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief ctor for the enemy class
    /// \param _p position
    /// \param _v velocity
    /// \param _type the ship type (defined in the ship header)
    /// \param _team the ai team of this agent
    //----------------------------------------------------------------------------------------------------------------------
    enemy(const vec3 _p, const vec3 _v, const ship_spec _type, const aiTeam _team);

		void addXP(const float _xp);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for curent goal
    //----------------------------------------------------------------------------------------------------------------------
    void setGoal(aiGoal _g) {m_curGoal = _g;}
    aiGoal getGoal() {return m_curGoal;}

    void targetAcquisition(player &_ply, slotmap<enemy> &_enemies, slotmap<ship> &_asteroids, const std::vector<debris> &_resources, std::vector<faction> &_factions);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Updates the behaviour of the agent, based on any target it has, and its relative position
    /// \param _dt time difference since last update
    //----------------------------------------------------------------------------------------------------------------------
    void behvrUpdate(float _dt);

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Applies forces to the agent, attempting to steer it towards the target
    //----------------------------------------------------------------------------------------------------------------------
    void steering();

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the team
    //----------------------------------------------------------------------------------------------------------------------
    aiTeam getTeam() const {return m_team;}
    void setTeam(const aiTeam _t) {m_team = _t;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the target
    //----------------------------------------------------------------------------------------------------------------------
    ship * getTarget() {return m_target.get();}

    ship * getLastAttacker() {return m_lastAttacker.get();}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getters and setters for confidence
    //----------------------------------------------------------------------------------------------------------------------
    float getConfidence() {return m_confidence;}
    void setConfidence(float c) {m_confidence = c;}
    void decrConfidence(float d) {m_confidence -= d;}
    float getBaseConfidence() const {return m_baseConfidence;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for squad id
    //----------------------------------------------------------------------------------------------------------------------
    slot getSquadID() const {return m_squadID;}
    void setSquadID(slot _id) {m_squadID = _id;}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Getter and setter for the target position
    //----------------------------------------------------------------------------------------------------------------------
    void setTPos(vec3 _tPos) {m_tPos = _tPos;}
    vec3 getTPos() const {return m_tPos;}
    void setTVel(vec3 _tVel) {m_tVel = _tVel;}

    float damage(const float _d, const vec3 _v, aiTarget _attacker);
    void damage(const float _d);

		bool isFleeing() const {return m_curGoal == GOAL_FLEE_FROM or m_curGoal == GOAL_FLEE_TO;}
private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The target position
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_tPos;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The target velocity
    //----------------------------------------------------------------------------------------------------------------------
    vec3 m_tVel;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The current goal
    //----------------------------------------------------------------------------------------------------------------------
    aiGoal m_curGoal;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief A reference to the current target. Since the target need not be another enemy, it is of type ship
    //----------------------------------------------------------------------------------------------------------------------
    aiTarget m_target;

    aiTarget m_lastAttacker;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The distance this agent will attempt to hold around its target
    //----------------------------------------------------------------------------------------------------------------------
    float m_stopDist;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The confidence. If this drops too low, the enemy will flee
    //----------------------------------------------------------------------------------------------------------------------
		float m_baseConfidence;
    float m_confidence;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The team of the ship
    //----------------------------------------------------------------------------------------------------------------------
    aiTeam m_team;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The id of this agents' squad
    //----------------------------------------------------------------------------------------------------------------------
    slot m_squadID;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The agent will ignore enemies that are further away than this distance
    //----------------------------------------------------------------------------------------------------------------------
    float m_aggroRadius;
};

#endif
