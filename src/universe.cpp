#include "universe.hpp"
#include "sfx.hpp"
#include "util.hpp"
#include "shapes.hpp"
#include "common.hpp"

bool emnityCheck(const aiTeam _a, const aiTeam _b);

universe::universe()
  :
    m_drawer(g_WIN_WIDTH, g_WIN_HEIGHT),
    m_ply( {0.0f, 0.0f}, m_drawer.getTextureRadius(getTextureKey(PLAYER_SHIP)) )
{
  m_time_elapsed = 0.0;
  m_pos = {0.0f, 0.0f};
  setVel({0,0});

  m_factionCounts.assign(6, 0);
  m_factionMaxCounts.assign(6, 0);

  m_factionMaxCounts[GALACTIC_FEDERATION] = 3;

  m_ply.setPos({g_WIN_WIDTH/2.0f,g_WIN_HEIGHT/2.0f});
  m_ply.setPPos({g_WIN_WIDTH/2.0f,g_WIN_HEIGHT/2.0f});

  m_tCol[0] = 40.0f;
  m_tCol[1] = 90.0f;
  m_tCol[2] = 100.0f;

  m_cCol[0] = 40.0f;
  m_cCol[1] = 90.0f;
  m_cCol[2] = 100.0f;

  for(int i = 0; i < g_BACKGROUND_DOTS; i++)
  {
    m_dots.push_back(stardust(m_cCol));
  }

  for(int i = 0; i < g_BACKGROUND_DOTS; i++)
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
    m_sparkles.push_back(stardust_sprite(id, m_cCol, w, h));
  }
  initUI();
  if(g_DEV_MODE) setScore(100000);
  else setScore(0);

  m_paused = false;
  m_mouse_state = -1;

  createFactions();
  loadShips();

  m_escMenuShown = false;
}

void universe::addShot(
    const vec2 _p,
    const vec2 _v,
    const float _angle,
    const std::array<float, WEAPS_W> _weap,
    const aiTeam _team
    )
{
  int temp_angle = _angle + 90;
  for(int i = 0; i < _weap[0]; ++i)
  {
    //vec2 vec = computeVector(temp_angle);
    laser temp( _p + _v, _v, temp_angle, _weap, _team);
    m_shots.push_back(temp);
  }
}

void universe::addMissile(
    const vec2 _p,
    const vec2 _v,
    const float _angle,
    const aiTeam _team
    )
{
  missile m(_p, m_drawer.getTextureRadius("ION_MISSILE_MKI"));
  m.setVel(_v + computeVector(_angle + 90) * 5);
  m.setWVel(m_vel);
  m.setAng(_angle);

  int mx = 0, my = 0;
  SDL_GetMouseState(&mx,&my);
  mx -= g_HALFWIN.m_x;
  my -= g_HALFWIN.m_y;

  m.setTarget(closestEnemy({ (static_cast<float>(mx) - g_HALFWIN.m_x) / g_ZOOM_LEVEL, (static_cast<float>(my) - g_HALFWIN.m_y) / g_ZOOM_LEVEL}, _team));

  m_missiles.push_back(m);
}

