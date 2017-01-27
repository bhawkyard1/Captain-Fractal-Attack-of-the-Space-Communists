#include "common.hpp"
#include "file.hpp"
#include "sfx.hpp"
#include "shapes.hpp"
#include "sim_time.hpp"
#include "universe.hpp"
#include "util.hpp"

universe::universe()
	:
		m_drawer(),
		//loadShips(),
		m_ply( vec3() )
{
	createFactions();

	m_sounds.sfxInit();
	m_sounds.loadSounds();

	showUI = true;
	m_time_elapsed = 0.0;
	m_pos = vec3();
	setVel(vec3());

	m_maxMiners = 0;
	m_maxWingmen = 0;
	m_minerCount = 0;
	m_wingmenCount = 0;

	m_ply.setPos({g_WIN_WIDTH/2.0f,g_WIN_HEIGHT/2.0f,0.0f});
	m_ply.setPPos({g_WIN_WIDTH/2.0f,g_WIN_HEIGHT/2.0f,0.0f});

	float p0 = randNum(0.0f, 1.0f);
	float p1 = randNum(0.0f, 1.0f);
	float p2 = randNum(0.0f, 1.0f);
	float total = p0 + p1 + p2;

	m_tCol[0] = ( p0 / total ) * 250.0f;
	m_tCol[1] = ( p1 / total ) * 250.0f;
	m_tCol[2] = ( p2 / total ) * 250.0f;

	m_cCol[0] = 0.0f;
	m_cCol[1] = 0.0f;
	m_cCol[2] = 0.0f;

	initUI();
	if(g_DEV_MODE) setScore(100000);
	else setScore(16);

	m_paused = false;
	m_mouse_state = -1;

	m_escMenuShown = false;

	if(g_BEAST_MODE) m_enemySpawnRate = 8;
	else m_enemySpawnRate = 36000;

	m_balanceOfPower.assign(m_factions.size(), 0.0f);

	m_mapExpanded = false;

	m_contextShip = {0, -1};

	std::array<int, 4> btncol = {{20, 200, 255, 220}};
	std::array<int, 4> quitcol = {{255, 5, 30, 220}};
	std::array<int, 4> white = {{255, 255, 255, 255}};
	button resume ("RESUME", btncol, white, vec2(g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 300.0f), vec2(200.0f, 80.0f));
	button sGame ("SAVE GAME", btncol, white, vec2(g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 200.0f), vec2(200.0f, 80.0f));
	button lGame ("LOAD GAME", btncol, white, vec2(g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y - 100.0f), vec2(200.0f, 80.0f));
	button mm ("MAIN MENU", quitcol, white, vec2(g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y), vec2(200.0f, 80.0f));
	button quit ("QUIT", quitcol, white, vec2(g_HALFWIN.m_x - 100.0f, g_HALFWIN.m_y + 100.0f), vec2(200.0f, 80.0f));

	m_escMenuSelection.add(resume);
	m_escMenuSelection.add(sGame);
	m_escMenuSelection.add(lGame);
	m_escMenuSelection.add(mm);
	m_escMenuSelection.add(quit);
}

void universe::addShot(
		const vec3 _p,
		const vec3 _v,
		const float _angle,
		std::array<float, WEAPS_W> _weap,
		const aiTeam _team,
		slot _owner,
		const float _xpModifier
		)
{
	int temp_angle = _angle + 90;
	float dmgMul = 1.0f + (_xpModifier / 48.0f);
	_weap[DAMAGE] *= dmgMul;
	for(int i = 0; i < _weap[0]; ++i)
	{
		//vec2 vec = vec(temp_angle);
		laser temp( _p, _v, temp_angle, _weap, _team, _owner);
		m_shots.push_back(temp);
	}
}

void universe::addDebris(
		const vec3 _pos,
		const vec3 _vel,
		resourceType _type
		)
{
	debris temp (_pos, _vel, m_vel, _type);
	m_resources.push_back(temp);
}

void universe::addMissile(
		const vec3 _p,
		const vec3 _v,
		const float _angle,
		const aiTeam _team
		)
{
	missile m(_p);
	m.setVel(_v + tovec3(vec(_angle + 90)) * 5);
	m.setAng(_angle);

	m.setTarget( closestEnemy( tovec3( toWorldSpace( getMousePos() ) ), _team ) );

	m_missiles.push_back(m);
}

