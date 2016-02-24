#ifndef CLASS_UNIVERSE
#define CLASS_UNIVERSE

#include "universe.hpp"

bool emnityCheck(ai_team a, ai_team b);

universe::universe(): ply( {0.0f, 0.0f} )
{
  SDL_Surface * temp = IMG_Load("../resources/environment/background/sky.jpg");
	sky = SDL_CreateTextureFromSurface(renderer, temp);
	SDL_FreeSurface(temp);
	
	setVel({0,0});
	
	max_enemies_count = 3;
	max_wingmen_count = 0;
	max_miner_count = 0;
	enemy_count = 0;
	wingmen_count = 0;
	miner_count = 0;
	
	ply.setPos({WIN_WIDTH/2.0f,WIN_HEIGHT/2.0f});
	ply.setPPos({WIN_WIDTH/2.0f,WIN_HEIGHT/2.0f});
	
	tColP[0] = 40.0f;
	tColP[1] = 90.0f;
	tColP[2] = 100.0f;
	
	cColP[0] = 40.0f;
	cColP[1] = 90.0f;
	cColP[2] = 100.0f;
	
	for(int i = 0; i < BACKGROUND_DOTS; i++)
	{
		dots.push_back(stardust(cColP));
	}
	
	vector<SDL_Surface*> surfs;
	for(size_t i = 0; i < 5; i++)
	{
		string name = "../resources/images/environment/stardust_" + to_string(i);
		name += ".png";
	
		surfs.push_back(IMG_Load(name.c_str()));
	}
	
	vector<SDL_Texture*> texs;
	
	for(size_t i = 0; i < surfs.size(); i++)
	{
		texs.push_back(SDL_CreateTextureFromSurface(renderer, surfs.at(i)));
		SDL_FreeSurface(surfs.at(i));
	}
	surfs.clear();
	
	for(int i = 0; i < BACKGROUND_DOTS; i++)
	{
		sparkles.push_back(stardust_sprite(texs.at(rand()%texs.size()), cColP));
	}
	
	score = 0;
	paused = false;
	
	//for(int i = 0; i < 3; ++i) spawnShip(TEAM_PLAYER_MINER);
	//addStation();
}

void universe::addShot(vec2 p, vec2 v, float angle, int weap, ai_team team)
{
	int temp_angle = angle + 90;
	for(int i = 0; i < weapons[weap][0]; i++)
	{
		//vec2 vec = computeVector(temp_angle);
		laser temp( p + v, v, temp_angle, weapons[weap], team);
		shots.push_back(temp);
	}
}

void universe::addMissile(vec2 p, vec2 v, float angle, bool team)
{
	missile m(p);
	m.setVel(v + computeVector(angle + 90) * 5);
	m.setWVel(vel);
	m.setAng(angle);
	
	int mx = 0, my = 0;
	SDL_GetMouseState(&mx,&my);
	mx -= HALFWIN.x;
	my -= HALFWIN.y;
	
	if(team) m.setTarget(closestEnemy({ (static_cast<float>(mx) - HALFWIN.x) / ZOOM_LEVEL, (static_cast<float>(my) - HALFWIN.y) / ZOOM_LEVEL}, TEAM_PLAYER));
	else m.setTarget(getPly());
	
	if(m.getTarget() == nullptr) return;
	
	if(team) getPly()->incrMissiles(-1);
	
	missiles.push_back(m);
}

