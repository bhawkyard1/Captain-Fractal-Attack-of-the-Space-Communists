#include "ship.hpp"
#include "weapons.hpp"
#include "vectors.hpp"

std::vector<ship> g_ship_templates;

ship::ship()
{
	m_coolDown = 0.0f;
	m_shooting = false;

	setPos(vec3());
	setPPos(vec3());

	setVel(vec3());
	setWVel(vec3());
	m_angle = 0.0f;
	m_targetAngle = 0.0f;

	m_engineGlow = 0.0f;
	m_engineTemp = 0.0f;
	m_shieldGlow = 0.0f;
	m_drawShot = 0.0f;
	m_angVel = 0.0f;

	m_canMove = true;
	m_canShoot = true;

	m_curWeap = 0;
	m_weaponRange = {0, 0};

	m_hasParent = false;
	m_parent = slotID<ship>();
	m_parentOffset = {0.0f, 0.0f, 0.0f};

	for(short unsigned int i = 0; i < UPGRADES_LEN; i++) m_upgrades[i] = 0;
	m_shieldMul = 1.0f;
	m_generatorMul = 1.0f;

	m_cargo.setDim(vec2());

	m_type = SHIP_TYPE_FIGHTER;

	m_identifier = "";
	setMaxHealth(0.0f,true);
	setMaxShield(0.0f,true);
	setMaxEnergy(0.0f,true);
	m_initInertia = 0.0f;
	m_enginePower = 0.0f;
	//m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );

	m_angVelRange = {0, 0};
	m_angVel = 0.0f;

	m_priority = PRIORITY_NONE;

	m_classification = SHIPS_END;

	m_radius = 0.0f;

	m_kills = 0;
	m_experience = 0.0f;

	m_docked = false;
	m_maxShipStorage = 0;
	m_storedShips = 0;
}

ship::ship(
		const ship &_src,
		const vec3 _p
		)
{
	m_coolDown = 0.0f;
	m_shooting = false;

	setPos(_p);
	setPPos(_p);

	setVel({0.0f, 0.0f, 0.0f});
	setWVel({0.0f, 0.0f, 0.0f});
	m_angle = 0.0f;
	m_targetAngle = 0.0f;

	m_engineGlow = 0.0f;
	m_engineTemp = 0.0f;
	m_shieldGlow = 0.0f;
	m_drawShot = 0.0f;

	m_canMove = _src.getCanMove();
	m_canShoot = _src.getCanShoot();

	setMaxHealth( _src.getMaxHealth(), true );
	setMaxShield( _src.getMaxShield(), true );
	setMaxEnergy( _src.getMaxEnergy(), true );

	m_initInertia = _src.m_initInertia;
	m_enginePower = _src.getEnginePower();
	m_angVelRange = _src.m_angVelRange;
	m_angVel = randNum(m_angVelRange.first, m_angVelRange.second);

	m_identifier = _src.getIdentifier();

	m_priority = PRIORITY_NONE;

	for(short unsigned int i = 0; i < UPGRADES_LEN; i++) m_upgrades[i] = 0;
	m_shieldMul = _src.m_shieldMul;
	m_generatorMul = _src.m_generatorMul;

	m_classification = _src.getClassification();

	m_radius = _src.getRadius();

	m_curWeap = 0;

	m_hasParent = _src.hasParent();
	m_parent = _src.getParent();
	m_parentOffset = _src.m_parentOffset;

	m_cargo = _src.m_cargo;

	m_type = _src.m_type;

	if(m_weaponRange.first != 0 or m_weaponRange.second != 0)
		m_weapons.push_back( g_weapons[ randNum(m_weaponRange.first, m_weaponRange.second) ] );
	else
		m_weapons = _src.m_weapons;

	m_attachmentPoints = _src.getAttachmentPoints();

	m_kills = 0;
	m_experience = 0.0f;

	m_docked = false;
	m_maxShipStorage = _src.m_maxShipStorage;
	m_storedShips = _src.m_storedShips;
}