void universe::update(float _dt)
{
	if(g_PLAYER_MOVEMENT_LOCKED) m_ply.setVel(vec3());
	if(m_paused) _dt = 0.0f;

	g_VELOCITY_TIME_SCALE = 1.0f - (clamp(mag(m_vel) * 80.0f, 0.0f, static_cast<float>(LIGHTSPEED)) / LIGHTSPEED);
	g_KILL_TIME_SCALE = clamp(g_KILL_TIME_SCALE + _dt * 0.25f, 0.0f, 1.0f);
	g_TIME_SCALE = g_PLAYER_TIME_SCALE * g_VELOCITY_TIME_SCALE * g_KILL_TIME_SCALE;

	debug("updates start");

	//Rollovers and stuff.
	m_ui.update(m_factions[TEAM_PLAYER].getWealth(), getMousePos());
	//Clicking and buttons.
	processInputMap();

	m_ply.updatePos(_dt);
	m_ply.ctrlUpdate();
	m_ply.update(_dt);

	//If m_paused, we do not update the game.
	if(m_paused) return;

	calcPowerBalance();

	base * focus = nullptr;
	if(m_drawer.getFocus().m_id == -1)
	{
		focus = &m_ply;
		m_vel = -m_ply.getVel();
	}
	else
	{
		focus = m_agents.getByID( m_drawer.getFocus() );
		m_vel = -focus->getVel();
	}
	m_drawer.update(_dt, focus);

	m_pos += m_vel * g_PIXEL_UNIT_CONVERSION * _dt;

	m_time_elapsed += _dt;
	//Interpolate towards desired background colour.
	m_cCol[0] += clamp(m_tCol[0] - m_cCol[0], -0.01f, 0.01f);
	m_cCol[1] += clamp(m_tCol[1] - m_cCol[1], -0.01f, 0.01f);
	m_cCol[2] += clamp(m_tCol[2] - m_cCol[2], -0.01f, 0.01f);

	//Randomly change background colour.
	if(prob(1024))
	{
		float p0 = randNum(0.0f, 1.0f);
		float p1 = randNum(0.0f, 1.0f);
		float p2 = randNum(0.0f, 1.0f);
		float total = p0 + p1 + p2;

		m_tCol[0] = ( p0 / total ) * 250.0f;
		m_tCol[1] = ( p1 / total ) * 250.0f;
		m_tCol[2] = ( p2 / total ) * 250.0f;
	}

	if(prob(10000)) g_BG_DENSITY = randNum(1.0f,10.0f);
	if(prob(10000)) m_gameplay_intensity = randNum(0.0f, 2.2f);

	//If player health is below 25%, emit smoke.
	if(m_ply.getHealth() < m_ply.getMaxHealth() / 2.0f and m_ply.getHealth() > 0.0f) addParticleSprite(m_ply.getPos(), m_ply.getVel(), m_ply.getRadius() * 2.0f, "SMOKE");

	if(m_ply.isFiring() and m_ply.getCooldown() <= 0.0f and m_ply.getEnergy() > m_ply.getCurWeapStat( ENERGY_COST ) and !g_GAME_OVER)
	{
		m_sounds.playUISnd( static_cast<sound>(m_ply.getCurWeap()) );
		m_ply.shoot();
		m_ply.setFiring(false);
		addShot( m_ply.getPos(), m_ply.getVel(), m_ply.getAng(), m_ply.getWeap(), TEAM_PLAYER, {0, -2}, m_ply.getXP() );
		m_ply.setEnergy( m_ply.getEnergy() - m_ply.getCurWeapStat(ENERGY_COST) );
		m_ply.setCooldown( m_ply.getCurWeapStat(COOLDOWN) );
		m_drawer.addShake(m_ply.getCurWeapStat(STOPPING_POWER) * 1000.0f);
	}

	//Explode player if dead.
	if(!g_DEV_MODE and m_ply.getHealth() <= 0.0f and !g_GAME_OVER)
	{
		for(int p = 0; p < rand()%5 + 10; ++p)
		{
			vec3 pos = {randNum(-16.0f,16.0f), randNum(-16.0f,16.0f), 0.0f};
			pos += m_ply.getPos();
			addpfx(pos, m_ply.getVel(), rand()%50 + 20, rand()%50 + 8, {{255, 200, 20, 255}});
		}
		m_ply.setMaxHealth(0, true);
		m_ply.setMaxEnergy(0, true);
		m_ply.setMaxShield(0, true);
		playUISnd(EXPLOSION_SND);

		m_ply.setPos({F_INF, F_INF, F_INF});
		m_drawer.addShake(20.0f);

		m_ui.clear();

		g_GAME_OVER = true;
	}

	for(int i = static_cast<int>(m_shots.size()) - 1; i >= 0; i--)
	{
		if(m_shots[i].getPower() < 0.0f)
		{
			swapnpop(&m_shots, i);
		}
		else
		{
			m_shots[i].setWVel(m_vel);
			m_shots[i].update(_dt);
			vec3 pos = randVec3OnLine(m_shots[i].getPos(), m_shots[i].getPos() + m_shots[i].getVel());
			if(!(rand() % 32)) addpfx(pos, m_shots[i].getVel(), randNum(1.0f, 2.0f), 1.0f, m_shots[i].getCol());
		}
	}

	m_partitions.clear();

	std::vector<enemy*> init_ship;
	for(auto &i : m_agents.m_objects) init_ship.push_back(&i);
	std::vector<laser*> init_laser;
	for(auto &i : m_shots) init_laser.push_back(&i);
	std::vector<missile*> init_missile;
	for(auto &i : m_missiles) init_missile.push_back(&i);
	std::vector<ship*> init_asteroid;
	for(auto &i : m_asteroids.m_objects) init_asteroid.push_back(&i);
	std::vector<debris*> init_resources;
	for(auto &i : m_resources) init_resources.push_back(&i);

	std::vector<SDL_Rect> testRects;
	if(m_agents.size() > 0) testRects.push_back(enclose(m_agents.m_objects));
	if(m_asteroids.size() > 0) testRects.push_back(enclose(m_asteroids.m_objects));
	if(m_shots.size() > 0) testRects.push_back(enclose(m_shots));
	if(m_resources.size() > 0) testRects.push_back(enclose(m_resources));
	testRects.push_back( {static_cast<int>(m_ply.getPos().m_x), static_cast<int>(m_ply.getPos().m_y), 0, 0} );

	SDL_Rect ir = maxRect(testRects);

	debug("detCol");
	detectCollisions(ir, init_ship, init_laser, init_missile, init_asteroid, init_resources, 0);
	debug("checkCol");
	checkCollisions();

	for(int i = static_cast<int>(m_missiles.size()) - 1; i >= 0; i--)
	{
		m_missiles[i].updatePos(_dt);
		bool ofscr = isOffScreen(m_missiles[i].getPos(), 80000.0f);
		if(ofscr or m_missiles[i].getHealth() <= 0 or m_missiles[i].detonate())
		{
			if(!ofscr)
			{
				for(int p = 0; p < rand()%2 + 4; p++)
				{
					vec3 pos = {randNum(-16.0f,16.0f), randNum(-16.0f,16.0f), 0.0f};
					pos += m_missiles[i].getPos();
					addpfx(pos, m_missiles[i].getVel(), rand()%50 + 50, rand()%50 + 8, {{255, 200, 20, 255}});
				}

				for(size_t j = 0; j < m_agents.size(); j++)
				{
					vec3 ep = m_agents[j].getPos();
					vec3 mp = m_missiles[i].getPos();

					float dmg = magns(mp-ep);

					if(dmg > 300*300) continue;

					dmg = 1/mag(mp - ep) * 30000;
					m_agents[j].damage(dmg);
					addDamagePopup(dmg, m_agents[j].getTeam(), ep, m_agents[i].getVel() + randVec3(2.0f));
				}

				vec3 pdiff = m_missiles[i].getPos() - m_ply.getPos();
				if(magns(pdiff) < 300*300)
				{
					float dmg = invMag(pdiff) * 30000;
					m_ply.damage(dmg);
					addDamagePopup(dmg, TEAM_PLAYER, m_ply.getPos(), randVec3(2.0f));
				}
			}
			swapnpop(&m_missiles, i);
		}
		else
		{
			if(m_missiles[i].getTarget() == nullptr) m_missiles[i].setTarget(closestEnemy(m_missiles[i].getPos(), m_missiles[i].getTeam()));
			m_missiles[i].setWVel(m_vel);
			m_missiles[i].update(_dt);
			m_missiles[i].steering();
		}
	}

	for(int i = static_cast<int>(m_asteroids.size()) - 1; i >= 0; i--)
	{
		m_asteroids[i].updatePos(_dt);
		vec3 p = m_asteroids[i].getPos();
		vec3 v = m_asteroids[i].getVel();

		if((isOffScreen(p,60000.0f) or m_asteroids[i].getHealth() <= 0.0f))
		{
			if(m_asteroids[i].getHealth() <= 0.0f)
			{
				std::cout << "DESTROYING ASTEROID! CURRENT SPEC " << m_asteroids[i].getClassification() << ' ' << ASTEROID_SMALL << ' ' << ASTEROID_MID << ' ' << ASTEROID_LARGE << '\n';
				for(int fx = 0; fx < rand() % 5 + 1; fx++)
				{
					vec3 pos = randVec3(16.0f);
					pos += m_asteroids[i].getPos();
					addpfx(pos, m_asteroids[i].getVel(), rand()%20 + 50, rand()%30 + 2, {{2, 2, 2, 0}});
					for(int q = 0; q < 50; ++q) addParticleSprite(pos, m_asteroids[i].getVel() + m_vel + tovec3(randVec2(1.0f)), m_asteroids[i].getRadius() * 2.0f, "SMOKE");
				}

				if(m_asteroids[i].getClassification() == ASTEROID_SMALL)
				{
					std::cout << "DISINTERGRATING!\n";
					for(int i = 0; i < rand() % 3 + 3; ++i)
					{
						addDebris(p, v + tovec3(randVec2(1.0f)), RESOURCE_IRON);
					}
					addScore( 10 );
				}
				else
				{
					std::cout << "CONVERTING TO 4 x " << (m_asteroids[i].getClassification() - 1) << '\n';
					for(int q = 0; q < 4; ++q)
					{
						ship a(g_ship_templates[ m_asteroids[i].getClassification() - 1 ], p + tovec3(randVec2( m_asteroids[i].getRadius()) ));
						a.setVel( m_asteroids[i].getVel() + tovec3(randVec2(1.0f)) );
						a.setAng(randNum(-180.0f, 180.0f));
						a.update(_dt);
						m_asteroids.push_back(a);
					}
				}
				playSnd(EXPLOSION_SND, m_asteroids[i].getPos(), 1.0f);
				m_drawer.addShake(12000.0f / (1.0f + mag(m_asteroids[i].getPos() - m_ply.getPos())));
			}
			m_asteroids.free( i );
		}
		else
		{
			m_asteroids[i].setWVel(m_vel);
			m_asteroids[i].update(_dt);
		}
	}

	for(int i = static_cast<int>(m_resources.size()) - 1; i >= 0; --i)
	{
		if((isOffScreen(m_resources[i].getPos(),60000.0f)))
		{
			swapnpop(&m_resources, i);
		}
		else
		{
			m_resources[i].setWVel(m_vel);
			m_resources[i].updatePos(_dt);
		}
	}

	for(auto &i : m_selectedItems)
	{
		vec2 pos = toWorldSpace( getMousePos() );

		i.setPos( tovec3(pos) );

		//std::cout << "setPos : " << i.getPos().m_x << ", " << i.getPos().m_y << '\n';
	}

	debug("mid");

	//Cull dead m_agents. Important, this is the ONLY place agents are destroyed.
	cullAgents();

	calcSquadPositions();

	debug("updates");
	//Update live m_agents.
	for(size_t e = 0; e < m_agents.size(); ++e)
	{
		if(!rand()) playUISnd(RADIO_CHATTER_SND);

		debug("parenting");
		if(!m_agents[e].hasParent())
		{
			m_agents[e].updatePos(_dt);
		}
		else if(m_agents[e].hasParent())
		{
			slotID<ship> parentID = m_agents[e].getParent();
			ship * parent = m_agents.getByID( slot(parentID.m_id, parentID.m_version) );

			if(parent == nullptr)
			{
				m_agents[e].setHealth(-1.0f);
				continue;
			}

			float angle = parent->getAng();
			float s = sin(rad(angle));
			float c = cos(rad(angle));

			vec3 epos = m_agents[e].getParentOffset();
			float xn = epos.m_x * c - epos.m_y * s;
			float yn = epos.m_x * s + epos.m_y * c;
			vec3 comb = {xn, yn, 0.0f};

			m_agents[e].setPos( parent->getPos() + comb );
			m_agents[e].setPPos( m_agents[e].getPos() );

			m_agents[e].setVel(parent->getVel());
		}
		vec3 p = m_agents[e].getPos();

		//If the agent is damaged, add smoke.
		if(m_agents[e].getHealth() < m_agents[e].getMaxHealth())
			addParticleSprite(p, m_agents[e].getVel(), m_agents[e].getRadius() * 4.0f, "SMOKE");

		debug("special actions");
		//The stuff here is ships affecting other stuff easily accessible within the universe class, but not so easily dealt with inside enemy.cpp
		if(
			 m_agents[e].getType() == SHIP_TYPE_GRAVWELL
			 ) //Gravwell attraction
		{
			//Attract m_asteroids based on distancm_agents[e].
			for(auto &k : m_asteroids.m_objects)
			{
				vec3 incr = p - k.getPos();
				float dist = magns( incr );
				dist /= sqr( 32.0f );

				if(dist > 10000.0f) k.addForce( incr / std::max(dist, 0.01f) );
				else k.addForce( -k.getVel() * 0.0125f );
			}
		}
		else if(m_agents[e].getType() == SHIP_TYPE_BARRACKS)
		{
			//Spawn wingman.
			if(rand() % 2048 == 0 and m_wingmenCount < 20)
				spawnShip(getRandomShipType(m_agents[e].getTeam()), m_agents[e].getTeam(), p);
		}

		if(m_agents[e].getGoal() == GOAL_TRADE
			 and magns(p - m_agents[e].getTarget()->getPos()) < sqr(m_agents[e].getRadius() + m_agents[e].getTarget()->getRadius()))
		{
			//What ugly, ugly code.
			//The target held by an agent is always a ship pointer (to account for the fact that the player, asteroids, or other agents (all derived from the ship class) can be targeted.
			//This conduct trade function takes two const enemy references though, so I must first convert the target to an enemy pointer before dereferencing.
			//The major caveat here is we must be able to guarantee that the trade target is only another agent. If it is the player or an asteroid, we are probably looking at a crash.
			conductTrade(*static_cast<enemy *>(m_agents[e].getTarget()), m_agents[e]);
		}

		debug("fleeing");
		if(!sameTeam( m_agents[e].getTeam(), TEAM_PLAYER ) and m_agents[e].getHealth() < m_agents[e].getConfidence() and m_agents[e].getCanMove() and m_agents[e].getSquadID().m_id != 0 and m_agents[e].getSquadID().m_version != -1 )
		{
			//If the enemy can move and is scared, runs away.
			removeFromSquad(&m_agents[e]);
			m_agents[e].setGoal(GOAL_FLEE);
		}

		/*if(m_agents[e].getGoal() == GOAL_FLEE and m_agents[e].getHealth() > m_agents[e].getConfidence())
		{
			addToSquad( &m_agents[e], m_factions[m_agents[e].getTeam()].getBackSquad() );
			m_agents[e].setGoal( GOAL_IDLE );
		}*/

		debug("shooting");
		if(m_agents[e].isFiring() and m_agents[e].getCooldown() <= 0)
		{
			//If the agent is shooting, add lasers.
			m_agents[e].shoot();
			addShot(m_agents[e].getPos() - m_agents[e].getVel(), m_agents[e].getVel(), m_agents[e].getAng(), m_agents[e].getWeap(), m_agents[e].getTeam(), m_agents.getID(e), m_agents[e].getXP());
			m_agents[e].setCooldown( ( m_agents[e].getCurWeapStat( COOLDOWN ) ) );
			m_agents[e].setFiring(false);
		}

		//Updating--------------------------------------------------------------------------------//
		m_agents[e].targetAcquisition(m_ply, m_agents, m_asteroids, m_resources, m_factions);
		m_agents[e].update(_dt);
		m_agents[e].setWVel(m_vel);
		m_agents[e].behvrUpdate(_dt);
		m_agents[e].steering();
	}
	debug("ships end");
	//Ship spawning functions.
	for(int i = static_cast<int>(m_particles.size()) - 1; i >= 0; i--)
	{
		m_particles[i].setWVel(m_vel);
		m_particles[i].update(_dt);
		if(m_particles[i].done()) swapnpop(&m_particles, i);

	}

	for(int i = static_cast<int>(m_passiveSprites.size()) - 1; i >= 0; --i)
	{
		float alph = m_passiveSprites[i].getAlph();

		m_passiveSprites[i].updateSprite(_dt);

		/*int w = 32;
				int h = 32;*/

		if(alph <= 0.1f or m_passiveSprites[i].getDim() <= 0.0f)
		{
			//std::cout <<"popit" << std::endl;
			swapnpop(&m_passiveSprites, i);
			continue;
		}
		alph *= 0.9f;
		alph -= 0.02f;
		m_passiveSprites[i].setAlph(alph);
		m_passiveSprites[i].incrDim(_dt);
		m_passiveSprites[i].setWVel(m_vel);
	}

	for(int i = static_cast<int>(m_popups.size()) - 1; i >= 0; --i)
	{
		if(m_popups[i].getCol(3) <= 0.0f)
		{
			swapnpop(&m_popups, i);
			continue;
		}
		m_popups[i].setWVel(m_vel);
		m_popups[i].update(_dt);
	}
	debug("p2");
	if(rand() % 256 == 0 and m_wingmenCount < m_maxWingmen)
	{
		spawnShip(TEAM_PLAYER);
	}
	if(rand() % 256 == 0 and m_minerCount < m_maxMiners)
	{
		spawnShip(PLAYER_MINER_DROID, TEAM_PLAYER, tovec3(randVec2(10000.0f, 20000.0f)) + getCameraPosition());
	}

	if(g_DIFFICULTY == 0) return;

	debug("factions");
	//std::cout << "Pre facs\n";
	for(auto &i : m_factions)
	{
		//std::cout << i.getIdentifier() << ", " << i.getWealth() << '\n';
		i.setWVel(m_vel);

		debug("economy");
		i.updateEconomy(_dt);
		debug("deployment");
		i.updateDeployment(m_factions);
		debug("tactics");
		i.updateTactics(_dt, m_factions, m_agents.m_objects);

		spawnSquad(i.getTeam(), 20000.0f, 60000.0f, i.getDeployed());
		i.clearDeployed();
	}
	//std::cout << "Post facs\n";

	/*if(rand() % m_enemySpawnRate <= g_DIFFICULTY * m_gameplay_intensity and m_factionCounts[GALACTIC_FEDERATION] < clamp(m_factionMaxCounts[GALACTIC_FEDERATION],0,100))
		{
				int reps = clamp(rand() % (g_DIFFICULTY * 5) + 1, 1, clamp(m_factionMaxCounts[GALACTIC_FEDERATION],0,80) - m_factionCounts[GALACTIC_FEDERATION]);
				aiTeam pteam;
				if(rand() % 100 < 50) pteam = SPOOKY_SPACE_PIRATES;
				else pteam = GALACTIC_FEDERATION;
				spawnSquad(pteam, 10000.0f, 20000.0f, reps);
		}

		if(rand() % m_enemySpawnRate <= g_DIFFICULTY * m_gameplay_intensity and m_factionCounts[SPACE_COMMUNISTS] < clamp(m_factionMaxCounts[SPACE_COMMUNISTS],0,100))
		{
				int reps = clamp(rand() % (g_DIFFICULTY * 20) + 1, 1, clamp(m_factionMaxCounts[SPACE_COMMUNISTS],0,80) - m_factionCounts[SPACE_COMMUNISTS]);
				aiTeam pteam;
				pteam = SPACE_COMMUNISTS;
				spawnSquad(pteam, 10000.0f, 20000.0f, reps);
		}

		if(rand() % m_enemySpawnRate <= g_DIFFICULTY * m_gameplay_intensity and m_factionCounts[ALLIANCE] < clamp(m_factionMaxCounts[ALLIANCE],0,100))
		{
				int reps = clamp(rand() % (g_DIFFICULTY * 20) + 1, 1, clamp(m_factionMaxCounts[ALLIANCE],0,80) - m_factionCounts[ALLIANCE]);
				aiTeam pteam;
				pteam = ALLIANCE;
				spawnSquad(pteam, 10000.0f, 20000.0f, reps);
		}*/

	if(rand() % 1500 == 0 and m_asteroids.size() < 10)
	{
		ship_spec size = ASTEROID_MID;
		int prob = rand() % 100;

		if(prob > 50 and prob <= 80) size = ASTEROID_SMALL;
		else if(prob > 80 and prob <= 99) size = ASTEROID_LARGE;

		vec3 ppos;
		int side = rand() %4 ;
		if(side == 0) ppos = {randNum(-20000.0f,20000.0f), -20000.0f, 0.0f};
		else if(side == 1) ppos = {randNum(-20000.0f,20000.0f), 20000.0f, 0.0f};
		else if(side == 2) ppos = {-20000.0f, randNum(-20000.0f,20000.0f), 0.0f};
		else if(side == 3) ppos = {20000.0f, randNum(-20000.0f,20000.0f), 0.0f};
		ship a( g_ship_templates[size] );
		a.setVel( tovec3(randVec2(64.0f)) );
		a.setPos( ppos );
		a.setPPos( ppos );
		a.update(_dt);
		m_asteroids.push_back(a);
	}
	debug("updates end");
	//if(rand() == 0) m_factionMaxCounts[GALACTIC_FEDERATION] = clamp(m_factionMaxCounts[GALACTIC_FEDERATION] - 1, 0, I_MAX);
	//if(rand() == 0) m_factionMaxCounts[SPACE_COMMUNISTS] = clamp(m_factionMaxCounts[SPACE_COMMUNISTS] - 1, 0, I_MAX);
}

