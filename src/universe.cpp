#include "universe.hpp"
#include "sfx.hpp"
#include "util.hpp"
#include "shapes.hpp"
#include "common.hpp"

bool emnityCheck(ai_team a, ai_team b);

universe::universe(): m_drawer(WIN_WIDTH, WIN_HEIGHT),
                      ply( {0.0f, 0.0f}, 64.0f )
{	
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
	
	for(int i = 0; i < BACKGROUND_DOTS; i++)
	{
    std::string id;
    switch(rand()%4)
    {
      case 0:
        id = "STARDUST_1";
        break;
      case 1:
        id = "STARDUST_2";
        break;
      case 2:
        id = "STARDUST_3";
        break;
      case 3:
        id = "STARDUST_4";
        break;
      default:
        break;
    }

    int w = 0;
    int h = 0;
    m_drawer.queryTexture(id, 0, &w, &h);
    sparkles.push_back(stardust_sprite(id, cColP, w, h));
	}
	
	score = 0;
	paused = false;
    loadShips();
    initUI();
	//for(int i = 0; i < 3; ++i) spawnShip(TEAM_PLAYER_MINER);
	//addStation();
}

void universe::addShot(vec2 p, vec2 v, float angle, std::array<float, WEAPS_W> weap, ai_team team)
{
	int temp_angle = angle + 90;
  for(int i = 0; i < weap[0]; i++)
	{
    //vec2 vec = computeVector(temp_angle);
    laser temp( p + v, v, temp_angle, weap, team);
		shots.push_back(temp);
	}
}