void ship::accelerate(const float _mult)
{
	vec2 add = vec(getAng() + 90.0f);
	accelerate(tovec3(add), _mult);

	/*float energyLoss = 0.6f * _mult;
	float accelMult = 1.0f * _mult * m_inertia * m_enginePower;

	if(m_priority == PRIORITY_ENGINES)
	{
		energyLoss *= 1.2f;
		accelMult *= 2.0f;
	}
	else if(m_priority == PRIORITY_GUNS)
	{
		energyLoss *= 0.6f;
		accelMult *= 0.8f;
	}

	if(m_energy <= energyLoss) return;
	vec2 add = vec(getAng() + 90.0f) * accelMult;
	addForce(vec3(add.m_x, add.m_y, 0.0f));
	m_energy -= energyLoss;
	m_engineGlow = clamp(m_engineGlow + 5.0f * (accelMult + 0.75f), 0.0f, 255.0f);
	m_engineTemp += 1.0f;

	setAccelerating(true);*/
}

void ship::accelerate(
		const vec3 _dir,
		const float _mult
		)
{
	float energyLoss = 0.6f;
	float accelMult = 1.0f * _mult * m_inertia * m_enginePower * clamp(m_engineTemp / 100.0f, 0.0f, 1.0f);

	if(m_priority == PRIORITY_ENGINES)
	{
		energyLoss = 1.2f;
		accelMult = 2.0f;
	}
	else if(m_priority == PRIORITY_GUNS)
	{
		energyLoss = 0.6f;
		accelMult = 0.8f;
	}

	if(m_energy <= energyLoss) return;
	addForce(_dir * accelMult);
	m_energy -= energyLoss;
	m_engineGlow = clamp(m_engineGlow + 40.0f * accelMult, 0.0f, 255.0f);
	m_engineTemp += clamp( m_engineTemp + m_enginePower, 0.0f, 500.0f );

	setAccelerating(true);
}

void ship::decelerate()
{
	float energyLoss = 0.6f, accelMult = 1.0f;

	if(m_priority == PRIORITY_ENGINES)
	{
		energyLoss = 1.2f;
		accelMult = 2.0f;
	}
	else if(m_priority == PRIORITY_GUNS)
	{
		energyLoss = 0.6f;
		accelMult = 0.8f;
	}

	if(m_energy <= energyLoss) return;

	vec3 vel = getVel();
	float spd = mag(getVel());

	if(spd == 0.0f) return;

	vec3 add = -vel / spd;
	float clamped = clamp(accelMult * m_inertia * m_enginePower, 0.0f, spd);
	add *= clamped;

	addForce(add);
	m_energy -= energyLoss;
	m_engineGlow = clamp(m_engineGlow + 40.0f * accelMult, 0.0f, 255.0f);
	m_engineTemp += clamp( m_engineTemp + m_enginePower, 0.0f, 500.0f );

	setAccelerating(true);
}

void ship::damage(float _d)
{
	if(getShield() - _d > 0.0f) m_shieldGlow = 255.0f;

	float shieldDmg = clamp(_d, 0.0f, getShield());
	_d -= shieldDmg;
	setShield(getShield() - shieldDmg);

	float healthDmg = clamp(_d, 0.0f, getHealth());
	_d -= healthDmg;
	setHealth(getHealth()-healthDmg);

	m_damageTimer = 10.0f;
}

void ship::dodge(const float _side)
{
	float energyLoss = 0.2, accelMult = 1;

	if(m_priority == PRIORITY_ENGINES)
	{
		energyLoss = 0.4;
		accelMult = 2;
	}
	else if(m_priority == PRIORITY_GUNS)
	{
		energyLoss = 0.08;
		accelMult = 0.8;
	}

	if(m_energy <= energyLoss) return;
	vec2 avec = vec(getAng());
	addForce( vec3(avec.m_x, avec.m_y, 0.0f) * _side * accelMult * m_inertia * m_enginePower );
	m_energy -= energyLoss * fabs(_side);
}

void ship::setFiring(const bool _v)
{
	m_shooting = _v;
	if(_v)
	{
		m_damageTimer = 3.0f;
	}
}