void universe::cullAgents()
{
	for(int i = static_cast<int>(m_agents.size()) - 1; i >= 0; i--)
	{
		vec3 p = m_agents[i].getPos();

		bool isOffscreen = isOffScreen(p - getCameraPosition(), g_AGENTS_ACTIVE_RANGE) and m_agents[i].getGoal() == GOAL_FLEE;
		bool isDead = m_agents[i].getHealth() <= 0.0f;
		bool isPlayerOwned = sameTeam(m_agents[i].getTeam(), TEAM_PLAYER);
		bool isSmall = m_agents[i].getType() == SHIP_TYPE_FIGHTER;
		//Offscreen elimination, health-based elimination
		if( isDead or ( isOffscreen and (!isPlayerOwned or isSmall) ) )
		{
			removeFromSquad(&m_agents[i]);

			if(isDead)
			{
				destroyAgent(i);
			}
			else if(isOffscreen)
			{
				//destroyAgent(i);
				m_factions[ m_agents[i].getTeam() ].unitWithdrawn(m_agents[i].getClassification());
				if(m_agents.getByID(m_contextShip) == &m_agents[i]) m_contextShip = {0, -1};
				m_agents.free(i);
			}
		}
	}
}

void universe::calcSquadPositions()
{
	//Reset squad variables
	for(auto &f : m_factions) f.resetSquads();

	//Set average positions and velocities.
	for(auto &e : m_agents.m_objects)
	{
		squad * sq = getSquadFromID(e.getTeam(), e.getSquadID());
		if(sq != nullptr)
		{
			//Get the distance to the average position last f rame.
			float weight = clamp(mag(e.getPos() - sq->m_pAveragePos) /  20000.0f, 1.0f, 100.0f);
			vec3 averageDiff = e.getPos() - sq->m_pAveragePos;

			sq->m_averagePos += (averageDiff / sq->m_size) / weight;
			sq->m_averageVel += e.getVel() / sq->m_size;
		}
	}

	/*for(auto &f : m_factions)
				f.finaliseSquadAveragePos();*/

	//Set average distance.
	for(auto &e : m_agents.m_objects)
	{
		squad * sq = getSquadFromID(e.getTeam(), e.getSquadID());
		if(sq != nullptr)
		{
			sq->m_averageDistance += magns(e.getPos() - sq->m_targetPos) / sq->m_size;
		}
	}
}