void universe::update(const float _dt)
{
  //std::cout << 1/_dt << "fps" << std::endl;
  //If m_paused, we do not update the game.
  if(m_paused) return;

  m_pos += m_vel;
  m_time_elapsed += _dt;
  //Interpolate towards desired background colour.
  m_cCol[0] += clamp(m_tCol[0] - m_cCol[0], -1.0f, 1.0f);
  m_cCol[1] += clamp(m_tCol[1] - m_cCol[1], -1.0f, 1.0f);
  m_cCol[2] += clamp(m_tCol[2] - m_cCol[2], -1.0f, 1.0f);

  //Randomly change background colour.
  if(rand()%10000 == 0)
  {
    float p0 = randFloat(0.0f, 100.0f);
    float p1 = randFloat(0.0f, 100.0f) + p0;
    float p2 = randFloat(0.0f, 100.0f) + p0 + p1;
    float total = p0 + p1 + p2;

    m_tCol[0] = ( p0 / total ) * 250.0f;
    m_tCol[1] = ( p1 / total ) * 250.0f;
    m_tCol[2] = ( p2 / total ) * 250.0f;
  }

  if(rand()%10000 == 0) g_BG_DENSITY = randFloat(1.0f,10.0f);
  if(rand()%10000 == 0) m_gameplay_intensity = randFloat(0.0f, 2.2f);

  if(!g_GAME_OVER)
  {
    m_ply.ctrlUpdate();
    m_ply.update(_dt);
  }

  //If player health is below 25%, emit smoke.
  if(m_ply.getHealth() < m_ply.getMaxHealth() / 4.0f and m_ply.getHealth() > 0.0f) addParticleSprite(m_ply.getPos(), m_ply.getVel(), m_ply.getHealth() / m_ply.getMaxHealth(), "SMOKE");

  if(m_ply.isFiring() and m_ply.getCooldown() <= 0.0f and m_ply.getEnergy() > m_ply.getCurWeapStat( ENERGY_COST ))
  {
    playSnd( static_cast<sound>(m_ply.getCurWeap()) );
    addShot( m_ply.getPos() - m_ply.getVel(), m_ply.getVel(), m_ply.getAng(), m_ply.getWeap(), TEAM_PLAYER );
    m_ply.setEnergy( m_ply.getEnergy() - m_ply.getCurWeapStat(ENERGY_COST) );
    m_ply.setCooldown( m_ply.getCurWeapStat(COOLDOWN) );
  }

  if(!g_DEV_MODE and m_ply.getHealth() <= 0.0f and !g_GAME_OVER)
  {
    for(int p = 0; p < rand()%5 + 10; ++p)
    {
      vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
      pos += m_ply.getPos();
      addpfx(pos, m_ply.getVel(), m_vel, rand()%50 + 20, rand()%50 + 8);
    }
    m_ply.setMaxHealth(0, true);
    m_ply.setMaxEnergy(0, true);
    m_ply.setMaxShield(0, true);
    playSnd(EXPLOSION_SND);

    m_ply.setPos({F_INF, F_INF});

    m_ui.clear();

    g_GAME_OVER = true;
  }

  for(auto &i : m_dots)
  {
    i.setWVel(m_vel);
    i.updatePos(_dt);
    if(isOffScreen(i.getPos(), clamp(g_MAX_DIM * g_BG_DENSITY / g_ZOOM_LEVEL, g_MAX_DIM, F_INF)))
    {
      i.gen(true, m_cCol);
    }
  }

  for(auto &i : m_sparkles)
  {
    i.setWVel(m_vel);
    i.updateSprite(_dt);

    std::string temp = i.getIdentifier();
    int w = 0;
    int h = 0;
    m_drawer.queryTexture(temp, 0, &w, &h);

    if(isOffScreen(i.getPos(), clamp((g_MAX_DIM + std::max(w, h)) * g_BG_DENSITY * i.getZ() / g_ZOOM_LEVEL, (g_MAX_DIM + std::max(w, h)), F_INF )))
    {
      i.spriteGen(m_cCol, w, h);
    }
  }

  for(int i = m_shots.size() - 1; i >= 0; i--)
  {
    if(m_shots.at(i).getPower() < 0.0f)
    {
      swapnpop(&m_shots, i);
    }
    else
    {
      m_shots.at(i).setWVel(m_vel);
      m_shots.at(i).update(_dt);
    }
  }
  m_partitions.rects.clear();
  m_partitions.ships.clear();
  m_partitions.lasers.clear();
  m_partitions.rocks.clear();
  m_partitions.rockets.clear();
  std::vector<enemy*> init_ship;
  std::vector<laser*> init_laser;
  std::vector<missile*> init_missile;
  std::vector<ship*> init_asteroid;
  SDL_Rect init_rect = {0, 0, 0, 0};
  int minX = I_INF, minY = I_INF, maxX = -I_INF, maxY = -I_INF;
  for(auto &i : m_agents)
  {
    vec2 p = i.getPos();
    if(p.m_x < minX) minX = p.m_x;
    if(p.m_y < minY) minY = p.m_y;
    if(p.m_x > maxX) maxX = p.m_x;
    if(p.m_y > maxY) maxY = p.m_y;
    init_ship.push_back( &i );
  }
  for(auto &i : m_missiles)
  {
    vec2 p = i.getPos();
    if(p.m_x < minX) minX = p.m_x;
    if(p.m_y < minY) minY = p.m_y;
    if(p.m_x > maxX) maxX = p.m_x;
    if(p.m_y > maxY) maxY = p.m_y;
    init_missile.push_back( &i );
  }
  for(auto &i : m_asteroids)
  {
    vec2 p = i.getPos();
    if(p.m_x < minX) minX = p.m_x;
    if(p.m_y < minY) minY = p.m_y;
    if(p.m_x > maxX) maxX = p.m_x;
    if(p.m_y > maxY) maxY = p.m_y;
    init_asteroid.push_back( &i );
  }
  for(auto &i : m_shots)
  {
    vec2 p = i.getPos();
    vec2 pp = p + i.getVel();

    if(std::min(p.m_x, pp.m_x) < minX) minX = p.m_x;
    if(std::min(p.m_y, pp.m_y) < minY) minY = p.m_y;
    if(std::max(p.m_x, pp.m_x) > maxX) maxX = p.m_x;
    if(std::max(p.m_y, pp.m_y) > maxY) maxY = p.m_y;
    init_laser.push_back( &i );
  }
  if(m_ply.getPos().m_x < minX) minX = m_ply.getPos().m_x;
  if(m_ply.getPos().m_y < minY) minY = m_ply.getPos().m_y;
  if(m_ply.getPos().m_x > maxX) maxX = m_ply.getPos().m_x;
  if(m_ply.getPos().m_y > maxY) maxY = m_ply.getPos().m_y;

  init_rect.x = minX;
  init_rect.y = minY;
  init_rect.w = maxX - minX;
  init_rect.h = maxY - minY;

  detectCollisions(init_rect, init_ship, init_laser, init_missile, init_asteroid, 0);
  checkCollisions();

  for(int i = m_missiles.size() - 1; i >= 0; i--)
  {
    m_missiles.at(i).updatePos(_dt);
    bool ofscr = isOffScreen(m_missiles.at(i).getPos(), 30000.0f);
    if(ofscr or m_missiles.at(i).getHealth() <= 0 or m_missiles.at(i).detonate())
    {
      if(!ofscr)
      {
        for(int p = 0; p < rand()%2 + 4; p++)
        {
          vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
          pos += m_missiles.at(i).getPos();
          addpfx(pos, m_missiles.at(i).getVel(), m_vel, rand()%50 + 50, rand()%50 + 8);
        }

        for(size_t j = 0; j < m_agents.size(); j++)
        {
          vec2 ep = m_agents.at(j).getPos();
          vec2 mp = m_missiles.at(i).getPos();

          float dmg = magns(mp-ep);

          if(dmg > 300*300) continue;

          dmg = 1/mag(mp-ep) * 30000;
          m_agents.at(j).damage(dmg);
        }

        vec2 pdiff = m_missiles.at(i).getPos() - m_ply.getPos();
        if(magns(pdiff) < 300*300) m_ply.damage(1 / mag(pdiff) * 30000);
      }
      swapnpop(&m_missiles, i);
    }
    else
    {
      if(m_missiles.at(i).getTarget() == nullptr) m_missiles.at(i).setTarget(closestEnemy(m_missiles.at(i).getPos(), m_missiles.at(i).getTeam()));
      m_missiles.at(i).setWVel(m_vel);
      m_missiles.at(i).update(_dt);
      m_missiles.at(i).steering();
    }
  }

  for(int i = m_asteroids.size() - 1; i >= 0; i--)
  {
    m_asteroids.at(i).updatePos(_dt);
    vec2 p = m_asteroids.at(i).getPos();
    if((isOffScreen(p,30000.0f) or m_asteroids.at(i).getHealth() <= 0.0f))
    {
      if(m_asteroids.at(i).getHealth() <= 0.0f)
      {
        for(int fx = 0; fx < rand() % 5 + 1; fx++)
        {
          vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
          pos += m_asteroids.at(i).getPos();
          addpfx(pos, m_asteroids.at(i).getVel(), m_vel, rand()%20 + 50, rand()%30 + 2);
          for(int q = 0; q < 50; ++q) addParticleSprite(pos, m_asteroids.at(i).getVel() + m_vel + randVec(6.0f), 0.0f, "SMOKE");
        }
        if(m_asteroids.at(i).getClassification() == ASTEROID_SMALL) addScore( 10 );
        else
        {
          for(int q = 0; q < 4; ++q)
          {
            ship a(g_ship_templates.at( static_cast<int>(m_asteroids.at(i).getClassification()) - 1 ), p + randVec( m_asteroids.at(i).getRadius() ));
            a.setVel( m_asteroids.at(i).getVel() + randVec(1.0f) );
            a.update(_dt);
            m_asteroids.push_back(a);
          }
        }
        playSnd(EXPLOSION_SND);
      }
      swapnpop(&m_asteroids, i);
    }
    else
    {
      m_asteroids.at(i).setWVel(m_vel);
      m_asteroids.at(i).update(_dt);
    }
  }

  //Cull dead m_agents.
  for(int i = m_agents.size() - 1; i >= 0; i--)
  {
    vec2 p = m_agents.at(i).getPos();
    //Offscreen elimination, health-based elimination
    if((isOffScreen(p,30000.0f) and m_agents.at(i).getTeam() != TEAM_PLAYER and m_agents.at(i).getTeam() != TEAM_PLAYER_MINER) or m_agents.at(i).getHealth() <= 0)
    {
      if(m_agents.at(i).getHealth() <= 0.0f)
      {
        for(int p = 0; p < rand() % 5 + 1; p++)
        {
          vec2 pos = {randFloat(-16.0f,16.0f), randFloat(-16.0f,16.0f)};
          pos += m_agents.at(i).getPos();
          addpfx(pos, m_agents.at(i).getVel(), m_vel, rand()%20 + 50, rand()%30 + 2);
        }
        addScore( m_agents.at(i).getScore() );
        if( rand() % 8 <= g_DIFFICULTY ) m_factionMaxCounts[GALACTIC_FEDERATION] += g_DIFFICULTY + 1;
        playSnd(EXPLOSION_SND);
      }
      if(emnityCheck(m_agents.at(i).getTeam(), TEAM_PLAYER)) m_factionCounts[GALACTIC_FEDERATION]--;
      else m_factionCounts[m_agents.at(i).getTeam()]--;

      swapnpop(&m_agents, i);
    }
  }

  //Set squad variables
  //Get the average position for each squad.
  for(auto &s : m_squads) {s.m_centerPoint = {0.0f, 0.0f}; s.m_averageVel = {0.0f, 0.0f};}
  for(auto &e : m_agents)
  {
    //std::cout << e.getSquadID() << std::endl;
    if(e.getSquadID() >= 0) {m_squads[e.getSquadID()].m_centerPoint += e.getPos(); m_squads[e.getSquadID()].m_averageVel += e.getVel();}
  }
  for(auto &s : m_squads)
  {
    if(s.m_size > 0) {s.m_centerPoint /= s.m_size; s.m_averageVel /= s.m_size;}
  }

  //Update live m_agents.
  for(auto &e : m_agents)
  {
    e.updatePos(_dt);
    vec2 p = e.getPos();

    if(e.getHealth() < e.getConfidence()) e.setEnergyPriority(2);
    else if(e.getHealth() < e.getMaxHealth() * 0.75f) e.setEnergyPriority(1);
    else e.setEnergyPriority(0);

    e.setWVel(m_vel);
    e.update(_dt);

    //If the agent is damaged, add smoke.
    if(e.getHealth() < e.getMaxHealth()) addParticleSprite(p, e.getVel(), e.getHealth() / e.getMaxHealth(), "SMOKE");

    float minDist = F_MAX;
    e.setTarget(nullptr);

    if(e.getClassification() == PLAYER_MINER_DROID) //Set miner targets
    {
      //Find the closest asteroid.
      for(auto &k : m_asteroids)
      {
        float nd = magns(p - k.getPos());
        if(nd < minDist)
        {
          e.setTarget( &k );
          e.setGoal(GOAL_ATTACK);
          minDist = nd;
        }
      }
    }
    else if(e.getClassification() == PLAYER_GRAVWELL) //Gravwell attraction
    {
      //Attract m_asteroids based on distance.
      for(auto &k : m_asteroids)
      {
        vec2 incr = p - k.getPos();
        float dist = fabs( magns( incr ) );
        dist /= sqr( 32.0f );

        if(dist > 10000.0f) k.addVel( incr / dist );
        else k.addVel( -k.getVel() * 0.0125f );
      }
    }
    else if(e.getClassification() == PLAYER_BARRACKS)
    {
      //Spawn wingman.
      if(rand() % 2048 == 0 and m_factionCounts[TEAM_PLAYER] < 20) spawnShip(TEAM_PLAYER, p);
    }
    else if(e.getCanShoot()) //Default m_target acquisition
    {
      //Get closest enemy.
      for(auto &k : m_agents)
      {
        if(&e == &k) continue;

        float nd = magns(p - k.getPos());
        if(nd < minDist and emnityCheck(e.getTeam(), k.getTeam()))
        {
          e.setTarget( (enemy*)&k );
          if(e.getClassification() != PLAYER_TURRET) e.setGoal(GOAL_ATTACK);
          else e.setGoal(GOAL_TURRET);
          minDist = nd;
        }
      }
    }

    //Setting the follow distances of the different units.
    float fd = 0.0f;
    if(e.getTeam() == TEAM_PLAYER) fd = 1500.0f;
    else if(e.getTeam() == TEAM_PLAYER_MINER) fd = 20000.0f;
    float nd = magns(m_ply.getPos() - e.getPos());

    if(emnityCheck( e.getTeam(), TEAM_PLAYER ) and nd < minDist and !g_GAME_OVER )
    {
      //If the given agent is hostile, and the players distance is the closest ship.
      e.setTarget( (player*)&m_ply );
      e.setGoal(GOAL_ATTACK);
      minDist = nd;
    }
    else if(e.getCanMove() and !emnityCheck( e.getTeam(), TEAM_PLAYER ) and ( nd > fd * fd or e.getTarget() == nullptr ) and !e.inCombat())
    {
      //If the agent is non-hostile AND not in combat AND it either has no m_target, OR it is too far away, is follows the player.
      e.setTarget( (player*)&m_ply );
      e.setGoal( GOAL_CONGREGATE );
    }
    else if( e.getTarget() == nullptr )
    {
      //If the agent has no m_target, it becomes idle.
      e.setGoal( GOAL_IDLE );
    }
    //std::cout << "P1" << std::endl;
    if(emnityCheck( e.getTeam(), TEAM_PLAYER ) and e.getHealth() < e.getConfidence() and e.getCanMove())
    {
      //If the enemy can move and is scared, runs away.
      removeFromSquad(&e, getSquadFromID(e.getSquadID()));
      e.setGoal(GOAL_FLEE);
    }
    //std::cout << "P2" << std::endl;
    if(e.isFiring() and e.getCooldown() <= 0)
    {
      //If the agent is shooting, add lasers.
      addShot(e.getPos() - e.getVel(), e.getVel(), e.getAng(), e.getWeap(), e.getTeam());
      e.setCooldown( (e.getCurWeapStat(COOLDOWN)) );
      e.setFiring(false);
    }

    //If too far from group center, congregate at center.
    if(e.getSquadID() >= 0 and !e.inCombat() and magns(p - m_squads[e.getSquadID()].m_centerPoint) > sqr(m_squads[e.getSquadID()].m_regroupDist) )
    {
      e.setTarget(nullptr);
      e.setTPos( m_squads[e.getSquadID()].m_centerPoint );
      e.setTVel( m_squads[e.getSquadID()].m_averageVel );
      e.setGoal(GOAL_CONGREGATE);
    }

    //Update behaviour, steer towards m_target.
    e.behvrUpdate();
    e.steering();
  }

  //Ship spawning functions.
  for(int i = m_particles.size() - 1; i >= 0; i--)
  {
    m_particles.at(i).setWVel(m_vel);
    m_particles.at(i).update(_dt);
    if(m_particles.at(i).done()) swapnpop(&m_particles, i);

  }
  for(int i = m_passive_sprites.size() - 1; i >= 0; --i)
  {
    float alph = m_passive_sprites.at(i).getCol(3);

    m_passive_sprites.at(i).setWVel(m_vel);
    m_passive_sprites.at(i).updateSprite(_dt);

    std::string temp = m_passive_sprites.at(i).getIdentifier();
    int w = 0;
    int h = 0;
    m_drawer.queryTexture(temp, 0, &w, &h);

    if(alph <= 0.0f or isOffScreen(m_passive_sprites.at(i).getPos(), (g_MAX_DIM + std::max(w, h)) * g_BG_DENSITY * m_passive_sprites.at(i).getZ() / g_ZOOM_LEVEL) or m_passive_sprites.at(i).getDim() <= 0.0f)
    {
      //std::cout <<"popit" << std::endl;
      swapnpop(&m_passive_sprites, i);
      continue;
    }
    alph *= 0.9f;
    alph -= 0.2f;
    m_passive_sprites.at(i).setCol(3, alph);
  }

  if(g_DIFFICULTY == 0) return;
  if(rand() % 1024 <= g_DIFFICULTY * m_gameplay_intensity and m_factionCounts[GALACTIC_FEDERATION] < clamp(m_factionMaxCounts[GALACTIC_FEDERATION],0,200))
  {
    int reps = clamp(rand() % (g_DIFFICULTY * 5) + 1, 1, clamp(m_factionMaxCounts[GALACTIC_FEDERATION],0,80) - m_factionCounts[GALACTIC_FEDERATION]);
    aiTeam pteam;
    if(rand() % 100 < 55) pteam = SPOOKY_SPACE_PIRATES;
    else pteam = GALACTIC_FEDERATION;
    spawnSquad(pteam, 10000.0f, 20000.0f, reps);
  }
  if(rand() % 256 == 0 and atMaxCount(TEAM_PLAYER))
  {
    spawnShip(TEAM_PLAYER);
  }
  if(rand() % 256 == 0 and atMaxCount(TEAM_PLAYER_MINER))
  {
    spawnShip(TEAM_PLAYER_MINER);
  }
  if(rand() % 1024 == 0 and m_asteroids.size() < 10)
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
    a.update(_dt);
    m_asteroids.push_back(a);
  }
}