void universe::addMissile(vec2 p, vec2 v, float angle, bool team)
{
    missile m(p, m_drawer.getTextureRadius("MISSILE"));
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

void universe::update(float dt)
{
  std::cout << "INTERSECTION TEST " << lineIntersectCircle({-2,0},{-1,1},{0,0},1) << std::endl;
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
	
	ply.ctrlUpdate();
	ply.update(dt);
	
  if(ply.getHealth() < ply.getMaxHealth()) addParticleSprite(ply.getPos(), ply.getVel(), ply.getHealth() / ply.getMaxHealth(), "SMOKE");

  if(ply.isFiring() and ply.getCooldown() <= 0.0f and ply.getEnergy() > ply.getCurWeapStat( ENERGY_COST ))
  {
    playSnd( ply.getCurWeap() );
    addShot( ply.getPos() - ply.getVel(), ply.getVel(), ply.getAng(), ply.getWeap(), TEAM_PLAYER );
    ply.setEnergy( ply.getEnergy() - ply.getCurWeapStat(ENERGY_COST) );
    ply.setCooldown( ply.getCurWeapStat(COOLDOWN) );
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
	
	for(unsigned int i = 0; i < dots.size(); i++)
	{
		dots.at(i).setWVel(vel);
		dots.at(i).updatePos(dt);
		if(isOffScreen(dots.at(i).getPos(), MAX_DIM * BG_DENSITY / ZOOM_LEVEL))
		{
			dots.at(i).gen(true, cColP);
		}
	}	
	
	for(unsigned int i = 0; i < sparkles.size(); i++)
	{
		sparkles.at(i).setWVel(vel);
		sparkles.at(i).updateSprite(dt);
		
    std::string temp = sparkles.at(i).getIdentifier();
		int w = 0;
		int h = 0;
    m_drawer.queryTexture(temp, 0, &w, &h);
		
    if(isOffScreen(sparkles.at(i).getPos(), (MAX_DIM + std::max(w, h)) * BG_DENSITY * sparkles.at(i).getZ() / ZOOM_LEVEL))
		{
      sparkles.at(i).spriteGen(cColP, w, h);
		}
	}	
	
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
	
	partitions.rects.clear();
	partitions.ships.clear();
	partitions.lasers.clear();
	partitions.rocks.clear();
	partitions.rockets.clear();
  std::vector<enemy*> init_ship;
  std::vector<laser*> init_laser;
  std::vector<missile*> init_missile;
  std::vector<ship*> init_asteroid;
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
		
    if(std::min(p.x, pp.x) < minX) minX = p.x;
    if(std::min(p.y, pp.y) < minY) minY = p.y;
    if(std::max(p.x, pp.x) > maxX) maxX = p.x;
    if(std::max(p.y, pp.y) > maxY) maxY = p.y;
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
	
	detectCollisions(init_rect, init_ship, init_laser, init_missile, init_asteroid, 0);
	
	checkCollisions();	

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
          for(int q = 0; q < 50; ++q) addParticleSprite(pos, asteroids.at(i).getVel() + vel + randVec(6.0f), 0.0f, "SMOKE");
				}
				if(asteroids.at(i).getClassification() == ASTEROID_SMALL) score += 10;
				else
				{
					for(int q = 0; q < 4; ++q)
					{
                        ship a(m_ship_templates.at( static_cast<int>(asteroids.at(i).getClassification()) - 1 ), p + randVec( asteroids.at(i).getRadius() ));
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
        enemies.at(i).setCooldown( (enemies.at(i).getCurWeapStat(COOLDOWN)) );
				enemies.at(i).setFiring(false);
			}
			
      if(enemies.at(i).getHealth() < enemies.at(i).getMaxHealth()) addParticleSprite(p, enemies.at(i).getVel(), enemies.at(i).getHealth() / enemies.at(i).getMaxHealth(), "SMOKE");
		}
	}	
	
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
			
            vec2 ppos;
			int side = rand() %4 ;
            if(side == 0) ppos = {randFloat(-20000.0f,20000.0f), -20000.0f};
            else if(side == 1) ppos = {randFloat(-20000.0f,20000.0f), 20000.0f};
            else if(side == 2) ppos = {-20000.0f, randFloat(-20000.0f,20000.0f)};
            else if(side == 3) ppos = {20000.0f, randFloat(-20000.0f,20000.0f)};
            ship a(ppos, size, m_drawer.getTextureRadius( getTextureKey(size) ));
			a.setVel( randVec(64.0f) );
			a.update(dt);
			asteroids.push_back(a);
		}
	}
	
	for(int i = particles.size() - 1; i >= 0; i--)
	{
		particles.at(i).setWVel(vel);
		particles.at(i).update(dt);
		if(particles.at(i).done()) swapnpop(&particles, i);

	}
	
	for(int i = passive_sprites.size() - 1; i >= 0; --i)
	{
		float alph = passive_sprites.at(i).getCol(3);
		
		passive_sprites.at(i).setWVel(vel);
		passive_sprites.at(i).updateSprite(dt);
		
    std::string temp = passive_sprites.at(i).getIdentifier();
		int w = 0;
		int h = 0;
    m_drawer.queryTexture(temp, 0, &w, &h);
		
    if(alph < 0.0f or isOffScreen(passive_sprites.at(i).getPos(), (MAX_DIM + std::max(w, h)) * BG_DENSITY * passive_sprites.at(i).getZ() / ZOOM_LEVEL) or passive_sprites.at(i).getDim() <= 0.0f)
		{
			swapnpop(&passive_sprites, i);
			continue;
		}
		alph *= 0.94f;
		alph -= 0.1f;
		passive_sprites.at(i).setCol(3, alph);
	}
}

