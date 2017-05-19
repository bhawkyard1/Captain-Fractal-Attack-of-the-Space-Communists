#include "enemy.hpp"

enemy::enemy(
		const vec3 _p ,
		const vec3 _v,
		const ship_spec _type,
		const aiTeam _team
		)
	:
		ship(g_ship_templates[_type], _p)
{	
	m_curGoal = GOAL_IDLE;
	setPos(_p);
	setPPos(_p);
	setVel(_v);
	m_stopDist = randNum(200.0f,600.0f);
	m_aggroRadius = randNum(2000.0f, 2200.0f);
	if(_type == PLAYER_MINER_DROID) m_stopDist = randNum(20.0f, 60.0f);
	m_target = aiTarget();
	m_lastAttacker = aiTarget();
	m_curGoal = GOAL_IDLE;
	m_team = _team;
	m_baseConfidence = randNum(getMaxHealth() * 0.8f, getMaxHealth() * 1.2f);
	m_confidence = m_baseConfidence;//-1.0f;
	m_squadID = {0, -1};
	m_tPos = getPos();
}

void enemy::targetAcquisition(player &_ply, slotmap<enemy> &_enemies, slotmap<ship> &_asteroids, const std::vector<debris> &_resources, std::vector<faction> &_factions)
{
	debug("target acquisition start");
	//std::cout << "Enemies address is " << &_enemies << '\n';
	aiTarget curTarget = m_target;
	m_target = aiTarget();

	//std::cout << getIdentifier() << " : targetAcquisition start, flag is " << m_target.getFlag() << '\n';

	ship * lastAttacker = m_lastAttacker.get();

	if(getType() != SHIP_TYPE_MINER)
	{
		//Lowest weight wins.
		//Enemy hostile targeting.
		float bestWeight = F_INF;
		for(size_t i = 0; i < _enemies.m_objects.size(); ++i)
		{
			if(
				 _factions[getTeam()].isNeutral(_enemies[i].getTeam()) and
				 _factions[_enemies[i].getTeam()].isNeutral(getTeam())
				 ) continue;
			//Distance as a base.
			float weight = magns(getPos() - _enemies[i].getPos());

			//Skip this enemy if it is too far away.
			if(weight > sqr(m_aggroRadius)) continue;

			weight = sqrt(weight);
			//Concentrate on active / unweakened combatants.
			weight *= _enemies[i].getHealth() / _enemies[i].getMaxHealth() + 0.5f;
			//Do not target enemies shooting by sideways.
			//weight /= clamp( udot(getVel(), _enemies[i].getVel()), 0.001f, 1.0f );
			//If this is the agent's current target, prioritise.
			if(_enemies[i].getGoal() == GOAL_FLEE_FROM)
				weight *= 2.0f;

			if(curTarget.get() != nullptr and  curTarget.get() == &_enemies[i] )
				weight /= 3.0f;
			//Pursue last attacker.
			if( lastAttacker != nullptr and lastAttacker == &_enemies[i] )
				weight /= 2.0f;

			if(weight < bestWeight)
			{
				bestWeight = weight;
				slotmap<ship> * pt = reinterpret_cast<slotmap<ship>*>(&_enemies);
				m_target.setAgent( slotID<ship>( _enemies.getID(i), pt ) );
				//std::cout << "	set target to " << _enemies[i].getIdentifier() << "deriving from address " << &_enemies << " which was converted to " << pt << ", flag is " << m_target.getFlag() << '\n';

				if(!isFleeing())
					setGoal(GOAL_ATTACK);
			}
		}

		//Player hostile targeting
		if( _factions[getTeam()].isEnemy(TEAM_PLAYER) and !g_GAME_OVER )
		{
			float weight = magns(getPos() - _ply.getPos());

			if(weight < sqr(m_aggroRadius))
			{
				weight = sqrt(weight);
				//Concentrate on active / unweakened combatants.
				weight *= _ply.getHealth() / _ply.getMaxHealth() + 0.5f;
				//Do not target enemies shooting by sideways.
				//weight /= clamp( udot(getVel(), _ply.getVel()), 0.001f, 1.0f );
				//Pursue last attacker.
				if( lastAttacker == &_ply )
					weight /= 2.0f;

				if(weight < bestWeight)
				{
					bestWeight = weight;
					m_target.setPlayer( static_cast<ship*>(&_ply) );
					//std::cout << "	set target to " << m_target.get()->getIdentifier() << ", flag is " << m_target.getFlag() << '\n';
					if(!isFleeing())
						setGoal(GOAL_ATTACK);
				}
			}
		}
	}
	//Miner targeting
	else if(getType() == SHIP_TYPE_MINER)
	{
		float bestDist = F_INF;
		//Find the closest asteroid.
		for(size_t i = 0; i < _asteroids.size(); ++i)
		{
			float nd = magns(getPos() - _asteroids[i].getPos());
			if(nd < bestDist)
			{
				m_target.setAsteroid( _asteroids.getSlotID( i ) );
				setGoal(GOAL_ATTACK);
				bestDist = nd;
			}
		}
		//Find the closest debris.
		if(!getCargo()->full()) {
			for(auto &d : _resources)
			{
				float nd = magns(getPos() - d.getPos());
				if(nd < bestDist)
				{
					m_target.unset();
					setTPos( d.getPos() );
					setTVel( vec3() );
					setGoal( GOAL_GOTO );
					bestDist = nd;
				}
			}
		}
	}

	float nd = magns(_ply.getPos() - getPos());
	float fd = 2000.0f;

	//If the agent can move, is friendly to the player, and close by, and not in combat.
	if(getCanMove() and getTeam() == TEAM_PLAYER and nd > fd * fd and !inCombat())
	{
		m_target.setPlayer( dynamic_cast<ship*>(&_ply) );
		setGoal( GOAL_CONGREGATE );
	}
	else if( getTarget() == nullptr and m_curGoal != GOAL_FLEE_FROM and m_curGoal != GOAL_GOTO )
	{
		//If the agent has no m_target, it becomes idle.
		setGoal( GOAL_WANDER );
	}

	squad * sq = _factions[m_team].getSquad(m_squadID);
	if(sq != nullptr)
	{
		//If too far from group center, congregate at center.
		vec3 squadPos = sq->m_targetPos;
		//std::cout << "squadPos : " << squadPos.m_x << " ," << squadPos.m_y << '\n';
		vec3 squadVel = sq->m_averageVel;
		if(!inCombat() and magns(getPos() - squadPos) > sqr(sq->m_regroupDist) )
		{
			m_target.unset();
			//std::cout << "	target unset due to squad distance!\n";
			setTPos( squadPos );
			setTVel( squadVel );
			setGoal( GOAL_CONGREGATE );
		}
	}

	//Dump resources at base.
	if(!getCargo()->canAddMoreItems())
	{
		float best = F_INF;
		for(size_t i = 0; i < _enemies.size(); ++i)
		{
			if(m_team != _enemies[i].getTeam()) continue;
			if(!_enemies[i].getCanMove())
			{
				float temp = magns(_enemies[i].getPos() - getPos());
				if(temp < best)
				{
					best = temp;
					m_target.setAgent( slotID<ship>( _enemies.getID(i), reinterpret_cast< slotmap<ship> *>(&_enemies) ) );
					setGoal(GOAL_TRADE);
				}
			}
		}
	}

	/*std::cout << "p1\n";
	if(m_target.get() != nullptr)
		std::cout << "Target type : " << m_target.get()->getClassification() << ", " << m_target.get()->getIdentifier() << '\n';
	std::cout << "p2\n";*/

	//std::cout << "targetAcquisition end, flag is " << m_target.getFlag() << '\n';
	//If fleeing, look for a place to conduct repairs.
	if(m_curGoal == GOAL_FLEE_FROM)
	{
		float m = F_INF;
		for(size_t e = 0; e < _enemies.size(); ++e)
		{
			if( _enemies[e].canStoreShips() and _enemies[e].getTeam() == m_team )
			{
				float d = magns( _enemies[e].getPos() - getPos() );
				if(d < m)
				{
					m = d;
					setGoal( GOAL_FLEE_TO );
					m_target.setAgent( slotID<ship>( _enemies.getID(e), reinterpret_cast< slotmap<ship> *>(&_enemies) ) );
				}
			}
		}
	}

	debug("target acquisition end");
}