#if RENDER_MODE == 0
void universe::draw(float _dt)
{	
  //std::cout << 1/_dt << " fps" << std::endl;
  //std::cout << m_passive_sprites.size() << std::endl;
  if(m_paused) _dt = 0.0f;

  m_drawer.clear();

  for(auto i = m_dots.begin(); i != m_dots.end(); ++i)
  {
    if(i->getZ() > 1) continue;

    vec2 ipos = i->getInterpolatedPosition(_dt);
    vec2 ivel = (i->getVel() + i->getWVel()) * i->getZ();
    std::array<float, 4> icol = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
    m_drawer.drawLine(ipos, ipos + ivel, icol);
  }

  m_drawer.setBlendMode(SDL_BLENDMODE_ADD);
  for(auto i = m_sparkles.begin(); i != m_sparkles.end(); ++i)
  {
    if(i->getZ() <= 1)
    {
      vec2 ipos = i->getInterpolatedPosition(_dt);
      std::array<float, 4> col = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
      m_drawer.drawTexture( i->getTex(), 0, ipos, i->getAng(), col );
    }
  }
  m_drawer.setBlendMode(SDL_BLENDMODE_BLEND);

  for(auto i = m_passive_sprites.begin(); i != m_passive_sprites.end(); ++i)
  {
    if(!m_paused) i->incrDim();
    vec2 ipos = i->getInterpolatedPosition(_dt);
    std::array<float, 4> col = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
    m_drawer.drawTexture( i->getTex(), 0, ipos, i->getAng(), col );
  }

  for(auto i = m_shots.begin(); i != m_shots.end(); ++i)
  {
    vec2 ipos = i->getInterpolatedPosition(_dt);
    vec2 ivel = (i->getVel() + i->getWVel()) * 3;
    std::array<float, 4> iscol = {i->getCol(0), i->getCol(1), i->getCol(2), 255};
    std::array<float, 4> iecol = {iscol[0] / 2, iscol[1] / 2, iscol[2] / 2, 20};
    m_drawer.drawLineGr(ipos, ipos + ivel, iecol, iscol);
  }

  for(auto i = m_asteroids.begin(); i != m_asteroids.end(); ++i)
  {
    vec2 ipos = i->getInterpolatedPosition(_dt);
    std::array<float, 4> icol = {255, 255, 255, 255};
    m_drawer.drawTexture(i->getIdentifier(), 0, ipos, i->getAng(), icol);
  }

  for(auto i = m_agents.begin(); i != m_agents.end(); ++i)
  {
    vec2 ipos = i->getInterpolatedPosition(_dt);
    std::array<float, 4> ialpha = i->getAlphaStats();
    m_drawer.drawTextureSet(i->getIdentifier(), ipos, i->getAng(), ialpha);
  }

  if(!g_GAME_OVER)
  {
    vec2 ppos = m_ply.getInterpolatedPosition(_dt);
    std::array<float, 4> palpha = m_ply.getAlphaStats();
    m_drawer.drawTextureSet(m_ply.getIdentifier(), ppos, m_ply.getAng(), palpha);
  }

  for(auto i = m_missiles.begin(); i != m_missiles.end(); ++i)
  {
    vec2 ipos = i->getInterpolatedPosition(_dt);
    std::array<float, 4> ialpha = i->getAlphaStats();
    m_drawer.drawTextureSet(i->getIdentifier(), ipos, i->getAng(), ialpha);
  }

  for(auto i = m_particles.begin(); i != m_particles.end(); ++i)
  {
    vec2 ipos = i->getPos();
    std::array<float, 4> col = {i->getCol(0), i->getCol(1), i->getCol(2), i->getAlpha()};

    m_drawer.drawTexture(i->getIdentifier(), 0, ipos, 0, col);
    int k = 0;
    for(auto j = i->getParticles()->begin(); j != i->getParticles()->end(); ++j)
    {
      vec2 jpos = j->getInterpolatedPosition(_dt);
      col[3] = i->getAlpha(k);
      m_drawer.drawLine(jpos, jpos + j->getVel(), col);
      k++;
    }
  }

  for(auto i = m_dots.begin(); i != m_dots.end(); ++i)
  {
    if(i->getZ() <= 1) continue;
    vec2 ipos = i->getInterpolatedPosition(_dt);
    vec2 ivel = (i->getVel() + i->getWVel()) * i->getZ();
    std::array<float, 4> icol = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
    m_drawer.drawLine(ipos, ipos + ivel, icol);
  }

  m_drawer.setBlendMode(SDL_BLENDMODE_ADD);
  for(auto i = m_sparkles.begin(); i != m_sparkles.end(); ++i)
  {
    if(i->getZ() > 1)
    {
      vec2 ipos = i->getInterpolatedPosition(_dt);
      std::array<float, 4> icol = {i->getCol(0), i->getCol(1), i->getCol(2), i->getCol(3)};
      m_drawer.drawTexture( i->getTex(), 0, ipos, i->getAng(), icol );

    }
  }
  m_drawer.setBlendMode(SDL_BLENDMODE_BLEND);

  /*if(g_DEV_MODE)
    {
        for(auto i = m_partitions.rects.begin(); i != m_partitions.rects.end(); ++i)
        {
            std::array<int, 4> col = {255, 0, 0, 255};
            m_drawer.drawRect({static_cast<float>(i->x), static_cast<float>(i->y)}, {static_cast<float>(i->w), static_cast<float>(i->h)}, col, true);
        }
    }*/

  int mx = 0, my = 0;
  SDL_GetMouseState(&mx, &my);
  vec2 dpos = {static_cast<float>(mx), static_cast<float>(my)};
  dpos -= g_HALFWIN;
  dpos /= g_ZOOM_LEVEL;

  switch(m_mouse_state)
  {
  case 7:
    m_drawer.drawTexture("PLAYER_TURRET", 0, dpos, 0, {255, 255, 255, 100});
    m_drawer.drawTexture("PLAYER_TURRET", 5, dpos, 0, {255, 255, 255, 100});
    break;
  case 8:
    m_drawer.drawTexture("PLAYER_GRAVWELL", 0, dpos, 0, {255, 255, 255, 100});
    break;
  case 9:
    m_drawer.drawTexture("PLAYER_BARRACKS", 0, dpos, 0, {255, 255, 255, 100});
    break;
  case 10:
    m_drawer.drawTexture("PLAYER_STATION", 0, dpos, 0, {255, 255, 255, 100});
    break;
  default:
    break;
  }

  //Draw the ui
  drawUI();

  m_drawer.finalise();
}

