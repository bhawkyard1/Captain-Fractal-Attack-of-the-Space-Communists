#include "ship.hpp"
#include "weapons.hpp"

ship_spec decrSpec(ship_spec s) {return static_cast<ship_spec>( static_cast<int>(s) - 1 );}

ship::ship(vec2 p, ship_spec ptype)
{
	coolDown = 0.0f;
	shooting = false;
	
	setPos(p);
	setPPos(p);
	
	setVel({0.0f,0.0f});
	setWVel({0.0f,0.0f});
	angle = 0.0f;
	targetAngle = 0.0f;
	
  std::vector<SDL_Surface*> ship_surf;
	
	engineGlow = 0.0f;
	steeringGlow = 0.0f;
	shieldGlow = 0.0f;
	drawShot = 0.0f;
	angVel = 0.0f;
	
	canMove = true;
	canShoot = true;
	
	curWeap = 0;
	
	switch(ptype)
	{
		case FEDERATION_MKI:
      m_identifier = "FEDERATION_MKI";
			setMaxHealth(50.0f,true);
			setMaxShield(30.0f,true);
			setMaxEnergy(100.0f,true);
			inertia = 0.1f;
      weapons.push_back( weapons[rand() % 3 + 4] );
      curWeap = 0;
			break;
		case FEDERATION_MKII:
      m_identifier = "FEDERATION_MKII";
			setMaxHealth(90.0f,true);
			setMaxShield(60.0f,true);
			setMaxEnergy(100.0f,true);
			inertia = 0.09f;
      weapons.push_back( weapons[rand() % 3 + 4] );
      curWeap = 0;
			break;
		case FEDERATION_MKIII:
      m_identifier = "FEDERATION_MKIII";
			setMaxHealth(120.0f,true);
			setMaxShield(90.0f,true);
			setMaxEnergy(150.0f,true);
			inertia = 0.08f;
      weapons.push_back( weapons[rand() % 3 + 4] );
      curWeap = 0;
			break;
		case FEDERATION_MKIV:
      m_identifier = "FEDERATION_MKIV";
			setMaxHealth(150.0f,true);
			setMaxShield(120.0f,true);
			setMaxEnergy(200.0f,true);
			inertia = 0.15f;
      weapons.push_back( weapons[rand() % 3 + 4] );
      curWeap = 0;
			break;
		case FEDERATION_GUNSHIP:
      m_identifier = "FEDERATION_GUNSHIP";
			setMaxHealth(500.0f,true);
			setMaxShield(200.0f,true);
			setMaxEnergy(500.0f,true);
			inertia = 0.02f;
      weapons.push_back( weapons[rand() % 2 + 7] );
      curWeap = 0;
      break;
		case PIRATE_GNAT:
      m_identifier = "PIRATE_GNAT";
			setMaxHealth(80.0f,true);
			setMaxShield(40.0f,true);
			setMaxEnergy(90.0f,true);
			inertia = 0.15f;
      weapons.push_back( weapons[rand() % 3 + 14] );
      curWeap = 0;
			break;
		case PIRATE_CRUISER:
      m_identifier = "PIRATE_CRUISER";
			setMaxHealth(100.0f,true);
			setMaxShield(60.0f,true);
			setMaxEnergy(100.0f,true);
			inertia = 0.1f;
      weapons.push_back( weapons[rand() % 3 + 14] );
      curWeap = 0;
			break;
		case PIRATE_WRANGLER:
      m_identifier = "PIRATE_WRANGLER";
			setMaxHealth(140.0f,true);
			setMaxShield(70.0f,true);
			setMaxEnergy(120.0f,true);
			inertia = 0.05f;
      weapons.push_back( weapons[rand() % 3 + 14] );
      curWeap = 0;
			break;
		case PIRATE_MARAUDER:
      m_identifier = "PIRATE_MARAUDER";
			setMaxHealth(120.0f,true);
			setMaxShield(120.0f,true);
			setMaxEnergy(120.0f,true);
			inertia = 0.15f;
      weapons.push_back( weapons[rand() % 3 + 14] );
      curWeap = 0;
			break;
		case PIRATE_GUNSHIP:
      m_identifier = "PIRATE_GUNSHIP";
			setMaxHealth(200.0f,true);
			setMaxShield(120.0f,true);
			setMaxEnergy(150.0f,true);
			inertia = 0.05f;
      weapons.push_back( weapons[17] );
      curWeap = 0;
			break;
		case PLAYER_SHIP:
      m_identifier = "PLAYER_SHIP";
			setMaxHealth(100.0f,true);
			setMaxShield(100.0f,true);
			setMaxEnergy(100.0f,true);
			inertia = 0.07f;
      weapons.push_back( weapons[0] );
      weapons.push_back( weapons[1] );
      weapons.push_back( weapons[2] );
			curWeap = 0;
			break;
		case PLAYER_HUNTER:
      m_identifier = "PLAYER_HUNTER";
			setMaxHealth(100.0f,true); 
			setMaxShield(100.0f,true); 
			setMaxEnergy(100.0f,true);
			inertia = 0.07f;
      weapons.push_back( weapons[rand() % 3 + 9] );
      curWeap = 0;
			break;
		case PLAYER_DEFENDER:
      m_identifier = "PLAYER_DEFENDER";
			setMaxHealth(125.0f,true); 
			setMaxShield(125.0f,true); 
			setMaxEnergy(125.0f,true);
			inertia = 0.07f;
      weapons.push_back( weapons[rand() % 3 + 9] );
      curWeap = 0;
			break;
		case PLAYER_DESTROYER:
      m_identifier = "PLAYER_DESTROYER";
			setMaxHealth(150.0f,true); 
			setMaxShield(150.0f,true); 
			setMaxEnergy(150.0f,true);
			inertia = 0.07f;
      weapons.push_back( weapons[rand() % 3 + 9] );
      curWeap = 0;
			break;
		case PLAYER_MINER_DROID:
      m_identifier = "PLAYER_MINER_DROID";
			setMaxHealth(10.0f,true); 
			setMaxShield(20.0f,true); 
			setMaxEnergy(200.0f,true);
			inertia = 0.07f;
      weapons.push_back( weapons[12] );
      curWeap = 0;
			break;
		case ION_MISSILE_MKI:
      m_identifier = "ION_MISSILE_MKI";
			inertia = 0.1f;
			canShoot = false;
			break;
		case ASTEROID_SMALL:
      m_identifier = "ASTEROID_SMALL";
			setMaxHealth(500.0f,true); 
			setMaxShield(0.0f,true); 
			setMaxEnergy(0.0f,true);
			angVel = randFloat(-1.0f, 1.0f);
			inertia = 0.0f;
			canMove = false;
			canShoot = false;
			break;
		case ASTEROID_MID:
      m_identifier = "ASTEROID_MID";
			setMaxHealth(1000.0f,true); 
			setMaxShield(0.0f,true); 
			setMaxEnergy(0.0f,true);
			angVel = randFloat(-1.0f, 1.0f);
			inertia = 0.0f;
			canMove = false;
			canShoot = false;
			break;
		case ASTEROID_LARGE:
      m_identifier = "ASTEROID_LARGE";
			setMaxHealth(2000.0f,true); 
			setMaxShield(0.0f,true); 
			setMaxEnergy(0.0f,true);
			angVel = randFloat(-1.0f, 1.0f);
			inertia = 0.0f;
			canMove = false;
			canShoot = false;
			break;
		case PLAYER_TURRET:
      m_identifier = "PLAYER_TURRET";
			setMaxHealth(100.0f,true); 
			setMaxShield(200.0f,true); 
			setMaxEnergy(300.0f,true);
			inertia = 0.1f;
      weapons.push_back( weapons[13] );
      curWeap = 0;
			canMove = false;
			break;
		case PLAYER_STATION:
      m_identifier = "PLAYER_STATION";
			setMaxHealth(10000.0f,true); 
			setMaxShield(10000.0f,true); 
			setMaxEnergy(100000.0f,true);
			angVel = randFloat(-0.1f, 0.1f);
			inertia = 0.0f;
			canMove = false;
			canShoot = false;
			break;
		case PLAYER_GRAVWELL:
      m_identifier = "PLAYER_GRAVWELL";
			setMaxHealth(5000.0f,true); 
			setMaxShield(5000.0f,true); 
			setMaxEnergy(50000.0f,true);
			angVel = randFloat(-0.1f, 0.1f);
			inertia = 0.0f;
			canMove = false;
			canShoot = false;
			break;
		case PLAYER_BARRACKS:
      m_identifier = "PLAYER_BARRACKS";
			setMaxHealth(8000.0f,true); 
			setMaxShield(8000.0f,true); 
			setMaxEnergy(80000.0f,true);
			angVel = randFloat(-0.1f, 0.1f);
			inertia = 0.0f;
			canMove = false;
			canShoot = false;
			break;
		case SHIPS_END:
			break;
	}
	
	for(size_t i = 0; i < ship_surf.size(); ++i)
	{
		if(ship_surf.at(i) != nullptr)
		{
			shipTex.push_back( SDL_CreateTextureFromSurface( renderer, ship_surf.at(i) ) );
			SDL_FreeSurface(ship_surf.at(i));
		}
		else 
		{
      std::cout << SDL_GetError() << std::endl;
			shipTex.push_back( nullptr );
		}
	}
	
	priority = NONE;
	
	for(short unsigned int i = 0; i < UPGRADES_LEN; i++) upgrades[i] = 0;
	shieldMul = 1.0f;
	generatorMul = 1.0f;
	
	int w = 0, h = 0;
	SDL_QueryTexture(shipTex.at(0), NULL, NULL, &w, &h);
	
	w/=2;
	h/=2;
	
	dstrect.w = w;
	dstrect.h = h;
	
	classification = ptype;
	
	radius = w / 2;
}