void enemy::addXP(const float _xp)
{
	m_baseConfidence += _xp;
	ship::addXP(_xp);
}

void enemy::behvrUpdate(float _dt)
{
	//Setting energy priorities----------------------------------------------------------------------------//
	if(getConfidence() < getMaxHealth() * 0.25f) setEnergyPriority(2);
	else if(getHealth() < getMaxHealth() * 0.75f) setEnergyPriority(1);
	else setEnergyPriority(0);
	//-----------------------------------------------------------------------------------------------------//

	float hp = getHealth() / getMaxHealth();

	//Recover confidence slower when fleeing.
	if(m_curGoal == GOAL_FLEE_FROM or m_curGoal == GOAL_FLEE_TO)
		m_confidence = std::min( m_confidence + _dt * 0.5f, hp * m_baseConfidence );
	else
		m_confidence = std::min( m_confidence + _dt * 1.5f, hp * m_baseConfidence );

	ship * t = m_target.get();
	if(t != nullptr)
	{
		float dist = mag(getPos() - t->getPos());
		vec3 diff = (t->getPos() - getPos()) / dist;
		m_tPos = t->getPos();
		if(m_curGoal == GOAL_ATTACK)
			m_tPos -= diff * fmin(t->getRadius(), dist);
		m_tVel = t->getVel();
	}

	if(m_curGoal == GOAL_FLEE_FROM)
	{
		if(t != nullptr)
		{
			m_tPos = getPos() + (getPos() - t->getPos()) * 100.0f;
			m_tVel = {0.0f, 0.0f, 0.0f};
		}
		else
		{
			m_tPos = getPos();
			m_tPos.m_y += 10000.0f;
			m_tVel = {0.0f, 0.0f, 0.0f};
		}
	}
	else if(m_curGoal == GOAL_WANDER)
	{
		m_tVel = {0.0f, 0.0f, 0.0f};
		m_tPos += getWVel() * g_PIXEL_UNIT_CONVERSION * _dt;;
		if(magns(getPos() - m_tPos) < 10000.0f)
		{
			m_tPos += tovec3(randVec2(2000.0f));
		}
	}
	else if(m_curGoal == GOAL_IDLE)
	{
		m_tPos = {randNum(-30000.0f, 30000.0f), randNum(-30000.0f, 30000.0f), 0.0f};
		m_tVel = {0.0f, 0.0f, 0.0f};
		m_curGoal = GOAL_IDLE;
	}
	else if(m_curGoal == GOAL_RETREAT)
	{
		m_tPos = getPos() * 2;
		m_tVel = {0.0f, 0.0f, 0.0f};
	}
}