void universe::drawUI()
{
  if(!g_GAME_OVER)
  {
    m_drawer.drawText("SCORE: " + std::to_string( m_score ),"pix",{260, 2});
    m_drawer.drawText("MISSILES: " + std::to_string( m_ply.getMissiles() ),"pix",{260, 20});

    m_drawer.drawMap(&m_missiles, &m_agents, &m_asteroids, &m_shots, &m_factions);
    m_drawer.statusBars(&m_ply);
    m_drawer.drawWeaponStats(&m_ply);
  }

  for(auto i = m_ui.getElements()->begin(); i != m_ui.getElements()->end(); ++i)
  {
    if(!i->isVisible()) continue;
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

#elif RENDER_MODE == 1
void universe::draw(float _dt)
{
  m_drawer.clear();

  m_drawer.useShader("background");
  m_drawer.drawBackground(m_time_elapsed, m_pos);

  m_drawer.useShader("plain");
  m_drawer.drawTri(m_ply.getPos(), 20.0f, m_ply.getAng(), {1.0f,1.0f,1.0f,1.0f});

  for(auto &i : m_agents)
  {
    m_drawer.drawTri(i.getPos(), 20.0f, i.getAng(), {1.0f,0.0f,0.0f,1.0f});
  }

  for(auto &i : m_shots)
  {
    vec2 ipos = i.getInterpolatedPosition(_dt);
    vec2 ivel = (i.getVel() + i.getWVel()) * 3;
    m_drawer.drawLine(i.getPos(), i.getPos() + ivel);
  }

  drawUI();

  m_drawer.swapWindow();
}

void universe::drawUI()
{

}
#endif

void universe::detectCollisions(
    SDL_Rect _box,
    std::vector<enemy*> _ships,
    std::vector<laser*> _lasers,
    std::vector<missile*> _rockets,
    std::vector<ship*> _rocks,
    unsigned short int _lvl
    )
{
  size_t count = 0;
  std::vector<enemy*> pships;
  std::vector<laser*> plasers;
  std::vector<missile*> prockets;
  std::vector<ship*> procks;

  vec2 boxPos = {static_cast<float>(_box.x), static_cast<float>(_box.y)};
  vec2 boxDim = {static_cast<float>(_box.w), static_cast<float>(_box.h)};

  for(auto& i: _ships)
  {
    if( circleIntersectRect(i->getPos(), i->getRadius(), boxPos, boxDim ) )
    {
      pships.push_back(i);
      count++;
    }
  }

  for(auto &i : _rockets)
  {
    if( circleIntersectRect(i->getPos(), i->getRadius(), boxPos, boxDim ) )
    {
      prockets.push_back(i);
      count++;
    }
  }

  for(auto &i : _rocks)
  {
    if( circleIntersectRect(i->getPos(), i->getRadius(), boxPos, boxDim ) )
    {
      procks.push_back(i);
      count++;
    }
  }

  for(auto &i : _lasers)
  {
    vec2 p = i->getPos();
    vec2 v = p + i->getVel();

    int sx = p.m_x, sy = p.m_y, ex = v.m_x, ey = v.m_y;
    if( SDL_IntersectRectAndLine(&_box, &sx, &sy, &ex, &ey ) )
    {
      plasers.push_back(i);
      count++;
    }
  }

  if(count < 40 or _lvl > 4 or _lasers.size() == 0 or ( _ships.size() == 0 and _rockets.size() == 0 and _rocks.size() == 0 ))
  {
    m_partitions.ships.push_back(pships);
    m_partitions.lasers.push_back(plasers);
    m_partitions.rockets.push_back(prockets);
    m_partitions.rocks.push_back(procks);
    m_partitions.rects.push_back(_box);
  }
  else
  {
    int x = _box.x, y = _box.y, w = _box.w / 2, h = _box.h / 2;
    SDL_Rect r1 = {x, 		y, 		w, 		h};
    SDL_Rect r2 = {x + w, 	y, 		w, 		h};
    SDL_Rect r3 = {x, 		y + h, 	w, 		h};
    SDL_Rect r4 = {x + w, 	y + h, 	w, 		h};

    detectCollisions(r1, pships, plasers, prockets, procks, _lvl + 1);
    detectCollisions(r2, pships, plasers, prockets, procks, _lvl + 1);
    detectCollisions(r3, pships, plasers, prockets, procks, _lvl + 1);
    detectCollisions(r4, pships, plasers, prockets, procks, _lvl + 1);
  }
}

void universe::checkCollisions()
{
  for(size_t p = 0; p < m_partitions.ships.size(); ++p)
  {
    bool done = false;
    int harm = 0;
    for(int l = m_partitions.lasers.at(p).size() - 1; l >= 0; --l)
    {
      vec2 sp = m_partitions.lasers.at(p).at(l)->getPos();
      vec2 sv = m_partitions.lasers.at(p).at(l)->getVel();
      vec2 spv = sp + sv * 1.5;
      float stop = m_partitions.lasers.at(p).at(l)->getStop();

      vec2 ep;
      vec2 ev;
      float er;

      //cout << "ENEMY CHECK" << endl;
      for(int s = m_partitions.ships.at(p).size() - 1; s >= 0; s--)
      {
        if(m_partitions.lasers.at(p).at(l)->getTeam() == TEAM_PLAYER_MINER or !emnityCheck(m_partitions.lasers.at(p).at(l)->getTeam(), m_partitions.ships.at(p).at(s)->getTeam())) continue;
        harm = 0;

        ep = m_partitions.ships.at(p).at(s)->getPos();
        ev = m_partitions.ships.at(p).at(s)->getVel();
        er = m_partitions.ships.at(p).at(s)->getRadius();

        if(lineIntersectCircle(sp, spv, ep, er))
        {
          addpfx(ep + randVec(er), ev, m_vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
          harm = m_partitions.lasers.at(p).at(l)->getDmg();

          //Delete m_shots if they match the ones in the universe vector.
          for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots.at(i) == m_partitions.lasers.at(p).at(l)) swapnpop(&m_shots, i);
          //Pop the partition pointer.
          swapnpop(&m_partitions.lasers.at(p), l);
          done = true;
        }
        if(harm > 0)
        {
          m_partitions.ships.at(p).at(s)->damage(harm);
          m_partitions.ships.at(p).at(s)->addVelS( (sv - ev) * stop );
          break;
        }
      }
      if(done) continue;

      for(int r = m_partitions.rocks.at(p).size() - 1; r >= 0; r--)
      {
        //cout << "R is " << r << ") " << m_partitions.rocks.at(p).at(r) << endl;
        harm = 0;

        ep = m_partitions.rocks.at(p).at(r)->getPos();
        ev = m_partitions.rocks.at(p).at(r)->getVel();
        er = m_partitions.rocks.at(p).at(r)->getRadius();

        if(lineIntersectCircle(sp, spv, ep, er))
        {
          addpfx(ep + randVec(er), ev, m_vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
          for(int q = 0; q < 20; ++q) addParticleSprite(ep, ev + randVec(6.0f), 0.0f, "SMOKE");
          harm = m_partitions.lasers.at(p).at(l)->getDmg();

          for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots.at(i) == m_partitions.lasers.at(p).at(l)) swapnpop(&m_shots, i);//m_shots.erase(m_shots.begin() + i);
          swapnpop(&m_partitions.lasers.at(p), l);

          done = true;
        }
        if(harm > 0)
        {
          m_partitions.rocks.at(p).at(r)->damage(harm);
          m_partitions.rocks.at(p).at(r)->addVel( (sv - ev) * stop );
          break;
        }
      }
      if(done) continue;

      //cout << "ENEMY CHECK" << endl;
      for(int m = m_partitions.rockets.at(p).size() - 1; m >= 0; m--)
      {
        if(m_partitions.lasers.at(p).at(l)->getTeam() == TEAM_PLAYER_MINER) continue;
        harm = 0;

        ep = m_partitions.rockets.at(p).at(m)->getPos();
        ev = m_partitions.rockets.at(p).at(m)->getVel();
        er = m_partitions.rockets.at(p).at(m)->getRadius();

        if(lineIntersectCircle(sp, spv, ep, er))
        {
          addpfx(ep + randVec(er), ev, m_vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));

          harm = m_shots.at(l).getDmg();

          for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots.at(i) == m_partitions.lasers.at(p).at(l)) m_shots.erase(m_shots.begin() + i);
          swapnpop(&m_partitions.lasers.at(p), l);

          done = true;
        }
        if(harm > 0)
        {
          m_partitions.rockets.at(p).at(m)->damage(harm);
          m_partitions.rockets.at(p).at(m)->addVel( (sv - ev) * stop );
          break;
        }
      }
      if(done) continue;

      if(m_partitions.lasers.at(p).at(l)->getTeam() == TEAM_PLAYER_MINER) continue;

      harm = 0;

      vec2 dp = sp - m_ply.getPos();
      vec2 dv = sv + m_ply.getVel();
      if(fabs(dv.m_x) > fabs(dp.m_x) - 32 and fabs(dv.m_y) > fabs(dp.m_y) - 32 and m_partitions.lasers.at(p).at(l)->getTeam() != TEAM_PLAYER)
      {
        vec2 spv = sp + sv;
        if(lineIntersectCircle(sp, spv, dp, m_ply.getRadius()))
        {
          playSnd(RICOCHET_SND);
          addpfx(sp + randVec(32.0f), m_ply.getVel(), m_vel, randFloat(3.0f, 8.0f), randFloat(3.0f, 8.0f));
          harm = m_partitions.lasers.at(p).at(l)->getDmg();

          for(int i = m_shots.size() - 1; i >= 0; --i) if(&m_shots.at(i) == m_partitions.lasers.at(p).at(l)) swapnpop(&m_shots, i);
          swapnpop(&m_partitions.lasers.at(p), l);

          done = true;
        }
        if(harm > 0)
        {
          m_ply.damage(harm);
          //std::cout << "ADDING VEL (" << sv.m_x << "," << sv.m_y << " - " << ev.m_x << "," << ev.m_y << ") * " << stop << " = (" << ( (sv - ev) * stop ).m_x << ", " << ( (sv - ev) * stop ).m_y << ")" << std::endl;
          m_ply.addVel( (sv - ev) * stop );
          setVel(-m_ply.getVel());
        }
      }

      //Space-station healing check.
      for(size_t i = 0; i < m_partitions.ships.at(p).size(); ++i)
      {
        enemy * a = m_partitions.ships.at(p).at(i);
        for(size_t j = 0; j < m_partitions.ships.at(p).size(); ++j)
        {
          enemy * b = m_partitions.ships.at(p).at(j);

          //If a is a station, and b the two ships are on the same team...
          if( a->getClassification() == PLAYER_STATION and !emnityCheck( a->getTeam(), b->getTeam() ) and a != b )
          {
            float dist = magns( b->getPos() - a->getPos() );
            if(dist < 160000.0f) b->incrHealth(0.01f);
          }
        }

        if( a->getClassification() == PLAYER_STATION and !emnityCheck( a->getTeam(), TEAM_PLAYER ) )
        {
          float dist = magns( m_ply.getPos() - a->getPos() );
          if(dist < 160000.0f) m_ply.incrHealth(0.01f);
        }
      }
    }
  }
}