float ship::damage(float _d, const vec3 _v)
{
	float ret = 0.0f;
	//Shots to the rear do more damage.
	if(m_canMove)
	{
		_d *= (dotProd( tovec3(vec(m_angle + 90.0f)), unit(_v) ) / 2.0f) + 1.5f;
		vec3 add = {_v.m_x, _v.m_y, 0.0f};
		addForce(add * m_inertia);
	}

	ret = _d;

	if(getShield() - _d > 0) m_shieldGlow = 255.0f;

	float shieldDmg = clamp(_d, 0.0f, getShield());
	_d -= shieldDmg;
	setShield(getShield() - shieldDmg);

	float healthDmg = clamp(_d, 0.0f, getHealth());
	_d -= healthDmg;
	setHealth(getHealth()-healthDmg);

	m_damageTimer = 10.0f;

	return ret;
}

void ship::update(const float _dt)
{
	float spd = mag(getVel()) / 100.0f;
	spd *= _dt;
	if(spd > 0.5f) damage( spd );
	m_inertia = 1.0f / (1.0f / m_initInertia + 1.0f / m_cargo.getInvMass());

	float angDiff = clampRoll(m_targetAngle - m_angle, -180.0f, 180.0f);
	float turnConst = 0.1f;

	if(m_angVel != 0.0f) setAng( clampRoll( m_angle + m_angVel * _dt, -180.0f, 180.0f ) );
	else if(angDiff < -0.5f)
	{
		setAng(clampRoll(m_angle + clamp(angDiff * m_inertia * m_enginePower * turnConst, -90.0f, -0.0f) * _dt * g_PIXEL_UNIT_CONVERSION, -180.0f, 180.0f));
	}
	else if(angDiff > 0.5f)
	{
		setAng(clampRoll(m_angle + clamp(angDiff * m_inertia * m_enginePower * turnConst, 0.0f, 90.0f) * _dt * g_PIXEL_UNIT_CONVERSION, -180.0f, 180.0f));
	}

	float energy_loss = 0.5f, shield_add = 0.5f;

	if(m_priority == PRIORITY_SHIELDS)
	{
		energy_loss = 3.0f;
		shield_add = 4.0f;
		if(m_shield >= 0.0f) m_shieldGlow = 255.0f;
	}
	else if(m_priority == PRIORITY_GUNS or m_priority == PRIORITY_ENGINES)
	{
		energy_loss = 0.1f;
		shield_add = 0.3f;
	}

	if(m_energy >= energy_loss and m_shield < m_maxShield)
	{
		m_shield = clamp(m_shield + shield_add * m_shieldMul * _dt, 0.0f, m_maxShield);
		m_energy -= energy_loss * _dt;
	}

	m_energy = clamp(m_energy + m_generatorMul * _dt * 5.0f, 0.0f, m_maxEnergy);

	//Repairs
	if(rand() % 999 == 0) m_health = clamp(m_health + 0.5f, 0.0f, m_maxHealth);
	if(m_docked and rand() % 64) m_health = clamp(m_health + 0.5f, 0.0f, m_maxHealth);

	m_shieldGlow = clamp(m_shieldGlow - 200.0f * _dt, 0.0f, 255.0f);
	if(!m_accelerating)
	{
		m_engineGlow = clamp(m_engineGlow - 500.0f * _dt, 0.0f, 255.0f);
		m_engineTemp = std::max( 0.0f, m_engineTemp - _dt );
	}

	m_coolDown = clamp(m_coolDown - _dt, 0.0f, 999.0f);
	m_damageTimer = clamp(m_damageTimer - _dt, 0.0f, 10.0f);

	m_drawShot *= clamp(20.0f * _dt, 0.0f, 0.9f);

	addForce(-getVel() * 0.0001f);

	m_accelerating = false;

	if(m_cargo.isVisible()) m_cargo.update(_dt);
}

void ship::setGrade(
		const int _i,
		const int _v
		)
{
	for(int k = 0; k < _v; ++k) upgrade(_i);
}

void ship::dock(slotID<ship> _parent)
{
	m_docked = true;
	m_dockParent = _parent;

	m_canMove = false;
	m_canShoot = false;

	/*m_dockedShips.push_back( _dockee );
	_dockee.get()->setDocked(true);
	_dockee.get()->setParent(  );*/
}

void ship::undock()
{
	m_docked = false;
	m_dockParent = slotID<ship>();

	m_canMove = true;
	m_canShoot = true;
}