void universe::processInputMap()
{
	if(m_inputs.key(SDLK_w))
	{
		m_ply.accelerate(1.0f);
		addShake(0.5f);
	}
	else if(m_inputs.key(SDLK_s))
	{
		if(m_inputs.keyEvent(SDLK_s) == INPUT_EVENT_PRESS and m_inputs.key(SDLK_LCTRL))
		{
			saveGame(this);
			//m_inputs.deactivate(SDLK_s);
		}
		else
		{
			m_ply.accelerate(-1.0f);
			addShake(0.5f);
		}
	}

	if(m_inputs.key(SDLK_a))
	{
		m_ply.dodge(1.0f);
		addShake(5.5f);
	}
	else if(m_inputs.key(SDLK_d))
	{
		m_ply.dodge(-1.0f);
		addShake(5.5f);
	}

	if(m_inputs.keyEvent(SDLK_ESCAPE) == INPUT_EVENT_PRESS)
	{
		setPause(true);
		escMenuTog();
		if(isEscMenuShown())
		{
			m_ui.add(m_escMenuSelection);
		}
		else
		{
			setPause(false);
			m_ui.pop();
		}
		//m_inputs.deactivate(SDLK_ESCAPE);
	}

	if(m_inputs.key(SDLK_EQUALS) or m_inputs.scroll() < 0)
	{
		m_ply.incrWeap(1);
		m_inputs.deactivate(SDLK_EQUALS);
		m_inputs.clearScroll();
	}
	else if(m_inputs.key(SDLK_MINUS) or m_inputs.scroll() > 0)
	{
		m_ply.incrWeap(-1);
		m_inputs.deactivate(SDLK_MINUS);
		m_inputs.clearScroll();
	}

	if(m_inputs.key(SDLK_p))
	{
		g_TARG_ZOOM_LEVEL = clamp(g_TARG_ZOOM_LEVEL + 0.02f, 0.1f, 2.0f);
	}
	else if(m_inputs.key(SDLK_o))
	{
		g_TARG_ZOOM_LEVEL = clamp(g_TARG_ZOOM_LEVEL - 0.02f, 0.1f, 2.0f);
	}

	if(m_inputs.key(SDLK_g))
	{
		if(g_DEV_MODE)
			addScore(1000);
	}

	if(m_inputs.keyEvent(SDLK_n) == INPUT_EVENT_PRESS and m_inputs.key(SDLK_LCTRL))
	{
		reload(true);
		//m_inputs.deactivate(SDLK_n);
	}

	if(m_inputs.keyEvent(SDLK_l) == INPUT_EVENT_PRESS and m_inputs.key(SDLK_LCTRL))
	{
		reload(false);
		loadGame(this);
		//m_inputs.deactivate(SDLK_l);
	}

	if(m_inputs.keyEvent(SDLK_m) == INPUT_EVENT_PRESS)
	{
		toggleMapMode();
		//m_inputs.deactivate(SDLK_m);
	}

	if(m_inputs.keyEvent(SDLK_SPACE) == INPUT_EVENT_PRESS)
	{
		pause();
		//m_inputs.deactivate(SDLK_SPACE);
	}

	if(m_inputs.key(SDLK_LEFTBRACKET))
		g_PLAYER_TIME_SCALE = clamp(g_PLAYER_TIME_SCALE - 0.05f, 0.0f, 2.0f);
	else if(m_inputs.key(SDLK_RIGHTBRACKET))
		g_PLAYER_TIME_SCALE = clamp(g_PLAYER_TIME_SCALE + 0.05f, 0.0f, 2.0f);

	if(m_inputs.keyEvent(SDLK_i) == INPUT_EVENT_PRESS)
	{
		m_ply.toggleInventory();
		//m_inputs.deactivate(SDLK_i);
	}

	if(m_inputs.keyEvent(SDLK_u) == INPUT_EVENT_PRESS)
	{
		toggleUIVisible();
		//m_inputs.deactivate(SDLK_u);
	}

	if(m_inputs.key(SDLK_e))
		m_ply.decelerate();

	if(m_inputs.key(SDLK_q))
	{
		addShake(25.5f);
		m_ply.accelerate(25.0f);
	}

	if(m_inputs.keyEvent(SDLK_z) == INPUT_EVENT_PRESS)
	{
		debris temp({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, RESOURCE_IRON);
		m_ply.addItem(temp);
		//m_inputs.deactivate(SDLK_z);
	}

	if(m_inputs.keyEvent(SDLK_k) == INPUT_EVENT_PRESS)
	{
		debug_lockPlayer();
		//m_inputs.deactivate(SDLK_k);
	}

	//If ctrl down (or esc menu shown), test against UI.
	if(m_inputs.key(SDLK_LCTRL) or m_escMenuShown)
	{
		if(m_inputs.lmb())
		{
			selectionReturn ret = handleMouseDownInput( getMousePos() );
			processUIInput(ret);
			m_inputs.deactivatelmb();
		}
	}
	else if(!m_inputs.key(SDLK_LCTRL))
	{
		//Make the player shoot if lmb + !ctrl
		m_ply.setFiring(m_inputs.lmb());

		if(m_inputs.rmb() and m_ply.getMissiles() > 0)
		{
			addMissile(m_ply.getPos(), m_ply.getVel(), m_ply.getAng(), TEAM_PLAYER);
			m_ply.incrMissiles(-1);
			m_inputs.deactivatermb();
		}
	}

	if(m_inputs.lmbEvent() == INPUT_EVENT_RELEASE or m_inputs.rmbEvent() == INPUT_EVENT_RELEASE)
	{
		if(m_inputs.lmbEvent() == INPUT_EVENT_RELEASE)
			m_ply.setFiring(false);

		selectionReturn ret = m_ui.handleInput( getMousePos() );
		if(ret.m_sel_val > 0) setMouseState(-1);

		vec3 pos = tovec3( toWorldSpace( getMousePos() ) );

		if(m_mouse_state > -1 and m_mouse_state != PLAYER_CAPITAL)
		{
			addBuild(pos, static_cast<ship_spec>(m_mouse_state), TEAM_PLAYER);
		}
		else if(m_mouse_state == PLAYER_CAPITAL)
		{
			spawnShip(PLAYER_CAPITAL, TEAM_PLAYER, pos);
		}
		setMouseState(-1);
		mouseUp();
	}
}

void universe::draw(float _dt)
{
	debug("draw start");

	m_drawer.disableDepthTesting();
	m_drawer.drawBackground(m_time_elapsed, tovec2(m_pos + m_drawer.getShake()), tovec2(m_vel), m_cCol);
	m_drawer.enableDepthTesting();

	m_drawer.drawingNonLitElements();
	m_drawer.activeColourAttachments({GL_COLOR_ATTACHMENT1});
	for(auto i = m_passiveSprites.begin(); i != m_passiveSprites.end(); ++i)
	{
		if(i->getIdentifier() != "SMOKE") continue;
		vec3 ipos = i->getInterpolatedPosition(_dt);
		vec2 idim = {i->getDim(), i->getDim()};
		std::array<float, 4> col = {{i->getCol(0), i->getCol(1), i->getCol(2), i->getAlph()}};
		col = col255to1(col);
		//col[3] *= 255.0f;
		m_drawer.addRect(ipos, idim, 0.0f, col);
	}
	//m_drawer.drawSmoke(m_time_elapsed);
	m_drawer.clearVectors();

	m_drawer.useShader("flame");

	float stat = (m_ply.getAlphaStats()[0] * m_ply.getEnginePower()) / 32.0f;
	vec3 backwards = tovec3(back(rad(m_ply.getAng())));
	vec3 pos = m_ply.getInterpolatedPosition(_dt) + backwards * m_ply.getRadius() + backwards * stat;

	if(stat > 0.05f and !g_GAME_OVER)
	{
		m_drawer.drawFlames(
					pos,
					vec2(m_ply.getRadius() * 0.85f, stat),
					m_ply.getAng(),
		{{0.1f, 0.4f, 1.0f, 1.0f}},
					m_time_elapsed,
					m_ply.getAlphaStats()[0] * m_ply.getEnginePower()
				);
	}

	debug("     first agent call");

	for(auto &i : m_agents.m_objects)
	{
		float stat = (i.getAlphaStats()[0] * i.getEnginePower()) / 32.0f;
		vec3 backwards = tovec3(back(rad(i.getAng())));
		vec3 pos = i.getInterpolatedPosition(_dt) + backwards * i.getRadius() + backwards * stat;
		std::array<float, 4> col = i.getCurWeapCol();
		col[3] = 1.0f;

		if(stat > 0.05f)
		{
			m_drawer.drawFlames(
						pos/* * clamp(stat * 0.055f, 1.0f, 20.0f)*/,
			{i.getRadius() * 0.85f, stat},
						i.getAng(),
						col,
						m_time_elapsed,
						i.getAlphaStats()[0] * i.getEnginePower()
					);
		}
	}
	debug("     done");
	for(auto &i : m_missiles)
	{
		float stat = (i.getAlphaStats()[0] * i.getEnginePower()) / 25.0f;

		if(stat > 0.05f)
		{
			m_drawer.drawFlames(
						i.getInterpolatedPosition(_dt) + tovec3(back(rad(i.getAng()))) * (i.getRadius() + stat),
						vec2(i.getRadius(), stat),
						i.getAng(),
			{{0.1f, 0.4f, 1.0f, 1.0f}},
						m_time_elapsed,
						i.getAlphaStats()[0] * i.getEnginePower()
					);
		}
	}

	m_drawer.drawingDeferredElements();
	//m_drawer.enableDepthTesting();
	m_drawer.useShader("ship");
	if(!g_GAME_OVER) m_drawer.drawAsset(m_ply.getInterpolatedPosition(_dt), m_ply.getAng(), m_ply.getIdentifier());
	for(auto &i : m_agents.m_objects)
	{
		m_drawer.drawAsset(i.getInterpolatedPosition(_dt), i.getAng(), i.getIdentifier());
	}
	for(auto &i : m_missiles)
	{
		m_drawer.drawAsset(i.getInterpolatedPosition(_dt), i.getAng(), i.getIdentifier());
	}
	for(auto &i : m_asteroids.m_objects)
	{
		m_drawer.drawAsset(i.getInterpolatedPosition(_dt), i.getAng(), i.getIdentifier());
	}
	for(auto &i : m_resources)
	{
		m_drawer.drawAsset(i.getInterpolatedPosition(_dt), i.getAng(), i.getIdentifier());
	}
	for(auto &i : m_selectedItems)
	{
		vec3 pos = tovec3( toWorldSpace( getMousePos() ) ) ;
		m_drawer.drawAsset(pos, i.getAng(), i.getIdentifier());
	}

	vec2 dpos = toWorldSpace( getMousePos() ) ;

	if(m_mouse_state != -1) m_drawer.drawAsset(dpos, 0.0f, g_ship_templates[m_mouse_state].getIdentifier(), 0.5f);

	m_drawer.drawingNonLitElements();
	m_drawer.activeColourAttachments({GL_COLOR_ATTACHMENT1});

	m_drawer.useShader("laser");
	for(auto &i : m_shots)
	{
		vec3 ipos = i.getInterpolatedPosition(_dt);
		vec3 ivel = (i.getVel() + i.getWVel()) * 8.0f;
		std::array<float, 4> icol = i.getCol();

		icol = col255to1(icol);
		icol[3] *= clamp(i.getPower() / 25.0f, 0.0f, 1.0f);

		m_drawer.addLine(ipos, ipos + ivel, 5.0f + i.getStop() * 40.0f, icol);
	}
	m_drawer.drawLasers(m_time_elapsed);

	m_drawer.clearVectors();
	for(auto &i : m_particles)
	{
		std::array<float, 4> col = i.getCol();
		col = col255to1(col);
		vec3 ipos = i.getInterpolatedPosition(_dt);
		float idim = i.getForce() * 20.0f;

		if(i.normalisedLifetime() < 1.0f)
		{
			m_drawer.addRect(ipos, {idim, idim}, 0.0f, col);
			m_drawer.packExtraData( i.getShaderData() );
		}
	}
	m_drawer.useShader("explosion");
	//m_drawer.drawExplosions( );
	m_drawer.clearVectors( );

	if(g_GRAPHICAL_DETAIL > 1)
	{
		for(auto &i : m_particles)
		{
			std::array<float, 4> col = i.getCol();
			col[3] = 1.0f;
			col = col255to1(col);
			int k = 0;
			for(auto j = i.getParticles()->begin(); j != i.getParticles()->end(); ++j)
			{
				vec3 jpos = j->getInterpolatedPosition(_dt);
				vec3 jvel = j->getVel() * 2.0f;
				col[3] = i.getAlpha(k) / 255.0f;

				m_drawer.addLine(jpos, jpos + (jvel + m_vel) * 1.0f, 4.0f * col[3], col);
				++k;
			}
		}
		m_drawer.useShader("sparks");
		m_drawer.drawLines(1.0f);
	}

	//Shields
	if(m_ply.getShieldGlow() > 0 and !g_GAME_OVER)
		m_drawer.drawShield(m_ply.getInterpolatedPosition(_dt), m_ply.getRadius(), m_time_elapsed / m_ply.getRadius(), {{0.1f, 0.4f, 1.0f, m_ply.getShieldGlow() / 255.0f}});
	for(auto &i : m_agents.m_objects)
	{
		if(i.getShieldGlow() > 0)
			m_drawer.drawShield(i.getInterpolatedPosition(_dt), i.getRadius(), m_time_elapsed / i.getRadius(), i.getShieldCol());
	}
	//for(auto &i : m_missiles)

	m_drawer.disableDepthTesting();

	m_drawer.drawingUI();

	debug("draw ui");
	if(showUI)
	{
		for(auto &i : m_popups)
		{
			m_drawer.drawText(i.getLabel(), "pix", tovec2(i.getPos()), true, i.getSize(), col255to1(i.getCol()));
		}
		drawUI(_dt);
		drawDebugUI();
	}
	debug("draw end");

	//Adding light.
	m_drawer.resetLights();
	size_t lightCount = 0;
	for(size_t i = 0; i < m_particles.size() and lightCount < MAX_LIGHTS; ++i)
	{
		vec3 pos = m_particles[i].getPos();
		if(isOffScreen(pos, g_WIN_WIDTH * 2.0f) or m_particles[i].getForce() < 5.0f) continue;
		std::array<float, 4> col = col255to1(m_particles[i].getCol());
		pointLight l (
					pos,
					vec3(col[0], col[1], col[2]),
				col[3]
				);
		m_drawer.addLight(l);
		lightCount++;
	}
	//From lasers
	for(size_t i = 0; i < m_shots.size() and lightCount < MAX_LIGHTS; ++i)
	{
		vec3 pos = m_shots[i].getPos();
		//pos.m_z = 1.0f;
		if(isOffScreen(pos, g_WIN_WIDTH * 2.0f)) continue;
		std::array<float, 4> col = m_shots[i].getCol();
		pointLight l (
					pos,
					vec3(col[0], col[1], col[2]),
				col[3]
				);
		m_drawer.addLight(l);
		lightCount++;
	}
}

void universe::drawUI(const float _dt)
{
	m_drawer.clearVectors();
	if(!g_GAME_OVER)
	{
		m_drawer.drawText("SCORE: " + std::to_string( m_factions[TEAM_PLAYER].getWealth() ),"pix",{280, 56}, false, 1.0f);
		m_drawer.drawText("MISSILES: " + std::to_string( m_ply.getMissiles() ),"pix",{280, 80}, false, 1.0f);
		m_drawer.drawText("SPEED: " + std::to_string(static_cast<int>(mag(m_vel))) + " m/s", "pix", {280, 104}, false, 1.0f);

		m_drawer.statusBars(&m_ply);
		m_drawer.drawWeaponStats(&m_ply);
		m_drawer.drawMap(&m_missiles, &m_agents.m_objects, &m_asteroids.m_objects, &m_shots, &m_factions, m_mapExpanded);

		m_drawer.clearVectors();
		float cumulative = 0.0f;
		int i = 0;
		for(auto &b : m_balanceOfPower)
		{
			cumulative += b * 128.0f;

			m_drawer.addRect(
			{g_WIN_WIDTH - 256.0f + cumulative, 272.0f, 0.0f},
			{b * 256.0f, 16.0f},
						0.0f,
						col255to1(m_factions[i].getColour())
						);

			cumulative += b * 128.0f;
			++i;
		}
		m_drawer.useShader("plain");
		m_drawer.drawRects(false);
		m_drawer.clearVectors();

		//DRAWING CONTEXT-SELECTED SHIP STATS
		selection * infoCard = &(*m_ui.getElements())[3];

		enemy * contextPtr = m_agents.getByID(m_contextShip);

		if(contextPtr != nullptr)
		{
			vec3 csp = contextPtr->getPos();
			//csp = tovec3(toWorldSpace(tovec2(csp)));

			//Clamp this position to the screen.
			vec2 offset = {256.0f, 256.0f};
			vec2 min = (-g_HALFWIN + offset) / g_ZOOM_LEVEL;
			vec2 max = (g_HALFWIN - offset) / g_ZOOM_LEVEL;
			csp = clamp(csp, tovec3(min) + getCameraPosition(), tovec3(max) + getCameraPosition());

			float health = 128.0f * contextPtr->getHealth() / contextPtr->getMaxHealth();
			float shield = 128.0f * contextPtr->getShield() /  contextPtr->getMaxShield();
			float energy = 128.0f * contextPtr->getEnergy() / contextPtr->getMaxEnergy();

			infoCard->setVisible(true);

			infoCard->getAt(0)->setLabel(contextPtr->getIdentifier());
			infoCard->getAt(1)->setLabel("KILLS: " + std::to_string(contextPtr->getKills()));
			infoCard->getAt(2)->setLabel("DISTANCE: " + std::to_string(static_cast<int>(mag(contextPtr->getPos()) /*/ g_PIXEL_UNIT_CONVERSION*/)) + " m");

			infoCard->setPos(tovec2(csp));
			for(auto &i : *(infoCard->getButtons()))
			{
				vec2 pos = i.getStart();
				pos += tovec2( csp );
				i.setPos( pos );
				//m_drawer.addRect( tovec3(pos), i.getDim(), 0.0f, {255,0,0,255} );
			}

			if(neutralityCheck(contextPtr->getTeam(), TEAM_PLAYER))
			{
				infoCard->getAt(3)->setDark(false);
			}
			else
			{
				infoCard->getAt(3)->setDark(true);
			}

			csp.m_x += 64.0f;
			csp.m_y += 32.0f;

			//Health base
			m_drawer.addRect(csp, vec2(128.0f, 16.0f), 0.0f, {{0.4f, 0.08f, 0.08f, 0.5f}});
			//Health
			m_drawer.addRect({csp.m_x - 64.0f + health / 2.0f, csp.m_y, 0.0f}, {health, 16.0f}, 0.0f, {{0.9f, 0.2f, 0.2f, 0.5f}});

			csp.m_y += 16.0f;
			//Shield base
			m_drawer.addRect(csp, vec2(128.0f, 16.0f), 0.0f, {{0.1f, 0.1f, 0.4f, 0.5f}});
			//Shield
			m_drawer.addRect({csp.m_x - 64.0f + shield / 2.0f, csp.m_y, 0.0f}, {shield, 16.0f}, 0.0f, {{0.2f, 0.2f, 0.9f, 0.5f}});

			csp.m_y += 16.0f;
			//Energy base
			m_drawer.addRect(csp, vec2(128.0f, 16.0f), 0.0f, {{0.08f, 0.4f, 0.08f, 0.5f}});
			//Energy
			m_drawer.addRect(vec3(csp.m_x - 64.0f + energy / 2.0f, csp.m_y, 0.0f), vec2(energy, 16.0f), 0.0f, {{0.2f, 0.9f, 0.2f, 0.5f}});

			m_drawer.drawRects(true);
			m_drawer.clearVectors();

			csp.m_y += 24.0f;
			m_drawer.addRect(csp, vec2(128.0f, 24.0f), 0.0f, {{1.0f, 0.0f, 0.0f, 1.0f}});
			m_drawer.drawXP( contextPtr->getXP() );
			m_drawer.drawRects(true);
			m_drawer.clearVectors();

			if(g_DEV_MODE)
			{
				m_drawer.useShader("plain");
				m_drawer.addLine(contextPtr->getPos(), contextPtr->getTPos(), 1.0f, {{1.0f, 1.0f, 1.0f, 1.0f}});

				squad * sq = getSquadFromID( contextPtr->getTeam(), contextPtr->getSquadID() );
				if(sq != nullptr)
				{
					m_drawer.addLine(contextPtr->getPos(), sq->m_averagePos, 1.0f, {{1.0f, 0.0f, 0.0f, 1.0f}});
					m_drawer.addLine(sq->m_averagePos, sq->m_targetPos, 1.0f, {{0.0f, 0.0f, 1.0f, 1.0f}});
				}

				m_drawer.drawLines(2.0f);
				m_drawer.clearVectors();

				m_drawer.drawText("Goal " + std::to_string(contextPtr->getGoal()), "pix", tovec2(csp) + vec2(0.0f, 128.0f), true, 2.0f);
			}
		}
		else
		{
			infoCard->setVisible(false);
		}

		if(contextPtr != nullptr and contextPtr->getCargo()->isVisible())
		{
			m_drawer.useShader("plain");
			vec2 dim = contextPtr->getCargo()->getDim();
			m_drawer.addRect(contextPtr->getInterpolatedPosition(_dt), dim, 0.0f, {{0.8f, 0.8f, 0.8f, 0.8f}});
			m_drawer.drawRects(true);
			m_drawer.clearVectors();

			m_drawer.useShader("ship");
			//CRASH HERE
			for(auto &i : contextPtr->getCargo()->getItems()->m_objects)
			{
				m_drawer.drawAsset(contextPtr->getInterpolatedPosition(_dt) + i.getInterpolatedPosition(_dt), i.getAng(), i.getIdentifier());
			}
			m_drawer.useShader("plain");
		}
	}

	if(m_ply.getCargo()->isVisible())
	{
		m_drawer.useShader("plain");

		vec2 dim = m_ply.getCargo()->getDim();
		m_drawer.addRect(m_ply.getInterpolatedPosition(_dt), dim, 0.0f, {{0.8f, 0.8f, 0.8f, 0.8f}});
		m_drawer.drawRects(true);
		m_drawer.clearVectors();

		m_drawer.useShader("ship");
		//CRASH HERE
		for(auto &i : m_ply.getCargo()->getItems()->m_objects)
		{
			m_drawer.drawAsset(m_ply.getInterpolatedPosition(_dt) + i.getInterpolatedPosition(_dt), i.getAng(), i.getIdentifier());
		}
	}

	for(auto i = m_ui.getElements()->begin(); i != m_ui.getElements()->end(); ++i)
	{
		if(!i->isVisible()) continue;
		//m_drawer.addRect( tovec3(i->getPos() + i->getDim() * 0.5f), i->getDim(), 0.0f, {{1, 0, 0, .5f}} );
		for(auto j = i->getButtons()->begin(); j != i->getButtons()->end(); ++j)
		{
			std::array<float, 4> col = j->getDrawCol();

			vec2 jdim = j->getDim();
			vec2 jpos = j->getPos();

			jpos += jdim * 0.5f;

			m_drawer.addRect(tovec3(jpos), jdim, 0.0f, col);
		}

		m_drawer.useShader("plain");
		m_drawer.drawRects(i->inWorldSpace());
		m_drawer.clearVectors();

		for(auto k = i->getButtons()->begin(); k != i->getButtons()->end(); ++k)
		{
			std::array<float, 4> col = col255to1(k->getTCol());

			vec2 kdim = k->getDim();
			vec2 kpos = k->getPos();

			kpos.m_x += kdim.m_x * 0.25f;
			kpos.m_y += kdim.m_y * 0.5f;

			m_drawer.drawText(k->getLabel(), "pix", kpos, i->inWorldSpace(), k->getTextSizeMul(), col);
		}
	}
}

void universe::drawDebugUI()
{
	m_drawer.clearVectors();
	size_t i = 0;
	for(auto &f : m_factions)
	{
		if(f.isOrganised())
		{
			std::array<float, 3> pb = f.getPowerBalance();
			float x = i * 256.0f + 256.0f;
			float y = 128.0f;
			m_drawer.drawText( f.getIdentifier(), "pix", vec2(x, y), false, 1.0f, col255to1(f.getColour()) ); y += 20.0f;
			m_drawer.drawText( "WEALTH : " + std::to_string(static_cast<int>(std::round(f.getWealth()))), "pix", vec2(x, y), false, 1.0f, {{1.0f, 0.9f, 0.0f, 1.0f}} ); y += 20.0f;
			m_drawer.drawText( "WEALTH-DT : " + std::to_string(f.getWealthDT()), "pix", vec2(x, y), false, 1.0f, {{1.0f, 1.0f, 0.5f, 1.0f}} ); y += 20.0f;
			m_drawer.drawText( "RESERVES POWER : " + std::to_string(static_cast<int>(std::round(f.getReservesPower()))), "pix", vec2(x, y), false, 1.0f ); y += 20.0f;
			m_drawer.drawText( "FRIENDLY POWER : " + std::to_string(static_cast<int>(std::round(pb[2]))), "pix", vec2(x, y), false, 1.0f ); y += 20.0f;
			m_drawer.drawText( "TARGET POWER : " + std::to_string( static_cast<int>(std::round(f.getTargetPower())) ), "pix", vec2(x, y), false, 1.0f); y += 20.0f;
			m_drawer.drawText( "ENEMY POWER : " + std::to_string(static_cast<int>(std::round(pb[0]))), "pix", vec2(x, y), false, 1.0f ); y += 20.0f;

			++i;
		}
	}
}

void universe::detectCollisions(
		SDL_Rect _box,
		std::vector<enemy*> _ships,
		std::vector<laser*> _lasers,
		std::vector<missile*> _rockets,
		std::vector<ship*> _rocks,
		std::vector<debris*> _resources,
		unsigned short int _lvl
		)
{
	size_t count = 0;
	std::vector<enemy*> pships;
	std::vector<laser*> plasers;
	std::vector<missile*> prockets;
	std::vector<ship*> procks;
	std::vector<debris*> presources;

	vec2 boxPos = {static_cast<float>(_box.x), static_cast<float>(_box.y)};
	vec2 boxDim = {static_cast<float>(_box.w), static_cast<float>(_box.h)};

	//Check through all input vectors.
	for(auto& i: _ships)
	{
		if( circleIntersectRectRough(tovec2(i->getPos()), i->getRadius(), boxPos, boxDim ) )
		{
			pships.push_back(i);
			count++;
		}
	}

	for(auto &i : _rockets)
	{
		if( circleIntersectRectRough(tovec2(i->getPos()), i->getRadius(), boxPos, boxDim ) )
		{
			prockets.push_back(i);
			count++;
		}
	}

	for(auto &i : _rocks)
	{
		if( circleIntersectRectRough(tovec2(i->getPos()), i->getRadius(), boxPos, boxDim ) )
		{
			procks.push_back(i);
			count++;
		}
	}

	for(auto &i : _lasers)
	{
		vec2 p = tovec2(i->getPos());
		vec2 v = p + tovec2(i->getVel());

		int sx = p.m_x, sy = p.m_y, ex = v.m_x, ey = v.m_y;
		if( SDL_IntersectRectAndLine(&_box, &sx, &sy, &ex, &ey ) )
		{
			plasers.push_back(i);
			count++;
		}
	}

	for(auto &i : _resources)
	{
		if( circleIntersectRectRough(tovec2(i->getPos()), i->getRadius(), boxPos, boxDim) )
		{
			presources.push_back(i);
			count++;
		}
	}

	//Return if count is too low, or nothing can interact.
	if(count < 16 or _lvl > 5)
	{
		m_partitions.push_back({
														 pships,
														 plasers,
														 prockets,
														 procks,
														 presources,
													 });

	}
	//Subdivide otherwise.
	else
	{
		int x = _box.x, y = _box.y, w = _box.w / 2, h = _box.h / 2;
		SDL_Rect r1 = {x, 		y, 		w, 		h};
		SDL_Rect r2 = {x + w, 	y, 		w, 		h};
		SDL_Rect r3 = {x, 		y + h, 	w, 		h};
		SDL_Rect r4 = {x + w, 	y + h, 	w, 		h};

		detectCollisions(r1, pships, plasers, prockets, procks, presources, _lvl + 1);
		detectCollisions(r2, pships, plasers, prockets, procks, presources, _lvl + 1);
		detectCollisions(r3, pships, plasers, prockets, procks, presources, _lvl + 1);
		detectCollisions(r4, pships, plasers, prockets, procks, presources, _lvl + 1);
	}
}

void universe::checkCollisions()
{
	for(size_t p = 0; p < m_partitions.size(); ++p)
	{
		bool done = false;
		int harm = 0;

		//LASERS
		//Pew pew
		for(int l = m_partitions[p].m_lasers.size() - 1; l >= 0; --l)
		{
			vec3 sp = m_partitions[p].m_lasers[l]->getPos();
			vec3 sv = m_partitions[p].m_lasers[l]->getVel() * 5.0f;
			vec3 spv = sp + sv * 1.5;
			float stop = m_partitions[p].m_lasers[l]->getStop();
			slot so = m_partitions[p].m_lasers[l]->getOwner();
			float sd = m_partitions[p].m_lasers[l]->getDmg();

			vec3 ep;
			vec3 ev;
			float er;

			std::array<float, 4> sc = m_partitions[p].m_lasers[l]->getCol();

			vec3 d_dir;

			//cout << "ENEMY CHECK" << endl;
			for(int s = m_partitions[p].m_ships.size() - 1; s >= 0; s--)
			{
				if(neutralityCheck(m_partitions[p].m_lasers[l]->getTeam(), m_partitions[p].m_ships[s]->getTeam())) continue;
				harm = 0;

				ep = m_partitions[p].m_ships[s]->getPos();
				ev = m_partitions[p].m_ships[s]->getVel();
				er = m_partitions[p].m_ships[s]->getRadius();

				//if(lineIntersectCircle(tovec2(sp), tovec2(spv), tovec2(ep), er))
				vec3 contact;
				if(lineIntersectSphere(sp, spv, ep, er, &contact))
				{
					addpfx(contact, ev, sd * randNum(1.0f, 3.0f), randNum(3.0f, 8.0f), sc);
					harm = sd;

					d_dir = m_partitions[p].m_lasers[l]->getVel();
					//Delete m_shots if they match the ones in the universe vector.
					for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots[i] == m_partitions[p].m_lasers[l]) swapnpop(&m_shots, i);
					//Pop the partition pointer.
					swapnpop(&m_partitions[p].m_lasers, l);
					done = true;
				}
				if(harm > 0)
				{
					enemy * lastAttacker = m_agents.getByID(so);

					float xp = clamp( calcAICost(m_partitions[p].m_ships[s]->getClassification()) * harm * 0.00005f, 0.0f, 0.5f);

					if(lastAttacker != nullptr) lastAttacker->addXP( xp );
					else if(so == slot(0, -2))
						m_ply.addXP(xp);

					float realDamage = m_partitions[p].m_ships[s]->damage(harm, d_dir * stop, so);
					addDamagePopup(realDamage, m_partitions[p].m_ships[s]->getTeam(), ep, m_partitions[p].m_ships[s]->getVel() + randVec3(2.0f));
					break;
				}
			}
			if(done) continue;

			for(int r = m_partitions[p].m_rocks.size() - 1; r >= 0; r--)
			{
				//cout << "R is " << r << ") " << m_partitions[p].m_rocks[r] << endl;
				harm = 0;

				ep = m_partitions[p].m_rocks[r]->getPos();
				ev = m_partitions[p].m_rocks[r]->getVel();
				er = m_partitions[p].m_rocks[r]->getRadius();

				vec3 contact;
				//if(lineIntersectCircle(tovec2(sp), tovec2(spv), tovec2(ep), er))
				if(lineIntersectSphere(sp, spv, ep, er, &contact))
				{
					addpfx(contact, ev, sd * randNum(1.0f, 3.0f), randNum(3.0f, 8.0f), {{2, 2, 2, 0}});
					for(int q = 0; q < 20; ++q) addParticleSprite(ep, ev + tovec3(randVec2(1.0f)), er * 2.0f, "SMOKE");
					harm = sd;

					d_dir = m_partitions[p].m_lasers[l]->getVel();
					for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots[i] == m_partitions[p].m_lasers[l]) swapnpop(&m_shots, i);//m_shots.erase(m_shots.begin() + i);
					swapnpop(&m_partitions[p].m_lasers, l);

					done = true;
				}
				if(harm > 0)
				{
					m_partitions[p].m_rocks[r]->damage(harm, d_dir * stop, so);
					break;
				}
			}
			if(done) continue;

			//cout << "ENEMY CHECK" << endl;
			for(int m = m_partitions[p].m_rockets.size() - 1; m >= 0; m--)
			{
				harm = 0;

				ep = m_partitions[p].m_rockets[m]->getPos();
				ev = m_partitions[p].m_rockets[m]->getVel();
				er = m_partitions[p].m_rockets[m]->getRadius();

				vec3 contact;
				//if(lineIntersectCircle(tovec2(sp), tovec2(spv), tovec2(ep), er))
				if(lineIntersectSphere(sp, spv, ep, er, &contact))
				{
					addpfx(contact, ev, randNum(3.0f, 8.0f), sd * randNum(1.0f, 3.0f), {{255, 200, 20, 255}});

					harm = sd;

					d_dir = m_partitions[p].m_lasers[l]->getVel();
					for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots[i] == m_partitions[p].m_lasers[l]) m_shots.erase(m_shots.begin() + i);
					swapnpop(&m_partitions[p].m_lasers, l);

					done = true;
				}
				if(harm > 0)
				{
					float realDamage = m_partitions[p].m_rockets[m]->damage(harm, d_dir * stop, so);
					addDamagePopup(realDamage, TEAM_PLAYER, ep, m_partitions[p].m_rockets[m]->getVel() + randVec3(2.0f));
					break;
				}
			}
			if(done) continue;

			harm = 0;

			vec3 dp = sp - m_ply.getPos();
			vec3 dv = sv + m_ply.getVel();
			vec3 contact;
			if(!g_GAME_OVER and fabs(dv.m_x) > fabs(dp.m_x) - 32 and fabs(dv.m_y) > fabs(dp.m_y) - 32 and emnityCheck(m_partitions[p].m_lasers[l]->getTeam(), TEAM_PLAYER))
			{
				vec3 spv = sp + sv;

				//if(lineIntersectCircle(tovec2(sp), tovec2(spv), tovec2(dp), m_ply.getRadius()))
				if(lineIntersectSphere(sp, spv, dp, m_ply.getRadius(), &contact))
				{
					playUISnd(RICOCHET_SND);
					m_drawer.addShake(5.0f);
					addpfx(contact, m_ply.getVel(), sd * randNum(1.0f, 3.0f), randNum(3.0f, 8.0f), sc);
					harm = sd;

					d_dir = m_partitions[p].m_lasers[l]->getVel();
					for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots[i] == m_partitions[p].m_lasers[l]) swapnpop(&m_shots, i);
					swapnpop(&m_partitions[p].m_lasers, l);

					done = true;
				}
				if(harm > 0)
				{
					m_ply.damage(harm, d_dir * stop, so);
					//std::cout << "ADDING VEL (" << sv.m_x << "," << sv.m_y << " - " << ev.m_x << "," << ev.m_y << ") * " << stop << " = (" << ( (sv - ev) * stop ).m_x << ", " << ( (sv - ev) * stop ).m_y << ")" << std::endl;
					setVel(-m_ply.getVel());
				}
			}

			//Space-station healing check.
			for(size_t i = 0; i < m_partitions[p].m_ships.size(); ++i)
			{
				enemy * a = m_partitions[p].m_ships[i];
				for(size_t j = 0; j < m_partitions[p].m_ships.size(); ++j)
				{
					enemy * b = m_partitions[p].m_ships[j];

					//If a is a station, and b the two ships are on the same team...
					if( a->getType() == SHIP_TYPE_STATION and friendshipCheck( a->getTeam(), b->getTeam() ) and a != b )
					{
						float dist = magns( b->getPos() - a->getPos() );
						if(dist < sqr(a->getRadius())) b->incrHealth(0.01f);
					}
				}

				if( a->getType() == SHIP_TYPE_STATION and friendshipCheck( a->getTeam(), TEAM_PLAYER ) )
				{
					float dist = magns( m_ply.getPos() - a->getPos() );
					if(dist < sqr(a->getRadius())) m_ply.incrHealth(0.01f);
				}
			}
		}

		//COLLISIONS
		//It made sense at the time...
		for(auto &i : m_partitions[p].m_ships)
		{
			//Ships vs ships
			for(auto &j : m_partitions[p].m_ships)
			{
				if(emnityCheck(i->getTeam(), j->getTeam()) and i != j)
				{
					resolveCollision(reinterpret_cast<ship *>(i), reinterpret_cast<ship *>(j));
				}
			}

			//Ships vs rocks
			for(auto &j : m_partitions[p].m_rocks)
			{
				if(i->getType() != SHIP_TYPE_MINER) resolveCollision(reinterpret_cast<ship *>(i), reinterpret_cast<ship *>(j));
			}

			//Ships vs player
			if(emnityCheck(i->getTeam(), TEAM_PLAYER) and !g_GAME_OVER) resolveCollision(reinterpret_cast<ship *>(i), reinterpret_cast<ship *>(&m_ply));
		}

		for(auto &i : m_partitions[p].m_rocks)
		{
			if(!g_GAME_OVER) resolveCollision(reinterpret_cast<ship *>(i), reinterpret_cast<ship *>(&m_ply));
			for(auto &j : m_partitions[p].m_rocks)
			{
				if(i != j) resolveCollision(reinterpret_cast<ship *>(i), reinterpret_cast<ship *>(j));
			}
		}

		//RESOURCE PICKUPS
		//Get rich
		for(int i = m_partitions[p].m_resources.size() - 1; i >= 0; --i)
		{
			debris * resource = m_partitions[p].m_resources[i];
			for(size_t j = 0; j < m_partitions[p].m_ships.size(); ++j)
			{
				ship * agent = m_partitions[p].m_ships[j];
				if(circleIntersectCircle(tovec2(resource->getPos()), resource->getRadius(), tovec2(agent->getPos()), agent->getRadius()))
				{
					if(!agent->addItem(*resource)) continue;

					for(int k = m_resources.size() - 1; k >= 0; --k) if(&m_resources[k] == resource) swapnpop(&m_resources, k);
					swapnpop(&m_partitions[p].m_resources, i);
					break;
				}
				else
				{
					vec3 add = agent->getPos() - resource->getPos();
					add = add / (magns(add) * 200.0f);
					resource->addForce(add);
				}
			}

			if(circleIntersectCircle(tovec2(resource->getPos()), resource->getRadius(), tovec2(m_ply.getPos()), m_ply.getRadius() and !g_GAME_OVER))
			{
				std::cout << "intersection!!!!\n";
				if(!m_ply.addItem(*resource)) continue;

				m_sounds.playUISnd(PLACE_SND);

				for(int k = m_resources.size() - 1; k >= 0; --k) if(&m_resources[k] == resource) swapnpop(&m_resources, k);
				swapnpop(&m_partitions[p].m_resources, i);
			}
			else
			{
				vec3 add = m_ply.getPos() - resource->getPos();
				add = add / (magns(add) * 200.0f);
				resource->addForce(add);
			}
		}
	}
}