void universe::addpfx(
    const vec2 _p,
    const vec2 _v,
    const vec2 _wv,
    const int _no,
    const float _f
    )
{
  pfx pf(_p, _v, _wv, _no, _f, "EXPLOSION");
  m_particles.push_back(pf);
}

ship * universe::closestEnemy(vec2 p, aiTeam t)
{
  float best_dist = F_MAX;
  ship * r = nullptr;
  for(size_t i = 0; i < m_agents.size(); i++)
  {
    if(!emnityCheck(m_agents.at(i).getTeam(), t)) continue;
    vec2 pe = m_agents.at(i).getPos();
    float dist = magns(pe-p);

    if(dist < best_dist)
    {
      best_dist = dist;
      r = m_agents.at(i).getSelf();
    }
  }
  return r;
}

void universe::addParticleSprite(
    const vec2 _p,
    const vec2 _v,
    const float _m,
    const std::string _tex
    )
{
  float col;
  if( _tex == "SMOKE" ) col = randFloat(200.0f, 220.0f) * (1.0f - _m);
  else if( _tex == "EXPLOSION" ) col = 255.0f;

  int w = 0, h = 0;
  m_drawer.queryTexture(_tex, 0, &w, &h);
  stardust_sprite sm (_tex, col, w, h);
  sm.setPos(_p);
  if( _tex == "SMOKE" ) sm.setVel(_v + randVec(1.0f));
  else if( _tex == "EXPLOSION" ) sm.setVel(_v);
  sm.setZ(1.0f);
  m_passive_sprites.push_back(sm);
}