void universe::update(int loopCount, float dt)
{
	sim_time profiler(0.0f);	
	/*if(DEV_MODE)
	{
		cout << "Update Loop Profiling Commence" << endl;
		profiler.setCur();
	}*/
	
	if(paused) return;
	
	cColP[0] += clamp(tColP[0] - cColP[0], -1.0f, 1.0f);
	cColP[1] += clamp(tColP[1] - cColP[1], -1.0f, 1.0f);
	cColP[2] += clamp(tColP[2] - cColP[2], -1.0f, 1.0f);
	
	if(rand()%10000 == 0)
	{
		float p0 = randFloat(0.0f, 100.0f);
		float p1 = randFloat(0.0f, 100.0f) + p0;
		float p2 = randFloat(0.0f, 100.0f) + p0 + p1;
		float total = p0 + p1 + p2;
		
		tColP[0] = ( p0 / total ) * 250.0f;
		tColP[1] = ( p1 / total ) * 250.0f;
		tColP[2] = ( p2 / total ) * 250.0f;
	}
	
	if(rand()%10000 == 0) BG_DENSITY = randFloat(1.0f,10.0f);
	if(rand()%10000 == 0) gameplay_intensity = randFloat(0.0f, 2.2f);
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Universe updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	ply.ctrlUpdate();
	ply.update(dt);
	
	if(ply.getHealth() < ply.getMaxHealth()) addParticleSprite(ply.getPos(), ply.getVel(), ply.getHealth() / ply.getMaxHealth(), SMOKE_TEXTURE);
	
	if(ply.isFiring() and ply.getCooldown() <= 0.0f and ply.getEnergy() > getEnergyCost(ply.getWeap()))
	{
		playSnd( ply.getWeap() );
		addShot( ply.getPos() - ply.getVel(), ply.getVel(), ply.getAng(), ply.getWeap(), TEAM_PLAYER );
		ply.setEnergy( ply.getEnergy() - getEnergyCost(ply.getWeap()) );
		ply.setCooldown( getCooldown(ply.getWeap()) );
	}
	
	if(!DEV_MODE and ply.getHealth() <= 0.0f and !GAME_OVER)
	{		
		for(int p = 0; p < rand()%5 + 10; p++)
		{
			vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
			pos += ply.getPos();
			addpfx(pos, ply.getVel(), vel, rand()%50 + 20, rand()%50 + 8);
		}
		ply.setMaxHealth(0, true);
		ply.setMaxEnergy(0, true);
		ply.setMaxShield(0, true);
		playSnd(3);
		
		GAME_OVER = true;
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Player updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	for(unsigned int i = 0; i < dots.size(); i++)
	{
		dots.at(i).setWVel(vel);
		dots.at(i).updatePos(dt);
		if(isOffScreen(dots.at(i).getPos(), MAX_DIM * BG_DENSITY / ZOOM_LEVEL))
		{
			dots.at(i).gen(true, cColP);
		}
	}	
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Stardust updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	for(unsigned int i = 0; i < sparkles.size(); i++)
	{
		sparkles.at(i).setWVel(vel);
		sparkles.at(i).updateSprite(dt);
		
		SDL_Texture * temp = sparkles.at(i).getTex();
		int w = 0;
		int h = 0;
		SDL_QueryTexture(temp, NULL, NULL, &w, &h);
		
		if(isOffScreen(sparkles.at(i).getPos(), (MAX_DIM + max(w, h)) * BG_DENSITY * sparkles.at(i).getZ() / ZOOM_LEVEL))
		{
			sparkles.at(i).spriteGen(cColP);
		}
	}	
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Sprite updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	for(int i = shots.size() - 1; i >= 0; i--)
	{	
		if(shots.at(i).getPower() < 0.0f)
		{
			swapnpop(&shots, i);
		}
		else
		{
			shots.at(i).setWVel(vel);
			shots.at(i).update(dt);
		}
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Shot updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	partitions.rects.clear();
	partitions.ships.clear();
	partitions.lasers.clear();
	partitions.rocks.clear();
	partitions.rockets.clear();
	vector<enemy*> init_ship;
	vector<laser*> init_laser;
	vector<missile*> init_missile;
	vector<ship*> init_asteroid;
	SDL_Rect init_rect = {0, 0, 0, 0};
	int minX = I_INF, minY = I_INF, maxX = -I_INF, maxY = -I_INF;
	for(size_t i = 0; i < enemies.size(); ++i)
	{
		vec2 p = enemies.at(i).getPos();
		if(p.x < minX) minX = p.x;
		if(p.y < minY) minY = p.y;
		if(p.x > maxX) maxX = p.x;
		if(p.y > maxY) maxY = p.y;
		init_ship.push_back( &enemies.at(i) );
	}
	for(size_t i = 0; i < missiles.size(); ++i)
	{
		vec2 p = missiles.at(i).getPos();
		if(p.x < minX) minX = p.x;
		if(p.y < minY) minY = p.y;
		if(p.x > maxX) maxX = p.x;
		if(p.y > maxY) maxY = p.y;
		init_missile.push_back( &missiles.at(i) );
	}
	for(size_t i = 0; i < asteroids.size(); ++i)
	{
		vec2 p = asteroids.at(i).getPos();
		if(p.x < minX) minX = p.x;
		if(p.y < minY) minY = p.y;
		if(p.x > maxX) maxX = p.x;
		if(p.y > maxY) maxY = p.y;
		init_asteroid.push_back( &asteroids.at(i) );
	}
	for(size_t i = 0; i < shots.size(); ++i)
	{
		vec2 p = shots.at(i).getPos();
		vec2 pp = p + shots.at(i).getVel();
		
		if(min(p.x, pp.x) < minX) minX = p.x;
		if(min(p.y, pp.y) < minY) minY = p.y;
		if(max(p.x, pp.x) > maxX) maxX = p.x;
		if(max(p.y, pp.y) > maxY) maxY = p.y;
		init_laser.push_back( &shots.at(i) );
	}
	if(ply.getPos().x < minX) minX = ply.getPos().x;
	if(ply.getPos().y < minY) minY = ply.getPos().y;
	if(ply.getPos().x > maxX) maxX = ply.getPos().x;
	if(ply.getPos().y > maxY) maxY = ply.getPos().y;
	
	init_rect.x = minX;
	init_rect.y = minY;
	init_rect.w = maxX - minX;
	init_rect.h = maxY - minY;

	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Quadtree parameter generation complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	detectCollisions(init_rect, init_ship, init_laser, init_missile, init_asteroid, 0);
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Collision detection complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	checkCollisions();	
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Collision resolution complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/

	for(int i = missiles.size() - 1; i >= 0; i--)
	{
		missiles.at(i).updatePos(dt);
		bool ofscr = isOffScreen(missiles.at(i).getPos(), 30000.0f);
		if(ofscr or missiles.at(i).getHealth() <= 0 or missiles.at(i).detonate())
		{
			if(!ofscr)
			{
				for(int p = 0; p < rand()%2 + 4; p++)
				{
					vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
					pos += missiles.at(i).getPos();
					addpfx(pos, missiles.at(i).getVel(), vel, rand()%50 + 50, rand()%50 + 8);
				}
				
				for(size_t j = 0; j < enemies.size(); j++)
				{
					vec2 ep = enemies.at(j).getPos();
					vec2 mp = missiles.at(i).getPos();
					
					float dmg = magns(mp-ep);
					
					if(dmg > 300*300) continue;
					
					dmg = 1/mag(mp-ep) * 30000;
					enemies.at(j).damage(dmg);			
				}
				
				vec2 pdiff = missiles.at(i).getPos() - ply.getPos();
				if(magns(pdiff) < 300*300) ply.damage(1 / mag(pdiff) * 30000);
			}
			swapnpop(&missiles, i);
		}
		else
		{		
			missiles.at(i).setWVel(vel);
			missiles.at(i).update(dt);
			missiles.at(i).steering();
		}
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Missile updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	//cout << "pre" << endl;
	for(int i = asteroids.size() - 1; i >= 0; i--)
	{
		asteroids.at(i).updatePos(dt);
		vec2 p = asteroids.at(i).getPos();
		if((isOffScreen(p,30000.0f) or asteroids.at(i).getHealth() <= 0.0f))
		{
			if(asteroids.at(i).getHealth() <= 0.0f)
			{		
				for(int fx = 0; fx < rand() % 5 + 1; fx++)
				{
					vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
					pos += asteroids.at(i).getPos();
					addpfx(pos, asteroids.at(i).getVel(), vel, rand()%20 + 50, rand()%30 + 2);
					for(int q = 0; q < 50; ++q) addParticleSprite(pos, asteroids.at(i).getVel() + vel + randVec(6.0f), 0.0f, SMOKE_TEXTURE);
				}
				if(asteroids.at(i).getClassification() == ASTEROID_SMALL) score += 10;
				else
				{
					for(int q = 0; q < 4; ++q)
					{
						ship a(GLOBAL_SHIPS.at( static_cast<int>(asteroids.at(i).getClassification()) - 1 ), p + randVec( asteroids.at(i).getRadius() ));
						a.setVel( asteroids.at(i).getVel() + randVec(1.0f) );
						a.update(dt);
						asteroids.push_back(a);
					}
				}
				playSnd(3);
			}
			swapnpop(&asteroids, i);
		}
		else
		{		
			asteroids.at(i).setWVel(vel);
			asteroids.at(i).update(dt);
		}	
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Asteroid updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	for(int i = enemies.size() - 1; i >= 0; i--)
	{	
		enemies.at(i).updatePos(dt);
		vec2 p = enemies.at(i).getPos();
		
		//Offscreen elimination, health-based elimination
		if((isOffScreen(p,30000.0f) and enemies.at(i).getTeam() != TEAM_PLAYER and enemies.at(i).getTeam() != TEAM_PLAYER_MINER) or enemies.at(i).getHealth() <= 0)
		{		
			if(enemies.at(i).getHealth() <= 0.0f)
			{
				if(enemies.at(i).getTeam() == TEAM_PLAYER) max_wingmen_count--;
				for(int p = 0; p < rand() % 5 + 1; p++)
				{
					vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
					pos += enemies.at(i).getPos();
					addpfx(pos, enemies.at(i).getVel(), vel, rand()%20 + 50, rand()%30 + 2);
				}
				score += enemies.at(i).getScore();
				if( rand() % 8 <= DIFFICULTY ) max_enemies_count += DIFFICULTY + 1;
				playSnd(3);
			}
			if(enemies.at(i).getTeam() == GALACTIC_FEDERATION) enemy_count--;
			else if(enemies.at(i).getTeam() == TEAM_PLAYER) wingmen_count--;

			swapnpop(&enemies, i);
			//enemies.erase(enemies.begin() + i);
		}
		else
		{
			if(enemies.at(i).getHealth() < enemies.at(i).getConfidence()) enemies.at(i).setEnergyPriority(2);
			else if(enemies.at(i).getHealth() < enemies.at(i).getMaxHealth() * 0.75f) enemies.at(i).setEnergyPriority(1);
			else enemies.at(i).setEnergyPriority(0);
			
			enemies.at(i).setWVel(vel);
			enemies.at(i).update(dt);
			
			float minDist = F_MAX;
			enemies.at(i).setTarget(nullptr);
			
			if(enemies.at(i).getClassification() == PLAYER_MINER_DROID) //Set miner targets
			{
				for(size_t k = 0; k < asteroids.size(); ++k)
				{
					enemy * a = &enemies.at(i);
					ship * b = &asteroids.at(k);
					
					float nd = magns(a->getPos() - b->getPos());
					if(nd < minDist)
					{
						enemies.at(i).setTarget( b );
						enemies.at(i).setGoal(GOAL_ATTACK);
						minDist = nd;
					} 
				}
			}
			else if(enemies.at(i).getClassification() == PLAYER_GRAVWELL) //Gravwell attraction
			{			
				for(size_t k = 0; k < asteroids.size(); ++k)
				{
					vec2 incr = enemies.at(i).getPos() - asteroids.at(k).getPos();
					float dist = fabs( magns( incr ) );
					dist /= sqr( 32.0f );

					if(dist > 10000.0f) asteroids.at(k).addVel( incr / dist );
					else asteroids.at(k).addVel( -asteroids.at(k).getVel() * 0.0125f );
				}
			}
			else if(enemies.at(i).getClassification() == PLAYER_BARRACKS)
			{
				if(rand() % 2048 == 0 and wingmen_count < 20) spawnShip(TEAM_PLAYER, enemies.at(i).getPos());
			}
			else if(!enemies.at(i).isLaserless()) //Default target acquisition
			{	
				for(size_t k = 0; k < enemies.size(); ++k)
				{
					enemy * a = &enemies.at(i);
					enemy * b = &enemies.at(k);
					
					if(a == b) continue;

					float nd = magns(a->getPos() - b->getPos());
					if(nd < minDist and emnityCheck(a->getTeam(), b->getTeam()))
					{
						enemies.at(i).setTarget( (enemy*)b );
						if(enemies.at(i).getClassification() != PLAYER_TURRET) enemies.at(i).setGoal(GOAL_ATTACK);
						else enemies.at(i).setGoal(GOAL_TURRET);
						minDist = nd;
					} 
				}
			}
			
			//Setting the follow distances of the different units.  
			float fd = 0.0f;
			if(enemies.at(i).getTeam() == TEAM_PLAYER) fd = 1500.0f;
			else if(enemies.at(i).getTeam() == TEAM_PLAYER_MINER) fd = 20000.0f;
			
			float nd = magns(ply.getPos() - enemies.at(i).getPos());
			if(emnityCheck( enemies.at(i).getTeam(), TEAM_PLAYER ) and nd < minDist )
			{
				enemies.at(i).setTarget( (player*)&ply );
				enemies.at(i).setGoal(GOAL_ATTACK);
				minDist = nd;
			}
			else if(!enemies.at(i).isStatic() and !emnityCheck( enemies.at(i).getTeam(), TEAM_PLAYER ) and ( nd > fd * fd or enemies.at(i).getTarget() == nullptr ) and !enemies.at(i).inCombat())
			{
				enemies.at(i).setTarget( (player*)&ply );
				enemies.at(i).setGoal( GOAL_CONGREGATE );
			}
			else if( enemies.at(i).getTarget() == nullptr )
			{
				//if(enemies.at(i).getClassification() == PLAYER_TURRET) cout << "PLAYER_TURRET (" << enemies.at(i).isStatic() << " " << !enemies.at(i).isLaserless() << ") " << (enemies.at(i).getTarget() == nullptr) << endl;
				enemies.at(i).setGoal( GOAL_IDLE );
			}
			
			if(emnityCheck( enemies.at(i).getTeam(), TEAM_PLAYER ) and enemies.at(i).getHealth() < enemies.at(i).getConfidence() and !enemies.at(i).isStatic())
			{
				enemies.at(i).setGoal(GOAL_FLEE);
			}
			
			enemies.at(i).behvrUpdate();
			enemies.at(i).steering();
			
			if(enemies.at(i).isFiring() and enemies.at(i).getCooldown() <= 0)
			{
				addShot(enemies.at(i).getPos() - enemies.at(i).getVel(), enemies.at(i).getVel(), enemies.at(i).getAng(), enemies.at(i).getWeap(), enemies.at(i).getTeam());
				enemies.at(i).setCooldown( getCooldown(enemies.at(i).getWeap()) );
				enemies.at(i).setFiring(false);
			}
			
			if(enemies.at(i).getHealth() < enemies.at(i).getMaxHealth()) addParticleSprite(p, enemies.at(i).getVel(), enemies.at(i).getHealth() / enemies.at(i).getMaxHealth(), SMOKE_TEXTURE);
		}
	}	
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Enemy updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	if(!GAME_OVER)
	{
		if(rand() % 45000 <= DIFFICULTY * gameplay_intensity and enemy_count < clamp(max_enemies_count,0,200))
		{
			int reps = clamp(rand() % (DIFFICULTY * 5) + 1, 1, clamp(max_enemies_count,0,80) - enemy_count);
			
			ai_team pteam;
			if(rand() % 100 < 70) pteam = SPOOKY_SPACE_PIRATES;
			else pteam = GALACTIC_FEDERATION;
			
			for(int i = 0; i < reps; i++)
			{
				spawnShip(pteam);
			}
		}
		if(rand() % 256 == 0 and wingmen_count < max_wingmen_count)
		{
			spawnShip(TEAM_PLAYER);
		}
		if(rand() % 256 == 0 and miner_count < max_miner_count)
		{
			spawnShip(TEAM_PLAYER_MINER);
		}
		if(rand() % 1024 == 0 and asteroids.size() < 10)
		{
			ship_spec size = ASTEROID_MID;
			int prob = rand() % 100;
			
			if(prob > 50 and prob <= 80) size = ASTEROID_SMALL;
			else if(prob > 80 and prob <= 99) size = ASTEROID_LARGE;
			
			ship a({0.0f, 0.0f}, size);
			int side = rand() %4 ;
			if(side == 0) a.setPos({randFloat(-20000.0f,20000.0f), -20000.0f});
			else if(side == 1) a.setPos({randFloat(-20000.0f,20000.0f), 20000.0f});
			else if(side == 2) a.setPos({-20000.0f, randFloat(-20000.0f,20000.0f)});
			else if(side == 3) a.setPos({20000.0f, randFloat(-20000.0f,20000.0f)});
			a.setVel( randVec(64.0f) );
			a.update(dt);
			asteroids.push_back(a);
		}
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Spawn cycles complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	for(int i = particles.size() - 1; i >= 0; i--)
	{
		particles.at(i).setWVel(vel);
		particles.at(i).update(dt);
		if(particles.at(i).done()) 
		{
			swapnpop(&particles, i);
			//particles.at(i).clear();
			//particles.erase(particles.begin() + i);
		}
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Particle updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
	
	for(int i = passive_sprites.size() - 1; i >= 0; --i)
	{
		float alph = passive_sprites.at(i).getCol(3);
		
		passive_sprites.at(i).setWVel(vel);
		passive_sprites.at(i).updateSprite(dt);
		
		SDL_Texture * temp = passive_sprites.at(i).getTex();
		int w = 0;
		int h = 0;
		SDL_QueryTexture(temp, NULL, NULL, &w, &h);
		
		if(alph < 0.0f or isOffScreen(passive_sprites.at(i).getPos(), (MAX_DIM + max(w, h)) * BG_DENSITY * passive_sprites.at(i).getZ() / ZOOM_LEVEL) or passive_sprites.at(i).getDim() <= 0.0f)
		{
			swapnpop(&passive_sprites, i);
			//passive_sprites.erase(passive_sprites.begin() + i);
			continue;
		}
		alph *= 0.94f;
		alph -= 0.1f;
		passive_sprites.at(i).setCol(3, alph);
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Particle sprite updates complete: " << profiler.getDiff() << " seconds." << endl;
		profiler.setCur();
	}*/
}

void universe::draw(float dt)
{	
	if(paused) dt = 0.0f;
	
	sim_time profiler(0.0f);	
	/*if(DEV_MODE)
	{
		cout << "Draw Loop Profiling Commence" << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < dots.size(); i++)
	{	
		if(dots.at(i).getZ() > 1) continue;
		dots.at(i).draw(dt);
	}

	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Stardust lower draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < sparkles.size(); i++)
	{	
		if(sparkles.at(i).getZ() <= 1) sparkles.at(i).draw(dt);;
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Sprite lower draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < passive_sprites.size(); i++)
	{	
		if(!paused) passive_sprites.at(i).incrDim();
		passive_sprites.at(i).drawDim(dt);
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Particle sprite draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < shots.size(); i++)
	{	
		shots.at(i).draw(dt);
		
		/*if(DEV_MODE)
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			vec2 pos = shots.at(i).getPos();
			vec2 vel = shots.at(i).getVel();
			SDL_RenderDrawLine( renderer, static_cast<int>(pos.x), static_cast<int>(pos.y), static_cast<int>(pos.x + vel.x), static_cast<int>(pos.y + vel.y) );
		}*/
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Laser draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < asteroids.size(); i++)
	{
		asteroids.at(i).draw(dt);
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Asteroids draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < enemies.size(); i++)
	{
		enemies.at(i).draw(dt);
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Enemy draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	ply.draw(dt);
	
	for(size_t i = 0; i < missiles.size(); i++)
	{	
		missiles.at(i).draw(dt);
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Missiles draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < particles.size(); i++)
	{
		particles.at(i).draw(dt);		
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Particles draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < dots.size(); i++)
	{	
		if(dots.at(i).getZ() <= 1) continue;
		dots.at(i).draw(dt);
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Dots lower draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	for(size_t i = 0; i < sparkles.size(); i++)
	{	
		if(sparkles.at(i).getZ() > 1) 
		{
			sparkles.at(i).draw(dt);
		}
	}
	
	/*if(DEV_MODE)
	{
		profiler.setCur();
		cout << std::fixed << "	Sprite upper draw complete: " << profiler.getDiff() << endl;
		profiler.setCur();
	}*/
	
	if(DEV_MODE)
	{
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		for(size_t i = 0; i < partitions.rects.size(); ++i)
		{
			SDL_Rect pr = partitions.rects.at(i);
			pr.x *= ZOOM_LEVEL;
			pr.y *= ZOOM_LEVEL;
			pr.w *= ZOOM_LEVEL;
			pr.h *= ZOOM_LEVEL;
			
			pr.x += HALFWIN.x;
			pr.y += HALFWIN.y;
			
			SDL_RenderDrawRect( renderer, &pr );
		}
	}
}

void universe::detectCollisions(SDL_Rect box, vector<enemy*> ships, vector<laser*> lasers, vector<missile*> rockets, vector<ship*> rocks, unsigned short int lvl)
{
	//cout << lvl << ") SIZES : ships " << ships.size() << ", lasers " << lasers.size() << ", missiles " << rockets.size() << ", rocks " << rocks.size() << endl;
	size_t count = 0;
	vector<enemy*> pships;
	vector<laser*> plasers;
	vector<missile*> prockets;
	vector<ship*> procks;
	
	for(size_t i = 0; i < ships.size(); ++i)
	{
		enemy * s = ships.at(i);
		SDL_Rect r = {0,0,0,0};
		if( SDL_IntersectRect(&box, s->getRekt(), &r) )
		{
			pships.push_back(s);
			count++;
		}
	}
	
	for(size_t i = 0; i < rockets.size(); ++i)
	{
		missile * r = rockets.at(i);
		SDL_Rect b = {0,0,0,0};
		if( SDL_IntersectRect(&box, r->getRekt(), &b) )
		{
			prockets.push_back(r);
			count++;
		}
	}
	
	for(size_t i = 0; i < rocks.size(); ++i)
	{
		ship * r = rocks.at(i);
		SDL_Rect b = {0,0,0,0};
		if( SDL_IntersectRect(&box, r->getRekt(), &b) )
		{
			procks.push_back(r);
			count++;
		}
	}
	
	for(size_t i = 0; i < lasers.size(); ++i)
	{
		laser * l = lasers.at(i);
		vec2 p = l->getPos();
		vec2 v = p + l->getVel();
		
		int sx = p.x, sy = p.y, ex = v.x, ey = v.y;
		if( SDL_IntersectRectAndLine(&box, &sx, &sy, &ex, &ey ) )
		{
			plasers.push_back(l);
			count++;
		}
	}
	
	if(count < 20 or lvl > 4 or lasers.size() == 0 or ( ships.size() == 0 and rockets.size() == 0 and rocks.size() == 0 ))
	{
		partitions.ships.push_back(pships);
		partitions.lasers.push_back(plasers);
		partitions.rockets.push_back(prockets);
		partitions.rocks.push_back(procks);
		partitions.rects.push_back(box);
	}
	else
	{
		int x = box.x, y = box.y, w = box.w / 2, h = box.h / 2;
		SDL_Rect r1 = {x, 		y, 		w, 		h};
		SDL_Rect r2 = {x + w, 	y, 		w, 		h};
		SDL_Rect r3 = {x, 		y + h, 	w, 		h};
		SDL_Rect r4 = {x + w, 	y + h, 	w, 		h};
		
		detectCollisions(r1, pships, plasers, prockets, procks, lvl + 1);
		detectCollisions(r2, pships, plasers, prockets, procks, lvl + 1);
		detectCollisions(r3, pships, plasers, prockets, procks, lvl + 1);
		detectCollisions(r4, pships, plasers, prockets, procks, lvl + 1);
	}
}

void universe::checkCollisions()
{
	for(size_t p = 0; p < partitions.ships.size(); ++p)
	{
		bool done = false;
		int harm = 0;
		for(int l = partitions.lasers.at(p).size() - 1; l >= 0; l--)
		{
			vec2 sp = partitions.lasers.at(p).at(l)->getPos();
			vec2 sv = partitions.lasers.at(p).at(l)->getVel();
			vec2 spv = sp + sv;
      float stop = partitions.lasers.at(p).at(l)->getStop();
			
			vec2 ep;
			vec2 ev;
			float er;
			
			//cout << "ENEMY CHECK" << endl;	
			for(int s = partitions.ships.at(p).size() - 1; s >= 0; s--)
			{
				if(partitions.lasers.at(p).at(l)->getTeam() == TEAM_PLAYER_MINER or !emnityCheck(partitions.lasers.at(p).at(l)->getTeam(), partitions.ships.at(p).at(s)->getTeam())) continue;
				harm = 0;
				
				ep = partitions.ships.at(p).at(s)->getPos();
				ev = partitions.ships.at(p).at(s)->getVel();	
				er = partitions.ships.at(p).at(s)->getRadius();
				
				int sx = sp.x, sy = sp.y, ex = spv.x, ey = spv.y;
				if(SDL_IntersectRectAndLine(partitions.ships.at(p).at(s)->getRekt(),&sx,&sy,&ex,&ey))
				{
					addpfx(ep + randVec(er), ev, vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
					harm = partitions.lasers.at(p).at(l)->getDmg();
					
					for(int i = shots.size() - 1; i >= 0; --i) if(&shots.at(i) == partitions.lasers.at(p).at(l)) swapnpop(&shots, i);//shots.erase(shots.begin() + i);
					swapnpop(&partitions.lasers.at(p), l);
					
					done = true;
				}
				if(harm > 0)
				{
					partitions.ships.at(p).at(s)->damage(harm);
          partitions.ships.at(p).at(s)->addVelS( sv * stop );
					break;
				}
			}
			if(done) continue;
			
			for(int r = partitions.rocks.at(p).size() - 1; r >= 0; r--)
			{
				//cout << "R is " << r << ") " << partitions.rocks.at(p).at(r) << endl;
				harm = 0;
				
				ep = partitions.rocks.at(p).at(r)->getPos();
				ev = partitions.rocks.at(p).at(r)->getVel();
				er = partitions.rocks.at(p).at(r)->getRadius();	
				
				int sx = sp.x, sy = sp.y, ex = spv.x, ey = spv.y;
				if(SDL_IntersectRectAndLine(partitions.rocks.at(p).at(r)->getRekt(),&sx,&sy,&ex,&ey))
				{
					addpfx(ep + randVec(er), ev, vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
					for(int q = 0; q < 20; ++q) addParticleSprite(ep, ev + randVec(6.0f), 0.0f, SMOKE_TEXTURE);
					harm = partitions.lasers.at(p).at(l)->getDmg();
					
					for(int i = shots.size() - 1; i >= 0; --i) if(&shots.at(i) == partitions.lasers.at(p).at(l)) swapnpop(&shots, i);//shots.erase(shots.begin() + i);
					swapnpop(&partitions.lasers.at(p), l);
					
					done = true;
				}
				if(harm > 0)
				{
					partitions.rocks.at(p).at(r)->damage(harm);
          partitions.rocks.at(p).at(r)->addVel( sv * stop );
					break;
				}
			}
			if(done) continue;
			
			//cout << "ENEMY CHECK" << endl;	
			for(int m = partitions.rockets.at(p).size() - 1; m >= 0; m--)
			{
				if(partitions.lasers.at(p).at(l)->getTeam() == TEAM_PLAYER_MINER) continue;
				harm = 0;
				
				ep = partitions.rockets.at(p).at(m)->getPos();
				ev = partitions.rockets.at(p).at(m)->getVel();
				er = partitions.rockets.at(p).at(m)->getRadius();	
								
				int sx = sp.x, sy = sp.y, ex = spv.x, ey = spv.y;
				
				if(SDL_IntersectRectAndLine(partitions.rockets.at(p).at(m)->getRekt(),&sx,&sy,&ex,&ey))
				{
					addpfx(ep + randVec(er), ev, vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
					
					harm = shots.at(l).getDmg();
					
					for(int i = shots.size() - 1; i >= 0; --i) if(&shots.at(i) == partitions.lasers.at(p).at(l)) shots.erase(shots.begin() + i);
					swapnpop(&partitions.lasers.at(p), l);
					
					done = true;
				}
				if(harm > 0) 
				{
					partitions.rockets.at(p).at(m)->damage(harm);
          partitions.rockets.at(p).at(m)->addVel( sv * stop );
					break;
				}
			}
			if(done) continue;
			
			if(partitions.lasers.at(p).at(l)->getTeam() == TEAM_PLAYER_MINER) continue;
			
			harm = 0;
			
			vec2 dp = sp - ply.getPos(); 
			vec2 dv = sv + ply.getVel();
			if(fabs(dv.x) > fabs(dp.x) - 32 and fabs(dv.y) > fabs(dp.y) - 32 and partitions.lasers.at(p).at(l)->getTeam() != TEAM_PLAYER)
			{
				vec2 spv = sp + sv;
				int sx = (int)sp.x, sy = (int)sp.y, ex = (int)spv.x, ey = (int)spv.y;
				if(SDL_IntersectRectAndLine(ply.getRekt(),&sx,&sy,&ex,&ey))
				{
					playSnd(4);
					addpfx(sp + randVec(32.0f), ply.getVel(), vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
					harm = partitions.lasers.at(p).at(l)->getDmg();
					
					for(int i = shots.size() - 1; i >= 0; --i) if(&shots.at(i) == partitions.lasers.at(p).at(l)) swapnpop(&shots, i);
					swapnpop(&partitions.lasers.at(p), l);
					
					done = true;
				}
        if(harm > 0)
        {
          ply.damage(harm);
          ply.addVel( sv * stop );
        }
			}
			
			//Space-station healing check.
			for(size_t i = 0; i < partitions.ships.at(p).size(); ++i)
			{
				enemy * a = partitions.ships.at(p).at(i);
				for(size_t j = 0; j < partitions.ships.at(p).size(); ++j)
				{
					enemy * b = partitions.ships.at(p).at(j);
					
					//If a is a station, and b the two ships are on the same team...
					if( a->getClassification() == PLAYER_STATION and !emnityCheck( a->getTeam(), b->getTeam() ) and a != b )
					{
						float dist = magns( b->getPos() - a->getPos() );
						if(dist < 160000.0f) b->incrHealth(0.01f);
					}
				}
				
				if( a->getClassification() == PLAYER_STATION and !emnityCheck( a->getTeam(), TEAM_PLAYER ) )
				{
					float dist = magns( ply.getPos() - a->getPos() );
					if(dist < 160000.0f) ply.incrHealth(0.01f);
				}
			}
		}
	}
}

void universe::addpfx(vec2 p, vec2 v, vec2 wv, int no, float f)
{
	pfx pf(p,v,wv,no,f,XPLO_TEXT);
	particles.push_back(pf);
}

ship * universe::closestEnemy(vec2 p, ai_team t)
{
	float best_dist = F_MAX;
	ship * r = nullptr;
	for(size_t i = 0; i < enemies.size(); i++)
	{
		if(!emnityCheck(enemies.at(i).getTeam(), t)) continue;
		vec2 pe = enemies.at(i).getPos();
		float dist = magns(pe-p);

		if(dist < best_dist)
		{
			best_dist = dist;
			r = enemies.at(i).getSelf();
		}
	}
	return r;
}

void universe::addParticleSprite(vec2 p, vec2 v, float m, SDL_Texture * tex)
{
	float col;
	if( tex == SMOKE_TEXTURE ) col = randFloat(200.0f, 220.0f) * (1.0f - m);
	else if( tex == XPLO_TEXT ) col = 255.0f;
	
	stardust_sprite sm (tex, col);
	sm.setPos(p);
	if( tex == SMOKE_TEXTURE ) sm.setVel(v + randVec(1.0f));
	else if( tex == XPLO_TEXT ) sm.setVel(v);
	sm.setZ(1.0f);
	passive_sprites.push_back(sm);
}

void universe::spawnShip(ai_team t)
{	
	int side = rand()%4;
	vec2 pass;
		
	if(side == 0) pass = {randFloat(-20000.0f,20000.0f), -20000.0f};
	else if(side == 1) pass = {-20000.0f,randFloat(-20000.0f, 20000.0f)};
	else if(side == 2) pass = {randFloat(-20000.0f, 20000.0f), 20000.0f};
	else if(side == 3) pass = {20000.0f, randFloat(-20000.0f, 20000.0f)};
	
	spawnShip(t, pass);
}

void universe::spawnShip(ai_team t, vec2 p)
{	
	int prob = rand()%100;
	
	ship_spec type;
	
	if( t == SPOOKY_SPACE_PIRATES )
	{
		type = PIRATE_GNAT;
		if(prob > 75 and prob <= 80) type = PIRATE_CRUISER;
		else if(prob > 80 and prob <= 90) type = PIRATE_WRANGLER;
		else if(prob > 90 and prob <= 98) type = PIRATE_MARAUDER;
		else if(prob == 99) type = PIRATE_GUNSHIP;
	}
	else if( t == GALACTIC_FEDERATION )
	{
		type = FEDERATION_MKI;
		if(prob > 75 and prob <= 80) type = FEDERATION_MKII;
		else if(prob > 80 and prob <= 90) type = FEDERATION_MKIII;
		else if(prob > 90 and prob <= 98) type = FEDERATION_MKIV;
		else if(prob == 99) type = FEDERATION_GUNSHIP;
	}
	else if( t == TEAM_PLAYER )
	{
		type = PLAYER_HUNTER;
		if(prob > 50 and prob <= 80) type = PLAYER_DEFENDER;
		if(prob > 80 and prob <= 99) type = PLAYER_DESTROYER;
	}
	else if( t == TEAM_PLAYER_MINER )
	{
		type = PLAYER_MINER_DROID;
	}
	
	enemy newShip({0.0f, 0.0f}, -vel, type, t);
	
	if( t == TEAM_PLAYER ) wingmen_count++;
	else if( t == TEAM_PLAYER_MINER ) miner_count++;
	else if( t == SPOOKY_SPACE_PIRATES or t == GALACTIC_FEDERATION )  enemy_count++;
	
	newShip.setPos(p);
	
	enemies.push_back(newShip);
}

bool emnityCheck(ai_team a, ai_team b)
{
	if(	(a == b) or
		(a == NEUTRAL or b == NEUTRAL) or
		( (a == TEAM_PLAYER and b == TEAM_PLAYER_MINER) or (a == TEAM_PLAYER_MINER and b == TEAM_PLAYER) )
		)
	{
		return false;
	} 
	return true;		
}

void universe::reload(bool newGame)
{
	enemies.clear();
	missiles.clear();
	shots.clear();
	asteroids.clear();
	passive_sprites.clear();
	
	vel = {0,0};
	ply.setHealth( ply.getMaxHealth() );
	ply.setShield( ply.getMaxShield() );
	ply.setEnergy( ply.getMaxEnergy() );
	ply.setVel({0, 0});
	
	float tmp[][9] =	{
						{1.0f,	2.0f,				8.0f,		12.0f,		255.0f,	50.0f,	50.0f,	4.0f,	0.2f},//Laser cannon	0
						{12.0f,	5.0f,				3.0f,		8.0f,		50.0f,	255.0f,	65.0f,	10.0f,	0.5f},//Shotgun			1
						{1.0f,	1.0f,				2.0f,		30.0f,		40.0f,	75.0f,	255.0f,	1.0f,	0.05f},//Blue laser		2
						{1.0f,	1.0f,				2000.0f,	100.0f,		255.0f,	210.0f,	0.0f,	0.0f,	0.02f},//Yellow laser	3
						{1.0f,	3.0f/DIFFICULTY,	8.0f,		10.0f,		255.0f,	20.0f,	240.0f,	0.1f,	0.8f},//AI laser 1		4
						{3.0f,	9.0f/DIFFICULTY,	6.0f,		5.0f,		255.0f,	20.0f,	240.0f,	0.1f,	1.0f},//AI laser 2		5
						{1.0f,	2.5f/DIFFICULTY,	3.0f,		15.0f,		255.0f,	20.0f,	240.0f,	0.1f,	0.7f},//AI laser 3		6
						{30.0f,	2.0f/DIFFICULTY,	4.0f,		3.0f,		255.0f,	30.0f,	80.0f,	1.0f,	1.0f},//Boss laser 1	7
						{1.0f,	1.5f/DIFFICULTY,	2.0f,		20.0f,		255.0f,	30.0f,	80.0f,	1.0f,	0.05f},//Boss laser 2	8
						{2.0f,	0.8f/DIFFICULTY,	10.0f,		24.0f,		20.0f,	255.0f,	40.0f,	1.0f,	0.8f},//Wingman laser	9
						{1.0f,	1.0f/DIFFICULTY,	1.0f,		40.0f,		20.0f,	255.0f,	40.0f,	1.0f,	0.2f},//Wingman laser	10
						{1.0f,	0.8f/DIFFICULTY,	20.0f,		5.0f,		20.0f,	255.0f,	40.0f,	1.0f,	1.0f},//Wingman laser	11
						{1.0f,	0.1f/DIFFICULTY,	100.0f,		40.0f,		20.0f,	255.0f,	40.0f,	1.0f,	5.0f},//Mining laser	12
						{1.0f,	0.5f/DIFFICULTY,	6.0f,		20.0f,		20.0f,	255.0f,	40.0f,	1.0f,	0.9f},//Turret laser	13
						{1.0f,	0.5f/DIFFICULTY,	7.0f,		10.0f,		255.0f,	216.0f,	0.0f,	1.0f,	0.5f},//Pirate laser 1	14
						{1.0f,	0.5f/DIFFICULTY,	7.0f,		10.0f,		255.0f,	216.0f,	0.0f,	1.0f,	0.5f},//Pirate laser 2	15
						{1.0f,	0.5f/DIFFICULTY,	7.0f,		10.0f,		255.0f,	216.0f,	0.0f,	1.0f,	0.5f},//Pirate laser 3	16
						{3.0f,	0.5f/DIFFICULTY,	7.0f,		10.0f,		255.0f,	216.0f,	0.0f,	1.0f,	0.9f},//P gunship laser	17
						};
	
	for(int i = 0; i < 14; ++i) copy(&tmp[i][0], &tmp[i][8], &weapons[i][0]);
	
	partitions.ships.clear();
	partitions.lasers.clear();
	partitions.rockets.clear();
	partitions.rocks.clear();
	partitions.rects.clear();
	
	if(!newGame) return;
	
	for(int i = 0; i < UPGRADES_LEN; ++i) 
	{
		ply.setGradeArr(i, 0);
	}
	ply.setMissiles(3);
	score = 0;
}

void universe::addBuild(ship_spec type)
{
	vec2 pass;
	
	int side = rand() % 4;
	if(side == 0) pass = {randFloat(-20000.0f,20000.0f), -20000.0f};
	else if(side == 1) pass = {-20000.0f,randFloat(-20000.0f, 20000.0f)};
	else if(side == 2) pass = {randFloat(-20000.0f, 20000.0f), 20000.0f};
	else if(side == 3) pass = {20000.0f, randFloat(-20000.0f, 20000.0f)};
	
	addBuild(pass, type);
}

void universe::addBuild(vec2 p, ship_spec type)
{
	enemy newShip(p, {0.0f, 0.0f}, type, TEAM_PLAYER);
	enemies.push_back(newShip);
	
	switch(type)
	{
		case PLAYER_STATION:
			max_enemies_count += 25;
			break;
		case PLAYER_GRAVWELL:
			max_enemies_count += 5;
			break;
		case PLAYER_TURRET:
			max_enemies_count += 1;
			break;
		case PLAYER_BARRACKS:
			max_enemies_count += 20;
			break;
		default:
			break;
	}
}

#endif