void universe::resolveCollision(slot _a, slot _b)
{
	if(_a != _b) return;
	ship * a = m_agents.getByID(_a);
	ship * b = m_agents.getByID(_b);
	resolveCollision(a, b);
}

void universe::resolveCollision(ship *_a, ship *_b)
{
	if(_a->hasParent() or _b->hasParent() or _a == _b) return;

	float dist = magns(_a->getPos() - _b->getPos());
	if(dist > sqr(_a->getRadius() + _b->getRadius())) return;

	dist = sqrt(dist);
	vec3 normal = (_b->getPos() - _a->getPos()) / dist;
	normal.m_z = 0.0f;

	//Spooky boolean math here we go
	float ainvmass = _a->getInertia() * _a->getCanMove();
	float binvmass = _b->getInertia() * _b->getCanMove();
	float cinvmass = ainvmass + binvmass;

	//Static objects do not collide.
	if(cinvmass == 0.0f) return;

	_a->damage(0.0f);
	_b->damage(0.0f);

	//Positional correction.
	float diff = _a->getRadius() + _b->getRadius() - dist;
	vec3 correction = diff * normal;
	correction.m_z = 0.0f;

	_a->addPos( -correction * (ainvmass / cinvmass) );
	_b->addPos( correction * (binvmass / cinvmass) );

	vec3 rv = _a->getVel() - _b->getVel();
	rv.m_z = 0.0f;
	float separation = dotProd(rv, normal);

	if(separation < 0.0f) return;

	float force = -1.6f * separation;
	force /= cinvmass;

	vec3 impulse = force * normal;
	impulse.m_z = 0.0f;

	float admg = fabs(force) * fmax(ainvmass, 0.05f);
	float bdmg = fabs(force) * fmax(binvmass, 0.05f);

	//addDamagePopup(admg, m_partitions[p].m_ships[s]->getTeam(), ep, ev);
	//addDamagePopup(bdmg, m_partitions[p].m_ships[s]->getTeam(), ep, ev);

	_a->damage(admg, _b->getVel());
	_b->damage(bdmg, _a->getVel());

	if(_a->getHealth() > 0.0f) _a->addVel( ainvmass * impulse );
	if(_b->getHealth() > 0.0f) _b->addVel( -binvmass * impulse );
}