int ship::upgrade(const int _i)
{
	if(_i < 4) m_upgrades[_i]++;
	else return 0;

	switch(_i)
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
		m_shieldMul *= 1.2f;
		break;
	case 2:
		m_generatorMul *= 1.2f;
		break;
	case 3:
		m_enginePower *= 1.1f;
		break;
	case 4:
		m_missiles++;
		break;
	case 5:
		break;
	default:
		break;
	}
	return m_upgrades[_i];
}

int ship::getScore() const
{
	return static_cast<int>((m_maxHealth + m_maxShield + m_maxEnergy) / 100.0f);
}

void ship::setCooldown(const float _f)
{
	m_coolDown = _f;
	addVelS(tovec3(-vec(m_angle + 90.0f) * getCurWeapStat(STOPPING_POWER)));
}

std::array<float, 4> ship::getCurWeapCol() const
{
	if(m_canShoot)
		return {{getCurWeapStat(COLOUR_RED) / 255.0f, getCurWeapStat(COLOUR_GREEN) / 255.0f, getCurWeapStat(COLOUR_BLUE) / 255.0f, 1.0f}};
	return {{0.0f, 0.0f, 0.0f, 0.0f}};
}

std::array<float, 4> ship::getShieldCol() const
{
	//This is weird. Maybe I should add a m_hasShield sometime?
	if(m_canShoot)
		return {{getCurWeapStat(COLOUR_RED) / 255.0f, getCurWeapStat(COLOUR_GREEN) / 255.0f, getCurWeapStat(COLOUR_BLUE) / 255.0f, static_cast<float>(getShieldGlow())}};
	return {{0.1f, 0.4f, 1.0f, 1.0f}};
}

float ship::getCurWeapStat(WEAPON_STAT _ws) const
{
	if(m_canShoot)
		return m_weapons[m_curWeap][_ws];
	return 0.0f;
}

void ship::setParent(slotID<ship> _p)
{
	m_parent = _p;
	if(_p.m_version != -1)
		m_hasParent = true;
	else
		m_hasParent = false;
}

void ship::addXP(const float _xp)
{
	float div = 1.0f + (m_experience / 32.0f);
	m_experience += _xp / div;
	//std::cout << "real experience " << (_xp / div) << '\n';
	//std::cout << "new experience " << m_experience << '\n';
}

float ship::calcAICost()
{
	return (m_maxHealth + m_maxShield) * 0.5f;
}

void ship::transferCargo(ship *_target, slot _item)
{
	debris item = *(m_cargo.getItems()->getByID(_item));
	m_cargo.removeItem(_item.m_id);
	_target->addItem(item);
}

void ship::transferAllCargo(ship *_target)
{
	for(size_t i = 0; i < m_cargo.getItems()->size(); ++i)
	{
		//If item added successfully, remove item from inventory.
		if(_target->addItem(m_cargo.getItems()->m_objects[i]))
			m_cargo.removeItem(i);
	}
}

float calcAICost(const ship_spec _spec)
{
	return g_ship_templates[_spec].calcAICost();
}

float calcAIPower(const ship_spec _spec)
{
	if(g_ship_templates[_spec].getCanMove()) return g_ship_templates[_spec].calcAICost();
	return 0.0f;
}

ship_spec operator+(const ship_spec &_lhs, const ship_spec &_rhs)
{
	return static_cast<ship_spec>( static_cast<int>(_lhs) + static_cast<int>(_rhs) );
}

ship_spec operator+(const ship_spec &_lhs, const int &_rhs)
{
	return static_cast<ship_spec>( static_cast<int>(_lhs) + _rhs );
}

ship_spec operator+(const int &_lhs, const ship_spec &_rhs)
{
	return static_cast<ship_spec>( _lhs + static_cast<int>(_rhs) );
}

ship_spec operator+(const ship_spec &_lhs, const size_t &_rhs)
{
	return static_cast<ship_spec>( static_cast<size_t>(_lhs) + _rhs );
}

ship_spec operator+(const size_t &_lhs, const ship_spec &_rhs)
{
	return static_cast<ship_spec>( _lhs + static_cast<size_t>(_rhs) );
}

/*
bool operator<(const ship_spec &_lhs, const ship_spec &_rhs)
{
		return static_cast<int>(_lhs) < static_cast<int>(_rhs);
}*/

ship_spec& operator++(ship_spec &_lhs)
{
	int temp = _lhs;
	return _lhs = static_cast<ship_spec>( ++temp );
}