ship::ship(ship &src, vec2 p)
{
	coolDown = 0.0f;
	shooting = false;
	
	setPos(p);
	setPPos(p);
	
	setVel({0.0f,0.0f});
	setWVel({0.0f,0.0f});
	angle = 0.0f;
	targetAngle = 0.0f;
	
	engineGlow = 0.0f;
	steeringGlow = 0.0f;
	shieldGlow = 0.0f;
	drawShot = 0.0f;
	
	canMove = !src.isStatic();
	canShoot = !src.isLaserless();
	
	setMaxHealth( src.getMaxHealth(), true );
	setMaxShield( src.getMaxShield(), true );
	setMaxEnergy( src.getMaxEnergy(), true );
	
	inertia = src.getInertia();
	angVel = src.getAngVel();

	shipTex = src.getTex();
	
	priority = NONE;
	
	for(short unsigned int i = 0; i < UPGRADES_LEN; i++) upgrades[i] = 0;
	shieldMul = 1.0f;
	generatorMul = 1.0f;
	
	int w = 0, h = 0;
	SDL_QueryTexture(shipTex.at(0), NULL, NULL, &w, &h);
	
	w/=2;
	h/=2;
	
	dstrect.w = w;
	dstrect.h = h;
	
	classification = src.getClassification();
	
	radius = src.getRadius();
	
	curWeap = src.getWeap();
	
	switch(src.getClassification())
	{
		case FEDERATION_MKI:
			curWeap = rand() % 3 + 4;
			break;
		case FEDERATION_MKII:
			curWeap = rand() % 3 + 4;
			break;
		case FEDERATION_MKIII:
			curWeap = rand() % 3 + 4;
			break;
		case FEDERATION_MKIV:
			curWeap = rand() % 3 + 4;
			break;
		case FEDERATION_GUNSHIP:
			curWeap = rand() % 2 + 7;
			break;
		case PIRATE_GNAT:
			curWeap = rand() % 3 + 14;
			break;
		case PIRATE_CRUISER:
			curWeap = rand() % 3 + 14;
			break;
		case PIRATE_WRANGLER:
			curWeap = rand() % 3 + 14;
			break;
		case PIRATE_MARAUDER:
			curWeap = rand() % 3 + 14;
			break;
		case PIRATE_GUNSHIP:
			curWeap = 17;
			break;
		case PLAYER_SHIP:
			curWeap = 0;
			break;
		case PLAYER_HUNTER:
			curWeap = rand() % 3 + 9;
			break;
		case PLAYER_DEFENDER:
			curWeap = rand() % 3 + 9;
			break;
		case PLAYER_DESTROYER:
			curWeap = rand() % 3 + 9;
			break;
		case PLAYER_MINER_DROID:
			curWeap = 12;
			break;
		case PLAYER_TURRET:
			curWeap = 13;
		default:
			break;
	}
}