void universe::addpfx(
		const vec3 _p,
		const vec3 _v,
		const int _no,
		const float _f,
		const std::array<float, 4> _col
		)
{
	pfx pf(_p, _v, m_vel, _no, _f, "EXPLOSION", _col);
	m_particles.push_back(pf);
}

ship * universe::closestEnemy(vec3 p, aiTeam t)
{
	float best_dist = F_MAX;
	ship * r = nullptr;
	for(size_t i = 0; i < m_agents.size(); i++)
	{
		if(!emnityCheck(m_agents[i].getTeam(), t)) continue;
		vec3 pe = m_agents[i].getPos();
		float dist = magns(pe-p);

		if(dist < best_dist)
		{
			best_dist = dist;
			r = &m_agents[i];
		}
	}
	return r;
}

void universe::addParticleSprite(
		const vec3 _p,
		const vec3 _v,
		const float _dim,
		const std::string _tex
		)
{
	float col = 0.0f;
	if( _tex == "SMOKE" )
		col = randNum(200.0f, 220.0f);
	else if( _tex == "EXPLOSION" )
		col = 255.0f;

	sprite sm (_tex, col, _dim, _dim);
	sm.setPos(_p);
	sm.setPPos(_p);
	if( _tex == "SMOKE" ) sm.setVel(_v + tovec3(randVec2(1.0f)));
	else if( _tex == "EXPLOSION" ) sm.setVel(_v);
	m_passiveSprites.push_back(sm);
}

void universe::spawnShip(const aiTeam _t)
{
	int side = rand()%4;
	vec2 pass;

	if(side == 0) pass = {randNum(-20000.0f,20000.0f), -20000.0f};
	else if(side == 1) pass = {-20000.0f,randNum(-20000.0f, 20000.0f)};
	else if(side == 2) pass = {randNum(-20000.0f, 20000.0f), 20000.0f};
	else if(side == 3) pass = {20000.0f, randNum(-20000.0f, 20000.0f)};

	spawnShip(getRandomShipType(_t), _t, tovec3(pass) + getCameraPosition());
}

ship_spec universe::getRandomShipType(const aiTeam _t)
{
	int prob = rand()%1000;

	ship_spec type = SHIPS_END;

	if( _t == SPACE_COMMUNISTS )
	{
		type = COMMUNIST_1;
		if(prob > 500 and prob <= 990) type = COMMUNIST_2;
		else if(prob > 990) type = COMMUNIST_CAPITAL;
	}
	else if( _t == SPOOKY_SPACE_PIRATES )
	{
		type = PIRATE_GNAT;
		if(prob > 750 and prob <= 800) type = PIRATE_CRUISER;
		else if(prob > 800 and prob <= 900) type = PIRATE_WRANGLER;
		else if(prob > 900 and prob <= 980) type = PIRATE_MARAUDER;
		else if(prob > 980 and prob <= 995) type = PIRATE_GUNSHIP;
		else if(prob > 995) type = PIRATE_CAPITAL;
	}
	else if( _t == GALACTIC_FEDERATION )
	{
		type = FEDERATION_MKI;
		if(prob > 750 and prob <= 800) type = FEDERATION_MKII;
		else if(prob > 800 and prob <= 900) type = FEDERATION_MKIII;
		else if(prob > 900 and prob <= 980) type = FEDERATION_MKIV;
		else if(prob > 980 and prob <= 995) type = FEDERATION_GUNSHIP;
		else if(prob > 995 and prob <= 999) type = FEDERATION_CAPITAL;
	}
	else if( _t == TEAM_PLAYER )
	{
		type = PLAYER_HUNTER;
		if(prob > 450 and prob <= 750) type = PLAYER_DEFENDER;
		else if(prob > 750 and prob <= 950) type = PLAYER_DESTROYER;
		else if(prob > 950) type = PLAYER_GUNSHIP;
		m_wingmenCount++;
	}
	else if( _t == ALLIANCE )
	{
		type = ALLIANCE_SCOUT;
		if(prob > 400 and prob <= 700) type = ALLIANCE_TRACKER;
		else if(prob > 700 and prob <= 800) type = ALLIANCE_PHOENIX;
		else if(prob > 800 and prob <= 900) type = ALLIANCE_DRAGON;
		else if(prob > 900 and prob <= 950) type = ALLIANCE_GUNSHIP;
		else if(prob > 950) type = ALLIANCE_TRADER;
	}

	return type;
}

void universe::spawnShip(
		const ship_spec _type,
		const aiTeam _t,
		const vec3 _p
		)
{
	enemy newShip(_p, -m_vel, _type, _t);
	if(_type == PLAYER_MINER_DROID) m_minerCount++;
	m_factions[_t].addActive(_type, 1);
	m_agents.push_back(newShip);

	//Do not assign squad if enemy cannot move and shoot.
	if(!newShip.getCanMove() or !newShip.getCanShoot() or newShip.getType() == SHIP_TYPE_MINER) return;

	std::vector<enemy> temp;
	ship_spec turretType;
	//Turrets.
	switch(_t)
	{
	case TEAM_PLAYER:
		turretType = PLAYER_TURRET;
		break;
	case GALACTIC_FEDERATION:
		turretType = FEDERATION_TURRET;
		break;
	case SPOOKY_SPACE_PIRATES:
		turretType = PIRATE_TURRET;
		break;
	case SPACE_COMMUNISTS:
		turretType = COMMUNIST_TURRET;
		break;
	default:
		turretType = PLAYER_TURRET;
		break;
	}

	//Turrets
	for(auto &p : newShip.getAttachmentPoints())
	{
		enemy temp1(vec3(), vec3(), turretType, _t);
		shipAddParent(m_agents.size() - 1, &temp1, p);
		temp.push_back(temp1);
	}

	bool quit = false;
	//Assign squad if enemy can move and shoot.
	for(auto &s : m_factions[_t].getSquads())
	{
		if(
			 s.m_size < s.m_max_size and
			 /*magns(m_agents.m_objects.back().getPos() - s.m_centerPoint) < sqr(s.m_regroupDist) and*/
			 rand() % 100 < 95)
		{
			addToSquad(&m_agents.m_objects.back(), m_factions[_t].getBackSquad());
			quit = true;
			break;
		}
	}

	if(!quit)
	{
		//Create a new squad if no suitable one exists.
		squad tempSquad = createSquad(_t);
		//tempSquad.m_id = m_squads.size();
		//addToSquad(&m_agents.m_objects.back(), &tempSquad);
		//m_squads.push_back(tempSquad);
		tempSquad.m_targetPos = _p;

		m_factions[_t].addSquad(tempSquad);

		addToSquad(&m_agents.m_objects.back(), m_factions[_t].getBackSquad());
	}

	for(auto &q : temp) m_agents.push_back(q);
}