void universe::spawnShip(const aiTeam _t)
{	
  int side = rand()%4;
  vec2 pass;

  if(side == 0) pass = {randFloat(-20000.0f,20000.0f), -20000.0f};
  else if(side == 1) pass = {-20000.0f,randFloat(-20000.0f, 20000.0f)};
  else if(side == 2) pass = {randFloat(-20000.0f, 20000.0f), 20000.0f};
  else if(side == 3) pass = {20000.0f, randFloat(-20000.0f, 20000.0f)};

  spawnShip(_t, pass);
}

void universe::spawnShip(
    const aiTeam _t,
    const vec2 _p
    )
{	
  int prob = rand()%100;

  ship_spec type;

  if( _t == SPOOKY_SPACE_PIRATES )
  {
    type = PIRATE_GNAT;
    if(prob > 75 and prob <= 80) type = PIRATE_CRUISER;
    else if(prob > 80 and prob <= 90) type = PIRATE_WRANGLER;
    else if(prob > 90 and prob <= 98) type = PIRATE_MARAUDER;
    else if(prob == 99) type = PIRATE_GUNSHIP;
  }
  else if( _t == GALACTIC_FEDERATION )
  {
    type = FEDERATION_MKI;
    if(prob > 75 and prob <= 80) type = FEDERATION_MKII;
    else if(prob > 80 and prob <= 90) type = FEDERATION_MKIII;
    else if(prob > 90 and prob <= 98) type = FEDERATION_MKIV;
    else if(prob == 99) type = FEDERATION_GUNSHIP;
  }
  else if( _t == TEAM_PLAYER )
  {
    type = PLAYER_HUNTER;
    if(prob > 50 and prob <= 80) type = PLAYER_DEFENDER;
    if(prob > 80 and prob <= 99) type = PLAYER_DESTROYER;
  }
  else if( _t == TEAM_PLAYER_MINER )
  {
    type = PLAYER_MINER_DROID;
  }

  enemy newShip({0.0f, 0.0f}, -m_vel, type, _t);

  if( _t == TEAM_PLAYER ) m_factionCounts[TEAM_PLAYER]++;
  else if( _t == TEAM_PLAYER_MINER ) m_factionCounts[TEAM_PLAYER_MINER]++;
  else if( _t == SPOOKY_SPACE_PIRATES or _t == GALACTIC_FEDERATION )  m_factionCounts[GALACTIC_FEDERATION]++;

  newShip.setPos(_p);

  //Do not assign squad if enemy can move and shoot.
  if(!newShip.getCanMove() or !newShip.getCanShoot() or _t == TEAM_PLAYER_MINER)
  {
    m_agents.push_back(newShip);
    return;
  }

  //Assign squad if enemy can move and shoot.
  for(auto &s : m_squads)
  {
    if(s.m_team == _t and s.m_size < s.m_max_size and rand() % 100 < 90)
    {
      addToSquad(&newShip, &s);
      m_agents.push_back(newShip);
      return;
    }
  }

  //Create a new squad if no suitable one exists.
  squad temp = createSquad(_t);
  temp.m_id = m_squads.size();
  addToSquad(&newShip, &temp);
  m_squads.push_back(temp);
  m_agents.push_back(newShip);
}