void enemy::steering()
{
	setFiring(false);

	if(m_curGoal == GOAL_IDLE or m_curGoal == GOAL_SPACE_STATION) return;
	vec3 p = getPos();
	vec3 v = getVel();
	vec3 uv = unit(v);
	vec3 diff = m_tPos - p;
	diff.m_z = 0.0f;
	vec3 utv = unit(diff);

	//This is the distance between the ship and its m_target position.

	float dist = mag(diff);

	float radius = 0.0f;
	ship * t = m_target.get();
	if(t != nullptr)
		radius = t->getRadius();

	//Reverse if too close.
	if(dist < getRadius() and getCanMove()) accelerate(-1.0f);

	//This is the closing speed. Add player and ship vectors, find magnitude.
	//Initially I didn't know whether the vectors were converging or diverging.
	//I solved it by multiplying by the dot of the ship and m_target vectors.
	float cSpd = mag( v - m_tVel );// * dot(utv, uv);

	//Whereas angleMul is all about the ships angle in relation to its m_target angle, this is about its vector in relation to its m_target angle.
	//ie where the ship is going vs where is should be going.
	float vecMul = dot(uv, utv);

	/*
				*The distance it will take the ship to stop, at max deceleration.
				*
				* v^2 = u^2 + 2as
				* -2as = u^2 - v^2
				* s = (u^2 - v^2)/-2a
				* s = (u^2 - v^2)/-2a * (-1/-1)
				* s = (v^2 - u^2)/2a
				* given v = spd, u = 0, a = -1:
				*/
	float stoppingDistance = 4 * ( cSpd * cSpd );

	if(vecMul < 0.0f) stoppingDistance *= -1;

	//This controls how much the ship is to accelerate. It depends on the closing speed between the ship and its m_target, their distance apart, and whether the ship is moving towards the m_target, or away.
	float accelMul;

	if(m_curGoal == GOAL_GOTO or m_curGoal == GOAL_TRADE)
		accelMul = dist - stoppingDistance;
	else
		accelMul = dist - stoppingDistance - m_stopDist - radius ;
	accelMul = clamp(accelMul / 50.0f, -1.0f, 0.5f);

	//We need to compare the distance to the closing speed. If the two points are converging rapidly, the ship must slow down.
	//cSpd will be subtracted from dist once per frame. Ships can accelerate at max 1au per frame.
	//Therefore, frames until collision: f = dist/cSpd

	//vec2 linePos = closest(m_tPos,unit(normal(v)),p);

	//m_tPos -= unit(linePos - m_tPos) * cSpd * 3;

	//Angle the ship towards its m_target.
    float ta = -deg( ang( tovec2(p - (m_tPos + m_tVel) ) ) );
    setTAng(clampRoll( ta, -180.0f, 180.0f));
     //std::cout << "TANG is " << getTAng() << '\n';

	//The proportion of the ships total energy it still has.
	float energyProportion = getEnergy() / getMaxEnergy();
	//Whether the ship is going towards or away from the target.
	float towardsOrAway = dot(utv, uv);
	//When travelling towards target, can use energy until 25% is left. When travelling away, can use until 10% left.
	bool canAccel =
			(towardsOrAway <= 0.0f and energyProportion > 0.1f) or
			(towardsOrAway > 0.0f and energyProportion > 0.25f);

	//If we are angled towards the m_target...
	float tvMul = dot(m_tVel, v);
	if( ( tvMul < 0.8f or tvMul > 1.2f or dist > radius )
			and ( canAccel /*or m_curGoal == GOAL_CONGREGATE*/ )
			and getCanMove()
            and (getType() != SHIP_TYPE_CAPITAL)
			)
	{
        //Bug where capital ships do this
		accelerate(utv, accelMul);
	}

	if(m_curGoal == GOAL_TURRET) dist -= 200.0f;

	float selfRadius = getRadius();
	if(getParent().m_id != -1) selfRadius += 2048.0f;

	if(getCanShoot()
		 and fabs(shortestAngle(getAng(),getTAng())) <= 2.0f
		 and dist < 800.0f + radius + selfRadius
		 and ( m_curGoal == GOAL_ATTACK or m_curGoal == GOAL_TURRET )
		 and getEnergy() / getMaxEnergy() > 0.05f)
	{
		setFiring(true);

		//Combat moves
		if(getType() == SHIP_TYPE_FIGHTER)
		{
			//Dodge
			//1.0f + getCooldown = more likely to dodge just after shooting.
			int odds = static_cast<int>(64 * (1.0f + getCooldown()));
			if(prob(odds) and
				 t != nullptr and
				 dot(vec(getAng()), vec(t->getAng())) < -0.8f
				 )
			{
				dodge(randNum(10.0f, 20.0f) * randNum(-1,1));
			}

			//Charge
			if(prob(odds * 2) and
				 t != nullptr and
				 t->getType() == SHIP_TYPE_FIGHTER
				 )
			{
				accelerate(
							unit(m_tPos - getPos()),
							randNum(15.0f, 25.0f)
							);
			}
		}
	}
	else if(m_curGoal == GOAL_TRADE
					and dist < radius)
	{

	}

	//This variable represents the ships' direction versus its ideal direction.
	float vecMulSide = dot(tovec2(uv), vec(getTAng()));

	if(fabs(vecMulSide) > 0.8f)
	{
		//closing speed * how sideways it is flying * its angle relative to velocity
		float dv = clamp(cSpd * vecMulSide * dot(tovec2(uv), vec(getAng())), 0.0f, 1.0f);
		if(vecMulSide < 0) dodge( dv );
		else if(vecMulSide > 0) dodge( -dv );
	}
}

void enemy::damage(const float _d)
{
	ship::damage(_d);
	decrConfidence(_d);
}

float enemy::damage(const float _d, const vec3 _v, aiTarget _attacker)
{
	m_lastAttacker = _attacker;
	float f = ship::damage(_d, _v);
	decrConfidence(f);
	return f;
}