void universe::draw(float dt)
{	
	if(paused) dt = 0.0f;

  m_drawer.clear();

  for(auto i = dots.begin(); i != dots.end(); ++i)
	{	
    if(i->getZ() > 1) continue;

    vec2 ipos = i->getInterpolatedPosition(dt);
    vec2 ivel = i->getVel();
    std::array<float, 4> icol = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
    m_drawer.drawLine(ipos, ipos + ivel, icol);
	}
	
  m_drawer.setBlendMode(SDL_BLENDMODE_ADD);
  for(auto i = sparkles.begin(); i != sparkles.end(); ++i)
	{	
    if(i->getZ() <= 1)
    {
      vec2 ipos = i->getInterpolatedPosition(dt);
      std::array<float, 4> col = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
      m_drawer.drawTexture( i->getTex(), 0, ipos, i->getAng(), col );
    }
	}
  m_drawer.setBlendMode(SDL_BLENDMODE_BLEND);
	
  for(auto i = passive_sprites.begin(); i != passive_sprites.end(); ++i)
	{	
    if(!paused) i->incrDim();
    vec2 ipos = i->getInterpolatedPosition(dt);
    std::array<float, 4> col = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
    m_drawer.drawTexture( i->getTex(), 0, ipos, i->getAng(), col );
	}
	
  for(auto i = shots.begin(); i != shots.end(); ++i)
  {
    vec2 ipos = i->getInterpolatedPosition(dt);
    vec2 ivel = (i->getVel() + i->getWVel()) * 3;
    std::array<float, 4> iscol = {i->getCol(0), i->getCol(1), i->getCol(2), 255};
    std::array<float, 4> iecol = {iscol[0] / 2, iscol[1] / 2, iscol[2] / 2, 20};
    m_drawer.drawLineGr(ipos, ipos + ivel, iecol, iscol);
	}
	
  for(auto i = asteroids.begin(); i != asteroids.end(); ++i)
	{
    vec2 ipos = i->getInterpolatedPosition(dt);
    std::array<float, 4> icol = {255, 255, 255, 255};
    m_drawer.drawTexture(i->getIdentifier(), 0, ipos, i->getAng(), icol);
    //asteroids.at(i).draw(dt);
	}
	
  for(auto i = enemies.begin(); i != enemies.end(); ++i)
	{
    //enemies.at(i).draw(dt);
    vec2 ipos = i->getInterpolatedPosition(dt);
    std::array<float, 4> ialpha = i->getAlphaStats();
    m_drawer.drawTextureSet(i->getIdentifier(), ipos, i->getAng(), ialpha);
	}
	
  vec2 ppos = ply.getInterpolatedPosition(dt);
  std::array<float, 4> palpha = ply.getAlphaStats();
  m_drawer.drawTextureSet(ply.getIdentifier(), ppos, ply.getAng(), palpha);
  m_drawer.drawCircle(ply.getPos().x, ply.getPos().y, ply.getRadius(), {255,0,0,255});
	
  for(auto i = missiles.begin(); i != missiles.end(); ++i)
	{	
    //missiles.at(i).draw(dt);
    vec2 ipos = i->getInterpolatedPosition(dt);
    std::array<float, 4> ialpha = i->getAlphaStats();
    m_drawer.drawTextureSet(i->getIdentifier(), ipos, i->getAng(), ialpha);
	}
	
  for(auto i = particles.begin(); i != particles.end(); ++i)
	{
    //particles.at(i).draw(dt);
    vec2 ipos = i->getPos();
    std::array<float, 4> col = {i->getCol(0), i->getCol(1), i->getCol(2), i->getAlpha()};
    m_drawer.drawTexture(i->getIdentifier(), 0, ipos, 0, col);
    int k = 0;
    for(auto j = i->getParticles()->begin(); j != i->getParticles()->end(); ++j)
    {
      vec2 jpos = j->getInterpolatedPosition(dt);
      std::array<float, 4> col = {col[0], col[1], col[2], i->getAlpha(k)};
      m_drawer.drawLine(jpos, jpos + j->getVel(), col);
      k++;
    }
	}
	
  for(auto i = dots.begin(); i != dots.end(); ++i)
	{	
    if(i->getZ() <= 1) continue;
    //dots.at(i).draw(dt);
    vec2 ipos = i->getInterpolatedPosition(dt);
    vec2 ivel = i->getVel();
    std::array<float, 4> icol = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
    m_drawer.drawLine(ipos, ipos + ivel, icol);
	}
	
  m_drawer.setBlendMode(SDL_BLENDMODE_ADD);
  for(auto i = sparkles.begin(); i != sparkles.end(); ++i)
	{	
    if(i->getZ() > 1)
		{
      //sparkles.at(i).draw(dt);
      vec2 ipos = i->getInterpolatedPosition(dt);
      std::array<float, 4> icol = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
      m_drawer.drawTexture( i->getTex(), 0, ipos, i->getAng(), icol );

		}
	}
  m_drawer.setBlendMode(SDL_BLENDMODE_BLEND);
	
	if(DEV_MODE)
	{
    for(auto i = partitions.rects.begin(); i != partitions.rects.end(); ++i)
    {
      std::array<int, 4> col = {255, 0, 0, 255};
      m_drawer.drawRect({static_cast<float>(i->x), static_cast<float>(i->y)}, {static_cast<float>(i->w), static_cast<float>(i->h)}, col, true);
    }
	}

  //Draw the ui
  drawUI();

  m_drawer.finalise();
}