void ship::memFree()
{
	for(int i = shipTex.size() - 1; i >= 0; i--)
	{
		SDL_DestroyTexture(shipTex.at(i));
	}
}

void ship::accelerate(double mult)
{
	float energyLoss = 0.6f, accelMult = 1.0f;
	
	if(priority == ENGINES) 
	{
		energyLoss = 1.2f;
		accelMult = 2.0f;
	}
	else if(priority == GUNS) 
	{
		energyLoss = 0.6f;
		accelMult = 0.8f;
	}
		
	if(energy <= energyLoss) return;
	vec2 vec = computeVector(getAng() + 90.0f) * accelMult;
	addVel(vec*mult);
	energy -= energyLoss;
	engineGlow = clamp(engineGlow + 10.0f * accelMult,0.0f,255.0f);
	
	accelerating = true;
}

void ship::dodge(float side)
{
	float energyLoss = 0.2, accelMult = 1;
	
	if(priority == ENGINES) 
	{
		energyLoss = 0.4;
		accelMult = 2;
	}
	else if(priority == GUNS) 
	{
		energyLoss = 0.08;
		accelMult = 0.8;
	}
	
	if(energy <= energyLoss) return;
	vec2 vec = computeVector(getAng());
	addVel(vec*side*accelMult);
	energy -= energyLoss * fabs(side);
}