void universe::addToSquad(
		enemy *_e,
		slot _s
		)
{
	squad * sq = m_factions[_e->getTeam()].getSquad(_s);
	if(sq != nullptr)
	{
		_e->setSquadID(_s);
		sq->m_size += 1;
		sq->m_strength += calcAICost(_e->getClassification());
	}
}

void universe::removeFromSquad(
		enemy *_e
		)
{
	squad * sq = m_factions[_e->getTeam()].getSquad(_e->getSquadID());

	_e->setSquadID({0, -1});

	if(sq != nullptr)
	{
		sq->m_size -= 1;
		sq->m_strength -= calcAICost(_e->getClassification());
	}
}

void universe::spawnSquad(
		const aiTeam _t,
		const float _min,
		const float _max,
		const int _i)
{
	vec3 p = tovec3(randVec2(_min, _max));
	for(int i = 0; i < _i; ++i)
	{
		spawnShip(getRandomShipType(_t), _t, p + tovec3(randVec2(512.0f)) + getCameraPosition());
	}
}

void universe::spawnSquad(
		const aiTeam _t,
		const float _min,
		const float _max,
		const std::vector<size_t> _ships
		)
{
	vec3 p = tovec3(randVec2(_min, _max));
	for(size_t i = 0; i < _ships.size(); ++i)
	{
		for(size_t j = 0; j < _ships[i]; ++j)
		{
			spawnShip(static_cast<ship_spec>(i), _t, p + tovec3(randVec2(128.0f)) + getCameraPosition());
			//spawnShip(PLAYER_SHIP, _t, p + tovec3(randVec2(512.0f)) + getCameraPosition());
		}
	}
}

void universe::spawnBase(
		const aiTeam _t,
		const vec3 _center,
		int _lvl,
		float _ang
		)
{
	std::vector<enemy> ships;
	std::vector<enemy> active;
	std::vector<ship_spec> structures;
	int start;

	for(ship_spec i = m_factions[_t].getStructures().first; i < m_factions[_t].getStructures().second; ++i)
		structures.push_back(i);

	if(structures.size() == 0)
		return;

	start = clamp(_lvl, 0, static_cast<int>(structures.size()) - 1);

	ships.push_back( enemy(_center, vec3(), structures[start], _t) );
	//std::cout << "FIRST: " << structures[start] << "\n";
	/*std::cout << start << " done\n";
																																		m_agents.push_back( enemy(vec3(), vec3(), ALLIANCE_STATION, TEAM_PLAYER) );
																																		return;*/

	float deg = _ang;
	float distMul = 1.5f;
	for(int i = start - 1; i >= 0; --i)
	{
		//std::cout << i << '\n';
		for(auto &j : ships)
		{
			//std::cout << "  j\n";
			for(float k = 0.0f; k < 360.0f; k += deg)
			{
				//std::cout << "     " << k << '\n';
				enemy temp (j.getPos() + tovec3(vec(k)) * j.getRadius() * distMul, vec3(), structures[i], _t);
				active.push_back( temp );
			}
		}

		for(auto &u : ships)
			m_agents.push_back( u );
		ships.clear();
		for(auto &u : active)
			ships.push_back( u );
		active.clear();

		//distMul /= 2.0f;
		deg /= 2.0f;
	}

	for(auto &i : ships)
		m_agents.push_back( i );
}

bool universe::emnityCheck(
		aiTeam _a,
		aiTeam _b
		)
{
	return m_factions[_a].isEnemy(_b)
			and m_factions[_b].isEnemy(_a);
}

bool universe::friendshipCheck(
		aiTeam _a,
		aiTeam _b
		)
{
	return m_factions[_a].isFriend(_b)
			and m_factions[_b].isFriend(_a);
}

bool universe::neutralityCheck(
		aiTeam _a,
		aiTeam _b
		)
{
	return m_factions[_a].isNeutral(_b)
			and m_factions[_b].isNeutral(_a);
}

bool universe::sameTeam(
		aiTeam _a,
		aiTeam _b
		)
{
	return m_factions[_a].isSame(_b)
			and m_factions[_b].isSame(_a);
}

void universe::reload(const bool _newGame)
{
	m_agents.clear();
	m_missiles.clear();
	m_shots.clear();
	m_asteroids.clear();
	m_passiveSprites.clear();
	m_particles.clear();
	m_resources.clear();
	m_factions.clear();

	m_ply.setPos(vec3());
	setCameraPosition(vec3());
	m_pos = vec3();

	m_vel = vec3();
	m_pos = vec3();
	m_ply.setHealth( m_ply.getMaxHealth() );
	m_ply.setShield( m_ply.getMaxShield() );
	m_ply.setEnergy( m_ply.getMaxEnergy() );
	m_ply.setVel(vec3());

	m_ui.clear();
	initUI();
	m_ui.reset();
	m_paused = false;

	m_partitions.clear();

	std::cout << "p1\n";
	m_time_elapsed = 0.0;

	m_mouse_state = -1;

	m_escMenuShown = false;

	std::cout << "p2\n";
	m_maxMiners = 0;
	m_maxWingmen = 0;
	m_minerCount = 0;
	m_wingmenCount = 0;

	std::cout << m_ply.getIdentifier() << '\n';
	m_ply.setWeapData(0,0);
	m_ply.setWeapData(1,1);
	m_ply.setWeapData(2,2);
	std::cout << "p3\n";

	m_ply.setEnginePower(5.0f);
	m_ply.setGeneratorMul(1.0f);
	m_ply.setShieldMul( 1.0f );

	m_contextShip = {0, -1};

	m_ply.getCargo()->getItems()->clear();

	if(!_newGame)
		return;

	//m_ply.setGeneratorMul(1000.0f);
	createFactions();

	m_sounds.playUISnd(SAVE_SND);
	for(int i = 0; i < UPGRADES_LEN; ++i)
	{
		m_ply.setGradeArr(i, 0);
	}

	m_ply.setMissiles(3);
	setScore(16);

	std::vector<float> degs = {{60.0f, 90.0f, 180.0f}};
	size_t range = m_factions[ALLIANCE].getStructures().second - m_factions[ALLIANCE].getStructures().first;
	for(int i = 0; i < 3; ++i)
	{
		spawnBase(
					ALLIANCE,
					tovec3( randVec2(1000000.0f) ),
					rand() % (range - 1) + 1,
					degs[rand() % degs.size()]
				);
	}

	std::cout << "reloading done\n";
}

void universe::addBuild(const ship_spec _type, const aiTeam _team)
{
	vec2 pass;

	int side = rand() % 4;
	if(side == 0) pass = {randNum(-20000.0f,20000.0f), -20000.0f};
	else if(side == 1) pass = {-20000.0f,randNum(-20000.0f, 20000.0f)};
	else if(side == 2) pass = {randNum(-20000.0f, 20000.0f), 20000.0f};
	else if(side == 3) pass = {20000.0f, randNum(-20000.0f, 20000.0f)};

	addBuild(tovec3(pass), _type, _team);
}

void universe::addBuild(
		const vec3 _p,
		const ship_spec _type,
		const aiTeam _team
		)
{
	enemy newShip(_p, {0.0f, 0.0f, 0.0f}, _type, _team);

	if(_type == PLAYER_TURRET)
	{
		//Attach to a player capital ship, if any is close enough.
		for(size_t i = 0; i < m_agents.size(); ++i)
		{
			if(m_agents[i].getClassification() == PLAYER_CAPITAL and magns(_p - m_agents[i].getPos()) < sqr(m_agents[i].getRadius()))
			{
				vec3 pos = _p - m_agents[i].getPos();
				float angle = -m_agents[i].getAng();
				float s = sin(rad(angle));
				float c = cos(rad(angle));

				float xn = pos.m_x * c - pos.m_y * s;
				float yn = pos.m_x * s + pos.m_y * c;
				vec3 comb = {xn, yn, 0.0f};

				shipAddParent(i, &newShip, comb);
			}
		}
	}

	m_agents.push_back(newShip);

	addpfx(_p, {0.0f, 0.0f, 0.0f}, rand()%20 + 50, newShip.getRadius() * randNum(0.3f, 0.5f), {{0.2f, 0.2f, 0.3f, 1.0f}});
	for(int q = 0; q < 50; ++q) addParticleSprite(_p, tovec3(randVec2(1.0f)), 128.0f, "SMOKE");
	m_sounds.playSnd(PLACE_SND, _p, 1.0f);
	m_sounds.playSnd(CLUNK_SND, _p, 1.0f);
}

void universe::initUI()
{
	//Initialise the two selection menus.
	selection energy_menu = loadSelection("priorityMenu.txt");
	selection upgrades_menu = loadSelection("upgradesMenu.txt");
	selection buy_menu = loadSelection("buyMenu.txt");

	selection info_menu;
	info_menu.setVisible(false);
	info_menu.setInWorldSpace(true);
	button name("", {{0,0,0,0}}, {{255,255,255,255}}, vec2(-32.0f, 0.0f), vec2(128.0f,32.0f));
	button kills("KILLS: ", {{0,0,0,0}}, {{255,255,255,255}}, vec2(-32.0f, 96.0f), vec2(128.0f,32.0f));
	button distance("DISTANCE: ", {{0,0,0,0}}, {{255,255,255,255}}, vec2(-32.0f, 112.0f), vec2(128.0f,32.0f));
	button trade("TRADE", {{100,100,100,255}}, {{255,255,255,255}}, vec2(0.0f, 144.0f), vec2(64.0f,32.0f));
	button close("CLOSE", {{255,10,20,255}}, {{255,255,255,255}}, vec2(64.0f, 144.0f), vec2(64.0f,32.0f));
	info_menu.add(name);
	info_menu.add(kills);
	info_menu.add(distance);
	info_menu.add(trade);
	info_menu.add(close);

	m_ui.add(energy_menu);
	m_ui.add(upgrades_menu);
	m_ui.add(buy_menu);
	m_ui.add(info_menu);
}

bool universe::upgradeCallback(
		int _sel,
		int _btn
		)
{
	//This function takes the selected button, looks at the cost vs the score, updates relevant values,
	//then returns a bool representing whether the upgrade was successful or unsuccessful.

	button * selectedbutton = &m_ui.getElements()->at(_sel).getButtons()->at(_btn);

	//The current upgrade level, -1 if it is a buy attempt.
	int lvl;
	if(_sel == 1) lvl = m_ply.getUpgrade( _btn );
	else if(_sel == 2) lvl = -1;

	selectedbutton->set(false);

	if(selectedbutton->getCost() > m_factions[TEAM_PLAYER].getWealth() or lvl > 9)
	{
		m_sounds.playUISnd(MENU_FAIL_SND);
		return false;
	}

	if(lvl < 9)
	{
		addScore( -selectedbutton->getCost() );
		if(_btn < 4 and _sel == 1) selectedbutton->setCost(selectedbutton->getCost() * 2);
		else return true;
	}

	m_ply.upgrade(_btn);
	upgradeSetLabels(_sel, _btn, -1);

	return true;
}

void universe::upgradeSetLabels(
		int _sel,
		int _btn,
		int _plvl
		)
{
	if(_sel == 2) return;
	button * selectedbutton = &m_ui.getElements()->at(_sel).getButtons()->at(_btn);

	std::string s1;
	int lvl;
	if(_plvl < 0) lvl = m_ply.getUpgrade( _btn );
	else lvl = _plvl;

	switch(_btn)
	{
	case 0:
		s1 = "LASERS ";
		break;
	case 1:
		s1 = "SHIELDS ";
		break;
	case 2:
		s1 = "GENERATORS ";
		break;
	case 3:
		s1 = "THRUSTERS ";
		break;
	default:
		return;
		break;
	}
	s1 += g_ROMAN_NUMS[clamp(lvl, 0, 9)];

	if(lvl < 8)
	{
		selectedbutton->setCost(pow(2.0, lvl + 1) * 2);
		s1 += " (";
		std::stringstream ss;
		ss << selectedbutton->getCost();
		s1 += ss.str();
		s1 += ")";
	}

	selectedbutton->updateText(s1);

	addScore(0);

	std::cout << "UPDATING... " << s1 << "(" << lvl << ")" << std::endl;
}