void universe::drawUI()
{
  m_drawer.drawText("SCORE: " + std::to_string( score ),"pix",{260, 2});
  m_drawer.drawText("MISSILES: " + std::to_string( ply.getMissiles() ),"pix",{260, 2});

  m_drawer.drawMap(&missiles, &enemies, &asteroids, &shots);
  m_drawer.statusBars(&ply);

  for(auto i = m_ui.getElements()->begin(); i != m_ui.getElements()->end(); ++i)
  {
    for(auto j = i->getButtons()->begin(); j != i->getButtons()->end(); ++j)
    {
        std::array<int, 8> col = j->getCol();
        if(!j->isSelected())
        {
          m_drawer.drawRect(j->getPos(), j->getDim(), {col[0], col[1], col[2], col[3]}, false);
        }
        else
        {
          m_drawer.drawRect(j->getPos(), j->getDim(), {col[4], col[5], col[6], col[7]}, false);
        }
        m_drawer.drawText(j->getLabel(), "pix", j->getPos());

        if(j->isDark())
        {
          m_drawer.drawRect(j->getPos(), j->getDim(), {0, 0 ,0 , 200}, false);
        }
     }
  }
}

void universe::detectCollisions(SDL_Rect box, std::vector<enemy*> ships, std::vector<laser*> lasers, std::vector<missile*> rockets, std::vector<ship*> rocks, unsigned short int lvl)
{
	//cout << lvl << ") SIZES : ships " << ships.size() << ", lasers " << lasers.size() << ", missiles " << rockets.size() << ", rocks " << rocks.size() << endl;
	size_t count = 0;
    std::vector<enemy*> pships;
    std::vector<laser*> plasers;
    std::vector<missile*> prockets;
    std::vector<ship*> procks;
	
    vec2 boxPos = {box.x, box.y};
    vec2 boxDim = {box.w, box.h};

	for(size_t i = 0; i < ships.size(); ++i)
	{
		enemy * s = ships.at(i);
        if( circleIntersectRect(s->getPos(), s->getRadius(), boxPos, boxDim ) )
		{
			pships.push_back(s);
			count++;
		}
	}
	
	for(size_t i = 0; i < rockets.size(); ++i)
	{
		missile * r = rockets.at(i);
        if( circleIntersectRect(r->getPos(), r->getRadius(), boxPos, boxDim ) )
		{
			prockets.push_back(r);
			count++;
		}
	}
	
	for(size_t i = 0; i < rocks.size(); ++i)
	{
		ship * r = rocks.at(i);
        if( circleIntersectRect(r->getPos(), r->getRadius(), boxPos, boxDim ) )
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
        for(int l = partitions.lasers.at(p).size() - 1; l >= 0; --l)
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

                if(lineIntersectCircle(sp, spv, ep, er))
				{
					addpfx(ep + randVec(er), ev, vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
					harm = partitions.lasers.at(p).at(l)->getDmg();
					
                    //Delete shots if they match the ones in the universe vector.
                    for(int i = shots.size() - 1; i >= 0; --i) if(&shots.at(i) == partitions.lasers.at(p).at(l)) swapnpop(&shots, i);
                    //Pop the partition pointer.
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
				
                if(lineIntersectCircle(sp, spv, ep, er))
				{
					addpfx(ep + randVec(er), ev, vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
                    for(int q = 0; q < 20; ++q) addParticleSprite(ep, ev + randVec(6.0f), 0.0f, "SMOKE");
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
												
                if(lineIntersectCircle(sp, spv, ep, er))
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
                if(lineIntersectCircle(sp, spv, dp, ply.getRadius()))
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
  pfx pf(p,v,wv,no,f,"EXPLOSION");
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

void universe::addParticleSprite(vec2 p, vec2 v, float m, std::string tex)
{
	float col;
  if( tex == "SMOKE" ) col = randFloat(200.0f, 220.0f) * (1.0f - m);
  else if( tex == "EXPLOSION" ) col = 255.0f;
	
  int w = 0, h = 0;
  m_drawer.queryTexture(tex, 0, &w, &h);
  stardust_sprite sm (tex, col, w, h);
	sm.setPos(p);
  if( tex == "SMOKE" ) sm.setVel(v + randVec(1.0f));
  else if( tex == "EXPLOSION" ) sm.setVel(v);
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
	
    enemy newShip({0.0f, 0.0f}, -vel, type, t, m_drawer.getTextureRadius(getTextureKey(type)));
	
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
	
  m_ui.reset();

  /*float tmp[][9] =	{
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
	
  for(int i = 0; i < 14; ++i) std::copy(&tmp[i][0], &tmp[i][8], &weapons[i][0]);*/
	
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
    enemy newShip(p, {0.0f, 0.0f}, type, TEAM_PLAYER, m_drawer.getTextureRadius(getTextureKey(type)));
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

void universe::initUI()
{
  //Initialise the two selection menus.
  selection energy_menu;
  selection upgrades_menu;

  //Add buttons to the energy menu.
  std::array<int, 8> arr1 = {20,20,20,200,100,100,100,255};
  std::array<int, 8> arr2 = {100,100,100,200,250,250,250,255};
  button energy_menu_neutral("BALANCED",arr1,arr2,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.35f},{128.0f,64.0f});
  energy_menu_neutral.set(true);
  energy_menu.add(energy_menu_neutral);

  arr1 = {12,24,26,200,27,95,232,255};
  arr2 = {45,67,188,200,119,156,238,255};
  button energy_menu_shields("SHIELDS",arr1,arr2,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.45f},{128.0f,64.0f});
  energy_menu.add(energy_menu_shields);

  arr1 = {14,35,20,200,36,204,52,255};
  arr2 = {65,127,64,200,129,241,127,255};
  button energy_menu_engines("ENGINES",arr1,arr2,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.55f},{128.0f,64.0f});
  energy_menu.add(energy_menu_engines);

  arr1 = {35,23,23,200,232,31,31,255};
  arr2 = {124,33,33,200,217,116,116,255};
  button energy_menu_guns("GUNS",arr1,arr2,{WIN_WIDTH * 0.9f, WIN_HEIGHT * 0.65f},{128.0f,64.0f});
  energy_menu.add(energy_menu_guns);

  //Add buttons to the upgrades menu.
  float w = 150.0f, h = 50.0f;
  arr1 = {100,50,50,200,250,200,200,255};
  arr2 = {100,50,50,200,250,200,200,255};
  button upgrades_lasers("LASERS I (4)",arr1,arr2,{WIN_WIDTH * 0.0f, WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_lasers);

  arr1 = {50,50,100,200,200,200,250,255};
  arr2 = {50,50,100,200,200,200,250,255};
  button upgrades_shields("SHIELDS I (4)",arr1,arr2,{WIN_WIDTH * 0.15f, WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_shields);

  arr1 = {50,100,50,200,200,250,200,255};
  arr2 = {50,100,50,200,200,250,200,255};
  button upgrades_generators("GENERATORS I (4)",arr1,arr2,{WIN_WIDTH * 0.3f, WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_generators);

  arr1 = {50,50,80,200,200,200,220,255};
  arr2 = {50,50,80,200,200,200,220,255};
  button upgrades_thrusters("THRUSTERS I (4)",arr1,arr2,{WIN_WIDTH * 0.45f, WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_thrusters);

  arr1 = {255,210,0,200,255,253,100,255};
  arr2 = {255,210,0,200,255,253,100,255};
  button upgrades_missiles("MISSILE (4)",arr1,arr2,{WIN_WIDTH * 0.6f, WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_missiles);

  arr1 = {100,210,255,200,180,220,255,255};
  arr2 = {100,210,255,200,180,220,255,255};
  button upgrades_miner("MINER (16)",arr1,arr2,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.85f},{w,h},16);
  upgrades_menu.add(upgrades_miner);

  button upgrades_wingman("WINGMAN (32)",arr1,arr2,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.79f},{w,h},32);
  upgrades_menu.add(upgrades_wingman);

  button upgrades_turret("TURRET (32)",arr1,arr2,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.73f},{w,h},32);
  upgrades_menu.add(upgrades_turret);

  button upgrades_gravwell("GRAVWELL (512)",arr1,arr2,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.67f},{w,h},512);
  upgrades_menu.add(upgrades_gravwell);

  button upgrades_barracks("BARRACKS (1024)",arr1,arr2,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.61f},{w,h},1024);
  upgrades_menu.add(upgrades_barracks);

  button upgrades_station("STATION (1024)",arr1,arr2,{WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.55f},{w,h},1024);
  upgrades_menu.add(upgrades_station);

  m_ui.add(energy_menu);
  m_ui.add(upgrades_menu);
}

bool universe::upgradeCallback(int _sel, int _btn)
{
  //This function takes the selected button, looks at the cost vs the score, updates relevant values,
  //then returns a bool representing whether the upgrade was successful or unsuccessful.

  button * selectedButton = &m_ui.getElements()->at(_sel).getButtons()->at(_btn);
  int lvl = ply.getUpgrade( _btn );

  selectedButton->set(false);

  if(selectedButton->getCost() > score) return false;

  if(lvl < 9)
  {
    score -= selectedButton->getCost();
    if(_btn < 4) selectedButton->setCost(selectedButton->getCost() * 2);
  }

  upgradeSetLabels(_sel, _btn);

  if(_btn > 3) return true;

  return true;
}

void universe::upgradeSetLabels(int _sel, int _btn)
{
  button * selectedButton = &m_ui.getElements()->at(_sel).getButtons()->at(_btn);
  std::string s1;
  int lvl = ply.getUpgrade( _btn );
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
  }

  s1 += roman_nums[lvl];

  if(lvl < 8)
  {
    s1 += " (";
    std::stringstream ss;
    ss << selectedButton->getCost();
    s1 += ss.str();
    s1 += ")";
  }

  if(lvl < 9) selectedButton->updateText(s1);
}

//This function loads all the ships in the game into a vector that we can copy from later.
void universe::loadShips()
{
    std::cout << "enter" << std::endl;
    for(int i = 0; i < static_cast<int>(SHIPS_END); ++i)
    {
      std::cout << "  loop " << i << std::endl;
        ship insert( {0.0f, 0.0f}, static_cast<ship_spec>(i), m_drawer.getTextureRadius(getTextureKey(static_cast<ship_spec>(i))) );
        m_ship_templates.push_back(insert);
    }
    std::cout << "No of ship types: " << m_ship_templates.size() << std::endl;
}