void ship::draw(float dt)
{
	vec2 p = dt * getPos() + (1 - dt) * getPPos();
	
	int w = dstrect.w, h = dstrect.h;
	
	SDL_Rect drawRect;
	
	p *= ZOOM_LEVEL;
	p += HALFWIN;
	
	w *= ZOOM_LEVEL;
	h *= ZOOM_LEVEL;
	
	drawRect.x = p.x - (w/2);
	drawRect.y = p.y - (h/2);
	drawRect.w = w;
	drawRect.h = h;
	
	SDL_SetTextureAlphaMod(shipTex.at(1),engineGlow);
	SDL_SetTextureAlphaMod(shipTex.at(2),steeringGlow);
	SDL_SetTextureAlphaMod(shipTex.at(3),drawShot);
	SDL_SetTextureAlphaMod(shipTex.at(4),shieldGlow);
	SDL_SetTextureColorMod(shipTex.at(3),weapons[curWeap][4],weapons[curWeap][5],weapons[curWeap][6]);
	
	SDL_RenderCopyEx(renderer,shipTex.at(5),NULL,&drawRect,0,NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer,shipTex.at(2),NULL,&drawRect,getAng(),NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer,shipTex.at(1),NULL,&drawRect,getAng(),NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer,shipTex.at(0),NULL,&drawRect,getAng(),NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer,shipTex.at(3),NULL,&drawRect,getAng(),NULL,SDL_FLIP_NONE);
	SDL_RenderCopyEx(renderer,shipTex.at(4),NULL,&drawRect,getAng(),NULL,SDL_FLIP_NONE);

	if(DEV_MODE)
	{	
		SDL_SetRenderDrawColor(renderer,weapons[curWeap][4],weapons[curWeap][5],weapons[curWeap][6],255);
		SDL_RenderDrawLine(renderer,p.x,p.y,p.x-sin(toRad(-targetAngle))*5,p.y-cos(toRad(-targetAngle))*5);
		SDL_SetRenderDrawColor(renderer,weapons[curWeap][4],weapons[curWeap][5],weapons[curWeap][6],100);
		SDL_Rect tmp = *getRekt();
		tmp.x *= ZOOM_LEVEL;
		tmp.y *= ZOOM_LEVEL;
		tmp.w *= ZOOM_LEVEL;
		tmp.h *= ZOOM_LEVEL;
		tmp.x += HALFWIN.x;
		tmp.y += HALFWIN.y;
		SDL_RenderFillRect(renderer, &tmp);
	}
	
	drawShot = clamp(drawShot - 30.0f, 0.0f, 255.0f);
}