void universe::createFactions()
{
	faction player("Player Team", {{0, 255, 0, 255}}, TEAM_PLAYER, {PLAYER_HUNTER, PLAYER_CAPITAL}, {PLAYER_MINER_DROID, PLAYER_MINER_DROID}, {PLAYER_TURRET, PLAYER_STATION}, false );
	player.setRelations( {DIPLOMACY_SELF, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_ENEMY} );
	player.setWealth( 16.0f );
	m_factions.push_back(player);

	faction galactic_fed("Galactic Federation", {{165, 14, 226, 255}}, GALACTIC_FEDERATION, {FEDERATION_MKI, FEDERATION_CAPITAL}, {SHIPS_END, SHIPS_END}, {FEDERATION_TURRET, FEDERATION_TURRET}, true );
	galactic_fed.setRelations( {DIPLOMACY_ENEMY, DIPLOMACY_SELF, DIPLOMACY_ENEMY, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY} );
	galactic_fed.setWealth( randNum(10000.0f, 16000.0f) );
	m_factions.push_back(galactic_fed);

	faction spooky_pirates("Spooky Space Pirates", {{240, 211, 10, 255}}, SPOOKY_SPACE_PIRATES, {PIRATE_GNAT, PIRATE_CAPITAL}, {SHIPS_END, SHIPS_END}, {PIRATE_TURRET, PIRATE_TURRET}, true );
	spooky_pirates.setRelations( {DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_SELF, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY} );
	spooky_pirates.setWealth( randNum(30000.0f, 32000.0f) );
	m_factions.push_back(spooky_pirates);

	faction space_communists("Space Communists", {{255, 0, 0, 255}}, SPACE_COMMUNISTS, {COMMUNIST_1, COMMUNIST_CAPITAL}, {SHIPS_END, SHIPS_END}, {COMMUNIST_TURRET, COMMUNIST_TURRET}, true );
	space_communists.setRelations( {DIPLOMACY_ENEMY, DIPLOMACY_NEUTRAL, DIPLOMACY_ENEMY, DIPLOMACY_SELF, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY} );
	space_communists.setWealth( randNum(24000.0f, 32000.0f) );
	m_factions.push_back(space_communists);

	faction alliance("Alliance", {{0, 255, 255, 255}}, ALLIANCE, {ALLIANCE_SCOUT, ALLIANCE_GUNSHIP}, {SHIPS_END, SHIPS_END}, {ALLIANCE_TURRET, ALLIANCE_STATION}, true );
	alliance.setRelations( {DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_SELF, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY} );
	alliance.setWealth( randNum(30000.0f, 35000.0f) );
	m_factions.push_back(alliance);

	faction neutral("Neutral", {{200, 200, 200, 255}}, NEUTRAL, {SHIPS_END, SHIPS_END}, {SHIPS_END, SHIPS_END}, {SHIPS_END, SHIPS_END}, false );
	neutral.setRelations( {DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL, DIPLOMACY_NEUTRAL} );
	m_factions.push_back(neutral);

	faction none("Unaffiliated", {{0, 0, 0, 255}}, NONE, {SHIPS_END, SHIPS_END}, {SHIPS_END, SHIPS_END}, {SHIPS_END, SHIPS_END}, false );
	none.setRelations( {DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY, DIPLOMACY_ENEMY} );
	m_factions.push_back(none);
}

squad * universe::getSquadFromID(aiTeam _t, slot _id)
{
	return m_factions[_t].getSquad(_id);
}

void universe::shipAddParent(size_t _index, ship * _child, vec3 _offset)
{
	slot s = m_agents.getID(_index);
	slotmap<ship> * ptr = reinterpret_cast<slotmap<ship> *> (&m_agents);
	_child->setParent( slotID<ship>(s.m_id, s.m_version, ptr)  );
	_child->setParentOffset(_offset);
}

void universe::addPopup(std::string _label, popup_type _type, float _smul, vec3 _pos, vec3 _vel)
{
	popup temp (_label, _pos, randVec3(2.0f), _smul, g_popupCols[_type]);
	m_popups.push_back(temp);
}

void universe::addDamagePopup(int _dmg, aiTeam _team, vec3 _pos, vec3 _vel)
{
	popup_type p = POPUP_GOOD;
	if(friendshipCheck(_team, TEAM_PLAYER)) p = POPUP_BAD;
	else if(neutralityCheck(_team, TEAM_PLAYER)) p = POPUP_NEUTRAL;

	addPopup(std::to_string(_dmg), p, 2.0f, _pos, _vel);
}

void universe::calcPowerBalance()
{
	m_balanceOfPower.assign(m_factions.size(), 0.0f);
	for(auto &i : m_agents.m_objects)
	{
		m_balanceOfPower[i.getTeam()] += calcAIPower(i.getClassification());
	}
	float total = 0.0f;
	for(auto &i : m_balanceOfPower) total += i;
	for(auto &i : m_balanceOfPower) i /= total;
}

selectionReturn universe::handleMouseDownInput(vec2 _mouse)
{
	//UI check
	selectionReturn ret = m_ui.handleInput(_mouse);
	if(ret.m_sel_val > -1)
	{
		m_sounds.playUISnd(MENU_SELECT_SND);
		return ret;
	}

	//Get mouse position.
	_mouse = toWorldSpace(_mouse );

	//Is the player moving an item in their inventory?
	m_ply.getCargo()->handleInput(_mouse, &m_selectedItems);

	enemy * contextShip = m_agents.getByID(m_contextShip);
	if(contextShip != nullptr)
	{

		//If the context ship exists and its inventory is open, test against it.
		if(!contextShip->getCargo()->handleInput(_mouse - tovec2(contextShip->getPos()), &m_selectedItems))
			m_contextShip = {0, -1};
	}
	else
	{
		//Context ship selection, if UI handle returns null.
		m_contextShip = {0, -1};
		for(size_t i = 0; i < m_agents.size(); ++i)
		{
			if(magns(_mouse - tovec2(m_agents[i].getPos())) < sqr(m_agents[i].getRadius() + 16.0f))
			{
				m_contextShip = m_agents.getID(i);
				break;
			}
		}
	}

	return ret;
}

void universe::processUIInput(selectionReturn _sel)
{
	std::cout << "processing " << _sel.m_sel_val << ", " << _sel.m_button_val << '\n';
	if(_sel.m_sel_val == 0)
	{
		m_ply.setEnergyPriority(_sel.m_button_val);
		m_ui.setDark(_sel.m_sel_val, true);
		m_ui.setDark(_sel.m_sel_val, _sel.m_button_val, false);
	}
	else if(_sel.m_sel_val == 1)
	{
		if( !upgradeCallback(_sel.m_sel_val, _sel.m_button_val) )
			return;
		else if(_sel.m_button_val == 4)
			m_ply.incrMissiles(1);
	}
	else if(_sel.m_sel_val == 2)
	{
		if( !upgradeCallback(_sel.m_sel_val, _sel.m_button_val) )
			return;

		if(_sel.m_button_val == 0) addMiner();
		else if(_sel.m_button_val == 1) addWingman();
		else if(_sel.m_button_val == 2) setMouseState( PLAYER_TURRET );
		else if(_sel.m_button_val == 3) setMouseState( PLAYER_GRAVWELL );
		else if(_sel.m_button_val == 4) setMouseState( PLAYER_BARRACKS );
		else if(_sel.m_button_val == 5) setMouseState( PLAYER_STATION );
		else if(_sel.m_button_val == 6) setMouseState( PLAYER_CAPITAL );
	}
	else if(_sel.m_sel_val == 3)
	{
		//Trade menu
		switch(_sel.m_button_val)
		{
		case 3:
		{
			std::cout << "inv toggle\n";
			enemy * con = m_agents.getByID( m_contextShip );
			if(con == nullptr)
				break;
			con->toggleInventory();
			break;
		}
		case 4:
			m_contextShip = {0, -1};
			break;
		default:
			break;
		}
	}
	else if(_sel.m_sel_val == 4)
	{
		switch(_sel.m_button_val)
		{
		case 0:
			setPause(false);
			escMenuTog();
			m_ui.pop();
			break;
		case 1:
			saveGame(this);
			setPause(false);
			escMenuTog();
			m_ui.pop();
			break;
		case 2:
			reload(true);
			m_factions.clear();
			loadGame(this);
			break;
		case 3:
			g_GAME_STATE = MODE_MENU;
			break;
		case 4:
			g_GAME_STATE = MODE_QUIT;
			break;
		default:
			break;

		}
	}
}

void universe::mouseUp()
{
	for(auto &i : m_selectedItems)
	{
		m_resources.push_back( i );
	}
	m_selectedItems.clear();
}

void universe::conductTrade(enemy &_buyer, enemy &_seller)
{
	//If the two ships are on the same team, no transaction takes place, we simply
	//transfer all items from the source to the target.
	std::cout << "trading!\n";
	if(sameTeam(_buyer.getTeam(), _seller.getTeam()))
	{
		std::cout << "transferring!\n";
		_seller.transferAllCargo(&_buyer);
		return;
	}

	//The items the buyer wants will be placed in this vector.
	std::vector<slot> itemsToBuy;

	float totalEconomy = m_factions[_buyer.getTeam()].getEconomicalStrength() +
			m_factions[_seller.getTeam()].getEconomicalStrength();
	float sellerRatio = m_factions[_seller.getTeam()].getEconomicalStrength() / totalEconomy;

	slotmap<debris> * src = _seller.getCargo()->getItems();
	for(size_t i = 0; i < src->size(); ++i)
	{
		resourceType type = src->m_objects[i].getResourceType();
		float buyerPrice = src->m_objects[i].calcCost(
					m_factions[_buyer.getTeam()].getResourceDemand(type)
				);
		float sellerPrice = src->m_objects[i].calcCost(
					m_factions[_seller.getTeam()].getResourceDemand(type)
				);

		//If the seller demands too much, do not buy.
		if(sellerPrice > buyerPrice) continue;
		//Else, compute a deal based on the economical strength of each faction.
		//Result is interpolated between the seller and buyer price.
		//Since the seller price is lower (given above condition) the seller will try to push price up to max.
		float price = sellerPrice + (buyerPrice - sellerPrice) * sellerRatio;

		m_factions[_buyer.getTeam()].addWealth( -price );
		itemsToBuy.push_back( src->getID(i) );
	}

	for(auto &i : itemsToBuy)
	{
		_seller.transferCargo(&_buyer, i);
	}
}

void universe::addFrag(slot _i)
{
	ship * killer = m_agents.getByID(_i);
	if(killer != nullptr) killer->addKill();
}

void universe::destroyAgent(size_t _i)
{
	enemy * lastAttacker = m_agents.getByID( m_agents[_i].getLastAttacker() );

	vec3 pos = {randNum(-16.0f,16.0f), randNum(-16.0f,16.0f), 0.0f};
	pos += m_agents[_i].getPos();
	addpfx(pos, m_agents[_i].getVel(), randNum(5.0, 7.0), m_agents[_i].getRadius() * randNum(0.5f, 0.8f), col1to255(m_agents[_i].getCurWeapCol()));

	//Dump inventory.
	for(auto &d : m_agents[_i].getCargo()->getItems()->m_objects)
	{
		addDebris(m_agents[_i].getPos(), m_agents[_i].getVel() + tovec3(randVec2(1.0f)), RESOURCE_IRON);
	}

	addScore( m_agents[_i].getScore() );
	addFrag( m_agents[_i].getLastAttacker() );

	float xp = clamp(calcAICost(m_agents[_i].getClassification()) * 0.005f, 0.0f, 5.0f);

	if(lastAttacker != nullptr) lastAttacker->addXP( xp );
	else if(m_agents[_i].getLastAttacker() == slot(0, -2))
	{
		g_KILL_TIME_SCALE = clamp(g_KILL_TIME_SCALE - m_ply.getXP() * 0.01f, 0.2f, 1.0f);
		m_ply.addXP(xp);
	}

	playSnd(EXPLOSION_SND, m_agents[_i].getPos(), 1.0f);
	m_drawer.addShake(10000.0f / (1.0f + mag(m_agents[_i].getPos() - m_ply.getPos())));

	if(lastAttacker != nullptr and lastAttacker->getTeam() == TEAM_PLAYER)
	{
		m_factions[ lastAttacker->getTeam() ].addWealth( calcAICost(m_agents[_i].getClassification() ) );
		if(lastAttacker->getTeam() == TEAM_PLAYER)
		{
			addPopup( getRandomEntry(&g_fragRemarks), POPUP_NEUTRAL, 4.0f, lastAttacker->getPos(), -m_vel + randVec3(2.0f) );
			m_factions[m_agents[_i].getTeam()].addAggression(1.0f);
		}
	}
	m_factions[ m_agents[_i].getTeam() ].unitDestroyed(m_agents[_i].getClassification());

	if(m_agents.getByID(m_contextShip) == &m_agents[_i]) m_contextShip = {0, -1};
	m_agents.free(_i);
}