void universe::spawnSquad(
    const aiTeam _t,
    const float _min,
    const float _max,
    const int _i)
{
  vec2 p = randVec(_min, _max);
  for(int i = 0; i < _i; ++i)
  {
    spawnShip(_t, p + randVec(512.0f));
  }
}

bool emnityCheck(
    aiTeam _a,
    aiTeam _b
    )
{
  if(
     (_a == _b) or
     (_a == NEUTRAL or _b == NEUTRAL) or
     (
       (_a == TEAM_PLAYER and _b == TEAM_PLAYER_MINER) or
       (_a == TEAM_PLAYER_MINER and _b == TEAM_PLAYER)
       )
     )
  {
    return false;
  }
  return true;
}

void universe::reload(const bool _newGame)
{
  m_agents.clear();
  m_missiles.clear();
  m_shots.clear();
  m_asteroids.clear();
  m_passive_sprites.clear();

  m_vel = {0,0};
  m_ply.setHealth( m_ply.getMaxHealth() );
  m_ply.setShield( m_ply.getMaxShield() );
  m_ply.setEnergy( m_ply.getMaxEnergy() );
  m_ply.setVel({0, 0});

  m_ui.clear();
  initUI();
  m_ui.reset();
  m_paused = false;

  m_partitions.ships.clear();
  m_partitions.lasers.clear();
  m_partitions.rockets.clear();
  m_partitions.rocks.clear();
  m_partitions.rects.clear();

  m_time_elapsed = 0.0;
  setVel({0,0});

  m_factionCounts.assign(6, 0);
  m_factionMaxCounts.assign(6, 0);

  m_mouse_state = -1;

  m_escMenuShown = false;

  m_factionMaxCounts[GALACTIC_FEDERATION] = 3;

  if(!_newGame) return;

  for(int i = 0; i < UPGRADES_LEN; ++i)
  {
    m_ply.setGradeArr(i, 0);
  }
  m_ply.setMissiles(3);
  setScore(0);
}

void universe::addBuild(const ship_spec _type)
{
  vec2 pass;

  int side = rand() % 4;
  if(side == 0) pass = {randFloat(-20000.0f,20000.0f), -20000.0f};
  else if(side == 1) pass = {-20000.0f,randFloat(-20000.0f, 20000.0f)};
  else if(side == 2) pass = {randFloat(-20000.0f, 20000.0f), 20000.0f};
  else if(side == 3) pass = {20000.0f, randFloat(-20000.0f, 20000.0f)};

  addBuild(pass, _type);
}

void universe::addBuild(
    const vec2 _p,
    const ship_spec _type
    )
{
  enemy newShip(_p, {0.0f, 0.0f}, _type, TEAM_PLAYER);
  m_agents.push_back(newShip);

  switch(_type)
  {
  case PLAYER_STATION:
    m_factionMaxCounts[GALACTIC_FEDERATION] += 25;
    break;
  case PLAYER_GRAVWELL:
    m_factionMaxCounts[GALACTIC_FEDERATION] += 5;
    break;
  case PLAYER_TURRET:
    m_factionMaxCounts[GALACTIC_FEDERATION] += 1;
    break;
  case PLAYER_BARRACKS:
    m_factionMaxCounts[GALACTIC_FEDERATION] += 20;
    break;
  default:
    break;
  }
  addpfx(_p, {0,0}, {0,0}, rand()%20 + 50, rand()%30 + 2);
  for(int q = 0; q < 50; ++q) addParticleSprite(_p, randVec(6.0f), 0.0f, "SMOKE");
  playSnd(PLACE_SND);
}