void ship::update(float dt)
{
	float angDiff = clampRoll(targetAngle - angle, -180.0f, 180.0f);
	
	if(angVel != 0.0f) setAng( clampRoll( angle + angVel, -180.0f, 180.0f ) );
	else if(angDiff < -1.0f)
	{
		setAng(clampRoll(angle+clamp(angDiff * inertia, -9999.0f, -1.0f) * dt * PIXEL_UNIT_CONVERSION, -180.0f, 180.0f));
		steeringGlow = clamp(steeringGlow + 20.0f, 0.0f, 255.0f);
	}
	else if(angDiff > 1.0f)
	{
		setAng(clampRoll(angle + clamp(angDiff * inertia, 1.0f, 9999.0f) * dt * PIXEL_UNIT_CONVERSION, -180.0f, 180.0f));
		steeringGlow = clamp(steeringGlow + 20.0f, 0.0f, 255.0f);
	}
	
	float energy_loss = 0.05f, shield_add = 0.05f;
	
	if(priority == SHIELDS)
	{
		energy_loss = 0.3f;
		shield_add = 0.4f;
		if(shield >= 0.0f) shieldGlow = 255.0f;
	}
	else if(priority == GUNS or priority == ENGINES) 
	{
		energy_loss = 0.01f;
		shield_add = 0.03f;
	}
	
	if(energy >= energy_loss and shield < maxShield)
	{
		shield = clamp(shield + shield_add * shieldMul, 0.0f, maxShield);
		energy -= energy_loss;
	}

	if(!accelerating) energy = clamp(energy + 0.1f * generatorMul, 0.0f, maxEnergy);
	
	if(rand()%999 == 0) health = clamp(health + 0.5f, 0.0f, maxHealth);
	
	steeringGlow = clamp(steeringGlow - 10.0f, 0.0f, 255.0f);
	shieldGlow = clamp(shieldGlow - 10.0f, 0.0f, 255.0f);
	engineGlow = clamp(engineGlow - 1.0f, 0.0f, 255.0f);
	
	coolDown = clamp(coolDown - dt, 0.0f, 999.0f);
	damageTimer = clamp(damageTimer - dt, 0.0f, 10.0f);
	
	dstrect.x = getPos().x - (dstrect.w/2);
	dstrect.y = getPos().y - (dstrect.h/2);
	
	accelerating = false;
}

void ship::damage(float d)
{
	if(getShield() - d > 0) shieldGlow = 255.0f;
	
	float shieldDmg = clamp(d, 0.0f, getShield());
	d -= shieldDmg;
	setShield(getShield()-shieldDmg);
	
	float healthDmg = clamp(d, 0.0f, getHealth());
	d -= healthDmg;
	setHealth(getHealth()-healthDmg);
	
	damageTimer = 10.0f;
}

int ship::upgrade(int i) 
{
  std::cout << "upgrade! " << i << std::endl;
	if(i < 4) upgrades[i]++;
	
	switch(i)
	{				
		case 0:
			/*
			0)lasers per shot 
			1)spread 
			2)dmg 
			3)speed 
			4)red 
			5)green 
			6)blue 
			7)energy cost 
			8)cooldown.
			*/
      if(m_weapons.size() < 1) break;
      m_weapons[0][1] *= 0.9f;
      m_weapons[0][2] *= 1.2f;
      m_weapons[0][3] *= 1.12f;
      m_weapons[0][7] *= 1.1f;
      m_weapons[0][8] *= 0.95f;
			
      if(m_weapons.size() < 2) break;
      m_weapons[1][0] *= 1.1f;
      m_weapons[1][1] *= 0.9f;
      m_weapons[1][2] *= 1.1f;
      m_weapons[1][3] *= 1.12f;
      m_weapons[1][7] *= 1.1f;
      m_weapons[1][8] *= 0.95f;
			
      if(m_weapons.size() < 3) break;
      m_weapons[2][0] *= 1.1f;
      m_weapons[2][1] *= 0.9f;
      m_weapons[2][3] *= 1.12f;
      m_weapons[1][7] *= 0.95f;
      m_weapons[2][8] *= 0.95f;
			break;
		case 1:
			shieldMul *= 1.2f;
			break;
		case 2:
			generatorMul *= 1.2f;
			break;
		case 3:
			inertia *= 1.2f;
			break;
		case 4:
			missiles++;
			break;
		case 5:
			break;
	}
	
	return upgrades[i];
}

void ship::setGrade(int i, int v)
{
	for(int k = 0; k < v; ++k) upgrade(i);
}

int ship::getScore()
{
	return static_cast<int>((maxHealth + maxShield + maxEnergy) / 100.0f);
}