void universe::initUI()
{
  using ui::selection;
  using ui::button;
  //Initialise the two selection menus.
  selection energy_menu;
  selection upgrades_menu;

  //Add buttons to the energy menu.
  std::array<int, 8> arr1 = {20,20,20,200,100,100,100,255};
  std::array<int, 8> arr2 = {100,100,100,200,250,250,250,255};
  button energy_menu_neutral("BALANCED",arr1,arr2,{g_WIN_WIDTH * 0.9f, g_WIN_HEIGHT * 0.35f},{128.0f,64.0f});
  energy_menu_neutral.set(true);
  energy_menu.add(energy_menu_neutral);

  arr1 = {12,24,26,200,27,95,232,255};
  arr2 = {45,67,188,200,119,156,238,255};
  button energy_menu_shields("SHIELDS",arr1,arr2,{g_WIN_WIDTH * 0.9f, g_WIN_HEIGHT * 0.45f},{128.0f,64.0f});
  energy_menu.add(energy_menu_shields);

  arr1 = {14,35,20,200,36,204,52,255};
  arr2 = {65,127,64,200,129,241,127,255};
  button energy_menu_engines("ENGINES",arr1,arr2,{g_WIN_WIDTH * 0.9f, g_WIN_HEIGHT * 0.55f},{128.0f,64.0f});
  energy_menu.add(energy_menu_engines);

  arr1 = {35,23,23,200,232,31,31,255};
  arr2 = {124,33,33,200,217,116,116,255};
  button energy_menu_guns("GUNS",arr1,arr2,{g_WIN_WIDTH * 0.9f, g_WIN_HEIGHT * 0.65f},{128.0f,64.0f});
  energy_menu.add(energy_menu_guns);

  //Add buttons to the upgrades menu.
  float w = 150.0f, h = 50.0f;
  arr1 = {100,50,50,200,250,200,200,255};
  arr2 = {100,50,50,200,250,200,200,255};
  button upgrades_lasers("LASERS I (4)",arr1,arr2,{g_WIN_WIDTH * 0.0f, g_WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_lasers);

  arr1 = {50,50,100,200,200,200,250,255};
  arr2 = {50,50,100,200,200,200,250,255};
  button upgrades_shields("SHIELDS I (4)",arr1,arr2,{g_WIN_WIDTH * 0.15f, g_WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_shields);

  arr1 = {50,100,50,200,200,250,200,255};
  arr2 = {50,100,50,200,200,250,200,255};
  button upgrades_generators("GENERATORS I (4)",arr1,arr2,{g_WIN_WIDTH * 0.3f, g_WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_generators);

  arr1 = {50,50,80,200,200,200,220,255};
  arr2 = {50,50,80,200,200,200,220,255};
  button upgrades_thrusters("THRUSTERS I (4)",arr1,arr2,{g_WIN_WIDTH * 0.45f, g_WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_thrusters);

  arr1 = {255,210,0,200,255,253,100,255};
  arr2 = {255,210,0,200,255,253,100,255};
  button upgrades_m_missiles("MISSILE (4)",arr1,arr2,{g_WIN_WIDTH * 0.6f, g_WIN_HEIGHT * 0.85f},{w,h},4);
  upgrades_menu.add(upgrades_m_missiles);

  arr1 = {100,210,255,200,180,220,255,255};
  arr2 = {100,210,255,200,180,220,255,255};
  button upgrades_miner("MINER (16)",arr1,arr2,{g_WIN_WIDTH * 0.75f, g_WIN_HEIGHT * 0.85f},{w,h},16);
  upgrades_menu.add(upgrades_miner);

  button upgrades_wingman("WINGMAN (32)",arr1,arr2,{g_WIN_WIDTH * 0.75f, g_WIN_HEIGHT * 0.79f},{w,h},32);
  upgrades_menu.add(upgrades_wingman);

  button upgrades_turret("TURRET (32)",arr1,arr2,{g_WIN_WIDTH * 0.75f, g_WIN_HEIGHT * 0.73f},{w,h},32);
  upgrades_menu.add(upgrades_turret);

  button upgrades_gravwell("GRAVWELL (512)",arr1,arr2,{g_WIN_WIDTH * 0.75f, g_WIN_HEIGHT * 0.67f},{w,h},512);
  upgrades_menu.add(upgrades_gravwell);

  button upgrades_barracks("BARRACKS (1024)",arr1,arr2,{g_WIN_WIDTH * 0.75f, g_WIN_HEIGHT * 0.61f},{w,h},1024);
  upgrades_menu.add(upgrades_barracks);

  button upgrades_station("STATION (1024)",arr1,arr2,{g_WIN_WIDTH * 0.75f, g_WIN_HEIGHT * 0.55f},{w,h},1024);
  upgrades_menu.add(upgrades_station);

  m_ui.add(energy_menu);
  m_ui.add(upgrades_menu);
}

bool universe::upgradeCallback(
    int _sel,
    int _btn
    )
{
  //This function takes the selected button, looks at the cost vs the score, updates relevant values,
  //then returns a bool representing whether the upgrade was successful or unsuccessful.

  ui::button * selectedButton = &m_ui.getElements()->at(_sel).getButtons()->at(_btn);
  int lvl = m_ply.getUpgrade( _btn );

  selectedButton->set(false);

  if(selectedButton->getCost() > m_score or m_ply.getUpgrade(_btn) > 9) return false;

  if(lvl < 9)
  {
    addScore( -selectedButton->getCost() );
    if(_btn < 4) selectedButton->setCost(selectedButton->getCost() * 2);
  }

  upgradeSetLabels(_sel, _btn, -1);
  if(_btn > 3) return true;

  return true;
}

void universe::upgradeSetLabels(
    int _sel,
    int _btn,
    int _plvl
    )
{
  ui::button * selectedButton = &m_ui.getElements()->at(_sel).getButtons()->at(_btn);

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

  s1 += g_ROMAN_NUMS.at(lvl);

  if(lvl < 8)
  {
    s1 += " (";
    std::stringstream ss;
    ss << selectedButton->getCost();
    s1 += ss.str();
    s1 += ")";
  }
  if(lvl < 9) selectedButton->updateText(s1);
  std::cout << "UPDATING... " << s1 << std::endl;
}

//This function loads all the ships in the game into a vector that we can copy from later.
void universe::loadShips()
{
  for(int i = 0; i < static_cast<int>(SHIPS_END); ++i)
  {
    ship insert( {0.0f, 0.0f}, static_cast<ship_spec>(i), m_drawer.getTextureRadius(getTextureKey(static_cast<ship_spec>(i))) );
    g_ship_templates.push_back(insert);
  }
  std::cout << "No of ship types: " << g_ship_templates.size() << std::endl;
}

void universe::createFactions()
{
  faction player;
  player.m_colour = {0, 255, 0};
  m_factions.push_back(player);

  faction player_miner;
  player_miner.m_colour = {0, 255, 0};
  m_factions.push_back(player_miner);

  faction galactic_fed;
  galactic_fed.m_colour = {165, 14, 226};
  m_factions.push_back(galactic_fed);

  faction spooky_pirates;
  spooky_pirates.m_colour = {240, 211, 10};
  m_factions.push_back(spooky_pirates);

  faction neutral;
  neutral.m_colour = {200, 200, 200};
  m_factions.push_back(neutral);

  faction none;
  none.m_colour = {0, 0, 0};
  m_factions.push_back(none);
}

squad * universe::getSquadFromID(int _id)
{
  for(auto &s : m_squads)
  {
    if(s.m_id == _id)
    {
      return &s;
    }
  }
  return nullptr;
}
