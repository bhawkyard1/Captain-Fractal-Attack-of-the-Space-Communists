#include "ship.hpp"
#include "weapons.hpp"
#include "vectors.hpp"

std::vector<tinfo> g_texture_keys = {
    {"COMMUNIST_1", 32}, {"COMMUNIST_2", 32}, {"COMMUNIST_CAPITAL", 1024},
    {"COMMUNIST_TURRET", 16},
    {"FEDERATION_MKI", 32}, {"FEDERATION_MKII", 32}, {"FEDERATION_MKIII", 32}, {"FEDERATION_MKIV", 32}, {"FEDERATION_GUNSHIP", 64}, {"FEDERATION_CAPITAL", 1024},
    {"FEDERATION_TURRET", 16},
    {"PIRATE_GNAT", 32}, {"PIRATE_CRUISER", 32}, {"PIRATE_WRANGLER", 40}, {"PIRATE_MARAUDER", 40}, {"PIRATE_GUNSHIP", 64}, {"PIRATE_CAPITAL", 1024},
    {"PIRATE_TURRET", 16},
    {"ALLIANCE_SCOUT", 32}, {"ALLIANCE_TRACKER", 32}, {"ALLIANCE_PHOENIX", 35}, {"ALLIANCE_DRAGON", 45}, {"ALLIANCE_GUNSHIP", 64},
    {"ALLIANCE_TRADER", 64},
    {"ALLIANCE_TURRET", 16}, {"ALLIANCE_GRAVWELL", 128}, {"ALLIANCE_BARRACKS", 512}, {"ALLIANCE_STATION", 2048},
    {"PLAYER_HUNTER", 32}, {"PLAYER_DEFENDER", 32}, {"PLAYER_DESTROYER", 32}, {"PLAYER_GUNSHIP", 64}, {"PLAYER_CAPITAL", 1024},
    {"PLAYER_MINER_DROID", 16},
    {"PLAYER_TURRET", 16}, {"PLAYER_GRAVWELL", 256}, {"PLAYER_BARRACKS", 512}, {"PLAYER_STATION", 1024},
    {"PLAYER_SHIP", 32},
    {"ION_MISSILE_MKI", 16},
    {"ASTEROID_SMALL", 32}, {"ASTEROID_MID", 64}, {"ASTEROID_LARGE", 128},
    {"SHIPS_END", 1}
};

std::vector<ship> g_ship_templates;

std::string getTextureKey(ship_spec _s) {return g_texture_keys[_s].m_name;}

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
    m_shieldGlow = 0.0f;
    m_drawShot = 0.0f;
    m_angVel = 0.0f;

    m_canMove = true;
    m_canShoot = true;

    m_curWeap = 0;
    m_weaponRange = {0, 0};

    m_hasParent = false;
    m_parent = {0, -1};
    m_parentOffset = {0.0f, 0.0f, 0.0f};

    for(short unsigned int i = 0; i < UPGRADES_LEN; i++) m_upgrades[i] = 0;
    m_shieldMul = 1.0f;
    m_generatorMul = 1.0f;

    m_lastAttacker = {0, -1};

    m_cargo.setDim(vec2());

    m_type = SHIP_TYPE_FIGHTER;

    m_identifier = "";
    setMaxHealth(0.0f,true);
    setMaxShield(0.0f,true);
    setMaxEnergy(0.0f,true);
    m_initInertia = 0.0f;
    m_enginePower = 0.0f;
    //m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );


    /*switch(_type)
    {
    case COMMUNIST_1:
        m_identifier = "COMMUNIST_1";
        setMaxHealth(30.0f,true);
        setMaxShield(20.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case COMMUNIST_2:
        m_identifier = "COMMUNIST_2";
        setMaxHealth(50.0f,true);
        setMaxShield(40.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.09f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case COMMUNIST_CAPITAL:
        m_identifier = "COMMUNIST_CAPITAL";
        setMaxHealth(4000.0f,true);
        setMaxShield(2500.0f,true);
        setMaxEnergy(1000.0f,true);
        m_initInertia = 0.00025f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[WEAPON_COMMUNIST_CAPITAL] );
        m_curWeap = 0;
        m_cargo.setDim({1024.0f, 1024.0f});
        m_type = SHIP_TYPE_CAPITAL;
        break;
    case COMMUNIST_TURRET:
        m_identifier = "COMMUNIST_TURRET";
        setMaxHealth(100.0f,true);
        setMaxShield(200.0f,true);
        setMaxEnergy(300.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );
        m_curWeap = 0;
        m_canMove = false;
        m_type = SHIP_TYPE_TURRET;
        m_cargo.setDim({64.0f, 64.0f});
        break;
    case FEDERATION_MKI:
        m_identifier = "FEDERATION_MKI";
        setMaxHealth(30.0f,true);
        setMaxShield(20.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case FEDERATION_MKII:
        m_identifier = "FEDERATION_MKII";
        setMaxHealth(50.0f,true);
        setMaxShield(50.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.09f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case FEDERATION_MKIII:
        m_identifier = "FEDERATION_MKIII";
        setMaxHealth(90.0f,true);
        setMaxShield(60.0f,true);
        setMaxEnergy(150.0f,true);
        m_initInertia = 0.08f;
        m_enginePower = 6.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        m_curWeap = 0;
        m_cargo.setDim({64.0f, 64.0f});
        break;
    case FEDERATION_MKIV:
        m_identifier = "FEDERATION_MKIV";
        setMaxHealth(120.0f,true);
        setMaxShield(80.0f,true);
        setMaxEnergy(200.0f,true);
        m_initInertia = 0.15f;
        m_enginePower = 7.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case FEDERATION_GUNSHIP:
        m_identifier = "FEDERATION_GUNSHIP";
        setMaxHealth(500.0f,true);
        setMaxShield(250.0f,true);
        setMaxEnergy(500.0f,true);
        m_initInertia = 0.02f;
        m_enginePower = 8.0f;
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_FED_BOSS_1] );
        m_curWeap = 0;
        m_cargo.setDim({256.0f, 256.0f});
        break;
    case FEDERATION_CAPITAL:
        m_identifier = "FEDERATION_CAPITAL";
        setMaxHealth(5000.0f,true);
        setMaxShield(1200.0f,true);
        setMaxEnergy(1000.0f,true);
        m_initInertia = 0.00025f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_FED_BOSS_1] );
        m_curWeap = 0;
        m_cargo.setDim({1200.0f, 1200.0f});
        m_type = SHIP_TYPE_CAPITAL;
        break;
    case FEDERATION_TURRET:
        m_identifier = "FEDERATION_TURRET";
        setMaxHealth(100.0f,true);
        setMaxShield(200.0f,true);
        setMaxEnergy(300.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[WEAPON_FED_3] );
        m_curWeap = 0;
        m_canMove = false;
        m_cargo.setDim({64.0f, 64.0f});
        m_type = SHIP_TYPE_TURRET;
        break;
    case PIRATE_GNAT:
        m_identifier = "PIRATE_GNAT";
        setMaxHealth(30.0f,true);
        setMaxShield(20.0f,true);
        setMaxEnergy(90.0f,true);
        m_initInertia = 0.15f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        m_curWeap = 0;
        m_cargo.setDim({64.0f, 64.0f});
        break;
    case PIRATE_CRUISER:
        m_identifier = "PIRATE_CRUISER";
        setMaxHealth(50.0f,true);
        setMaxShield(50.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        m_curWeap = 0;
        m_cargo.setDim({64.0f, 64.0f});
        break;
    case PIRATE_WRANGLER:
        m_identifier = "PIRATE_WRANGLER";
        setMaxHealth(90.0f,true);
        setMaxShield(60.0f, true);
        setMaxEnergy(120.0f,true);
        m_initInertia = 0.05f;
        m_enginePower = 6.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        m_curWeap = 0;
        m_cargo.setDim({80.0f, 80.0f});
        break;
    case PIRATE_MARAUDER:
        m_identifier = "PIRATE_MARAUDER";
        setMaxHealth(120.0f,true);
        setMaxShield(60.0f,true);
        setMaxEnergy(120.0f,true);
        m_initInertia = 0.15f;
        m_enginePower = 7.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        m_curWeap = 0;
        m_cargo.setDim({100.0f, 100.0f});
        break;
    case PIRATE_GUNSHIP:
        m_identifier = "PIRATE_GUNSHIP";
        setMaxHealth(400.0f,true);
        setMaxShield(300.0f,true);
        setMaxEnergy(150.0f,true);
        m_initInertia = 0.05f;
        m_enginePower = 8.0f;
        m_weapons.push_back( g_weapons[WEAPON_PIRATE_BOSS_1] );
        m_curWeap = 0;
        m_cargo.setDim({350.0f, 350.0f});
        break;
    case PIRATE_CAPITAL:
        m_identifier = "PIRATE_CAPITAL";
        setMaxHealth(5500.0f,true);
        setMaxShield(1200.0f,true);
        setMaxEnergy(1000.0f,true);
        m_initInertia = 0.00025f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[WEAPON_PIRATE_BOSS_1] );
        m_curWeap = 0;
        m_cargo.setDim({1500.0f, 1500.0f});
        m_type = SHIP_TYPE_CAPITAL;
        break;
    case PIRATE_TURRET:
        m_identifier = "PIRATE_TURRET";
        setMaxHealth(100.0f,true);
        setMaxShield(200.0f,true);
        setMaxEnergy(300.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[WEAPON_PIRATE_1] );
        m_curWeap = 0;
        m_canMove = false;
        m_cargo.setDim({64.0f, 64.0f});
        m_type = SHIP_TYPE_TURRET;
        break;
    case ALLIANCE_SCOUT:
        m_identifier = "ALLIANCE_SCOUT";
        setMaxHealth(30.0f,true);
        setMaxShield(50.0f,true);
        setMaxEnergy(120.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 3.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        m_curWeap = 0;
        m_cargo.setDim({16.0f, 16.0f});
        break;
    case ALLIANCE_TRACKER:
        m_identifier = "ALLIANCE_TRACKER";
        setMaxHealth(50.0f,true);
        setMaxShield(80.0f,true);
        setMaxEnergy(125.0f,true);
        m_initInertia = 0.09f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        m_curWeap = 0;
        m_cargo.setDim({16.0f, 16.0f});
        break;
    case ALLIANCE_PHOENIX:
        m_identifier = "ALLIANCE_PHOENIX";
        setMaxHealth(90.0f,true);
        setMaxShield(110.0f,true);
        setMaxEnergy(150.0f,true);
        m_initInertia = 0.07f;
        m_enginePower = 7.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case ALLIANCE_DRAGON:
        m_identifier = "ALLIANCE_DRAGON";
        setMaxHealth(120.0f,true);
        setMaxShield(130.0f,true);
        setMaxEnergy(200.0f,true);
        m_initInertia = 0.05f;
        m_enginePower = 8.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case ALLIANCE_GUNSHIP:
        m_identifier = "ALLIANCE_GUNSHIP";
        setMaxHealth(400.0f,true);
        setMaxShield(250.0f,true);
        setMaxEnergy(250.0f,true);
        m_initInertia = 0.02f;
        m_enginePower = 10.0f;
        m_weapons.push_back( g_weapons[WEAPON_ALLIANCE_BOSS] );
        m_curWeap = 0;
        m_cargo.setDim({256.0f, 256.0f});
        break;
    case ALLIANCE_TRADER:
        m_identifier = "ALLIANCE_TRADER";
        setMaxHealth(200.0f,true);
        setMaxShield(280.0f,true);
        setMaxEnergy(280.0f,true);
        m_initInertia = 0.02f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        m_curWeap = 0;
        m_cargo.setDim({1024.0f, 1024.0f});
        m_type = SHIP_TYPE_TRADER;
        break;
    case ALLIANCE_TURRET:
        m_identifier = "ALLIANCE_TURRET";
        setMaxHealth(100.0f,true);
        setMaxShield(200.0f,true);
        setMaxEnergy(300.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        m_curWeap = 0;
        m_canMove = false;
        m_cargo.setDim({64.0f, 64.0f});
        m_type = SHIP_TYPE_TURRET;
        break;
    case PLAYER_SHIP:
        m_identifier = "PLAYER_SHIP";
        setMaxHealth(100.0f,true);
        setMaxShield(100.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.07f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[WEAPON_PLAYER_RED] );
        m_weapons.push_back( g_weapons[WEAPON_PLAYER_GREEN] );
        m_weapons.push_back( g_weapons[WEAPON_PLAYER_BLUE] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case PLAYER_HUNTER:
        m_identifier = "PLAYER_HUNTER";
        setMaxHealth(100.0f,true);
        setMaxShield(100.0f,true);
        setMaxEnergy(100.0f,true);
        m_initInertia = 0.2f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case PLAYER_DEFENDER:
        m_identifier = "PLAYER_DEFENDER";
        setMaxHealth(125.0f,true);
        setMaxShield(125.0f,true);
        setMaxEnergy(125.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 8.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        m_curWeap = 0;
        m_cargo.setDim({32.0f, 32.0f});
        break;
    case PLAYER_DESTROYER:
        m_identifier = "PLAYER_DESTROYER";
        setMaxHealth(150.0f,true);
        setMaxShield(150.0f,true);
        setMaxEnergy(150.0f,true);
        m_initInertia = 0.07f;
        m_enginePower = 10.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        m_curWeap = 0;
        m_cargo.setDim({48.0f, 48.0f});
        break;
    case PLAYER_GUNSHIP:
        m_identifier = "PLAYER_GUNSHIP";
        setMaxHealth(350.0f,true);
        setMaxShield(400.0f,true);
        setMaxEnergy(500.0f,true);
        m_initInertia = 0.04f;
        m_enginePower = 9.0f;
        m_weapons.push_back( g_weapons[WEAPON_WINGMAN_GUNSHIP] );
        m_curWeap = 0;
        m_cargo.setDim({256.0f, 256.0f});
        break;
    case PLAYER_MINER_DROID:
        m_identifier = "PLAYER_MINER_DROID";
        setMaxHealth(10.0f,true);
        setMaxShield(20.0f,true);
        setMaxEnergy(200.0f,true);
        m_initInertia = 0.2f;
        m_enginePower = 4.0f;
        m_generatorMul = 5.0f;
        m_weapons.push_back( g_weapons[WEAPON_MINER_LASER] );
        m_curWeap = 0;
        m_cargo.setDim({64.0f, 64.0f});
        m_type = SHIP_TYPE_MINER;
        break;
    case ION_MISSILE_MKI:
        m_identifier = "ION_MISSILE_MKI";
        m_initInertia = 0.1f;
        m_enginePower = 6.0f;
        m_canShoot = false;
        break;
    case ASTEROID_SMALL:
        m_identifier = "ASTEROID_SMALL";
        setMaxHealth(200.0f,true);
        setMaxShield(0.0f,true);
        setMaxEnergy(0.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 0.0f;
        m_canMove = true;
        m_canShoot = false;
        m_type = SHIP_TYPE_NONE;
        break;
    case ASTEROID_MID:
        m_identifier = "ASTEROID_MID";
        setMaxHealth(1000.0f,true);
        setMaxShield(0.0f,true);
        setMaxEnergy(0.0f,true);
        m_initInertia = 0.025f;
        m_enginePower = 0.0f;
        m_canMove = true;
        m_canShoot = false;
        m_type = SHIP_TYPE_NONE;
        break;
    case ASTEROID_LARGE:
        m_identifier = "ASTEROID_LARGE";
        setMaxHealth(2000.0f,true);
        setMaxShield(0.0f,true);
        setMaxEnergy(0.0f,true);
        m_initInertia = 0.0025f;
        m_enginePower = 0.0f;
        m_canMove = true;
        m_canShoot = false;
        m_type = SHIP_TYPE_NONE;
        break;
    case PLAYER_CAPITAL:
        m_identifier = "PLAYER_CAPITAL";
        setMaxHealth(4000.0f,true);
        setMaxShield(4000.0f,true);
        setMaxEnergy(1000.0f,true);
        m_initInertia = 0.00025f;
        m_enginePower = 5.0f;
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        m_curWeap = 0;
        m_cargo.setDim({1200.0f, 1200.0f});
        m_type = SHIP_TYPE_CAPITAL;
        break;
    case PLAYER_TURRET:
        m_identifier = "PLAYER_TURRET";
        setMaxHealth(100.0f,true);
        setMaxShield(200.0f,true);
        setMaxEnergy(300.0f,true);
        m_initInertia = 0.1f;
        m_enginePower = 4.0f;
        m_weapons.push_back( g_weapons[WEAPON_TURRET_LASER] );
        m_curWeap = 0;
        m_canMove = false;
        m_cargo.setDim({64.0f, 64.0f});
        m_type = SHIP_TYPE_TURRET;
        break;
    case PLAYER_STATION:
        m_identifier = "PLAYER_STATION";
        setMaxHealth(10000.0f,true);
        setMaxShield(10000.0f,true);
        setMaxEnergy(100000.0f,true);
        m_initInertia = 0.0f;
        m_enginePower = 0.0f;
        m_canMove = false;
        m_canShoot = false;
        m_cargo.setDim({16000.0f, 16000.0f});
        m_type = SHIP_TYPE_STRUCTURE;
        break;
    case PLAYER_GRAVWELL:
        m_identifier = "PLAYER_GRAVWELL";
        setMaxHealth(700.0f,true);
        setMaxShield(2000.0f,true);
        setMaxEnergy(50000.0f,true);
        m_initInertia = 0.0f;
        m_enginePower = 0.0f;
        m_canMove = false;
        m_canShoot = false;
        m_type = SHIP_TYPE_STRUCTURE;
        m_cargo.setDim({512.0f, 512.0f});
        break;
    case PLAYER_BARRACKS:
        m_identifier = "PLAYER_BARRACKS";
        setMaxHealth(2000.0f,true);
        setMaxShield(4000.0f,true);
        setMaxEnergy(80000.0f,true);
        m_initInertia = 0.0f;
        m_enginePower = 0.0f;
        m_canMove = false;
        m_canShoot = false;
        m_type = SHIP_TYPE_STRUCTURE;
        m_cargo.setDim({1024.0f, 1024.0f});
        break;
    case ALLIANCE_STATION:
        m_identifier = "ALLIANCE_STATION";
        setMaxHealth(12000.0f,true);
        setMaxShield(9000.0f,true);
        setMaxEnergy(100000.0f,true);
        m_initInertia = 0.0f;
        m_enginePower = 0.0f;
        m_canMove = false;
        m_canShoot = false;
        m_cargo.setDim({16000.0f, 16000.0f});
        m_type = SHIP_TYPE_STRUCTURE;
        break;
    case ALLIANCE_BARRACKS:
        m_identifier = "ALLIANCE_BARRACKS";
        setMaxHealth(3000.0f,true);
        setMaxShield(4000.0f,true);
        setMaxEnergy(80000.0f,true);
        m_initInertia = 0.0f;
        m_enginePower = 0.0f;
        m_canMove = false;
        m_canShoot = false;
        m_type = SHIP_TYPE_STRUCTURE;
        m_cargo.setDim({1024.0f, 1024.0f});
        break;
    case ALLIANCE_GRAVWELL:
        m_identifier = "ALLIANCE_GRAVWELL";
        setMaxHealth(700.0f,true);
        setMaxShield(2000.0f,true);
        setMaxEnergy(50000.0f,true);
        m_initInertia = 0.0f;
        m_enginePower = 0.0f;
        m_canMove = false;
        m_canShoot = false;
        m_type = SHIP_TYPE_STRUCTURE;
        m_cargo.setDim({512.0f, 512.0f});
        break;
    case SHIPS_END:
        m_identifier = "ERROR";
        m_type = SHIP_TYPE_NONE;
        break;
    }*/

    m_angVelRange = {0, 0};
    m_angVel = 0.0f;
    /*if(m_type == SHIP_TYPE_NONE)
    {
        m_angVel = randNum(-1.0f, 1.0f);
    }
    else if(m_type == SHIP_TYPE_STRUCTURE)
    {
        m_angVel = randNum(-10.0f, 10.0f);
    }*/

    m_priority = PRIORITY_NONE;

    m_classification = SHIPS_END;

    m_radius = 0.0f;

    m_kills = 0;
    m_experience = 0.0f;
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

    m_lastAttacker = _src.m_lastAttacker;

    m_cargo = _src.m_cargo;

    m_type = _src.m_type;

    if(m_weaponRange.first != 0 or m_weaponRange.second != 0)
        m_weapons.push_back( g_weapons[ randNum(m_weaponRange.first, m_weaponRange.second) ] );
    else
        m_weapons = _src.m_weapons;

    /*switch(_src.getClassification())
    {
    case COMMUNIST_1:
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );
        break;
    case COMMUNIST_2:
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );
        break;
    case COMMUNIST_CAPITAL:
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_COMMUNIST_1] );
        break;
    case FEDERATION_MKI:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        break;
    case FEDERATION_MKII:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        break;
    case FEDERATION_MKIII:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        break;
    case FEDERATION_MKIV:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_FED_1] );
        break;
    case FEDERATION_GUNSHIP:
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_FED_BOSS_1] );
        break;
    case FEDERATION_CAPITAL:
        m_weapons.push_back( g_weapons[rand() % 2 + WEAPON_FED_BOSS_1] );
        break;
    case PIRATE_GNAT:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        break;
    case PIRATE_CRUISER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        break;
    case PIRATE_WRANGLER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        break;
    case PIRATE_MARAUDER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_PIRATE_1] );
        break;
    case PIRATE_GUNSHIP:
        m_weapons.push_back( g_weapons[WEAPON_PIRATE_BOSS_1] );
        break;
    case ALLIANCE_SCOUT:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        break;
    case ALLIANCE_TRACKER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        break;
    case ALLIANCE_PHOENIX:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        break;
    case ALLIANCE_DRAGON:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        break;
    case ALLIANCE_GUNSHIP:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        break;
    case ALLIANCE_TRADER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_ALLIANCE_1] );
        break;
    case PLAYER_SHIP:
        m_weapons.push_back( g_weapons[WEAPON_PLAYER_RED] );
        m_weapons.push_back( g_weapons[WEAPON_PLAYER_GREEN] );
        m_weapons.push_back( g_weapons[WEAPON_PLAYER_BLUE] );
        break;
    case PLAYER_HUNTER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        break;
    case PLAYER_DEFENDER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        break;
    case PLAYER_DESTROYER:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        break;
    case PLAYER_MINER_DROID:
        m_weapons.push_back( g_weapons[WEAPON_MINER_LASER] );
        break;
    case PLAYER_CAPITAL:
        m_weapons.push_back( g_weapons[rand() % 3 + WEAPON_WINGMAN_1] );
        break;
    case PLAYER_TURRET:
        m_weapons.push_back( g_weapons[WEAPON_TURRET_LASER] );
        break;
    default:
        for(size_t i = 0; i < _src.getWeaps().size(); ++i)
        {
            m_weapons.push_back(_src.getWeaps()[i]);
        }
        break;
    }*/

    m_kills = 0;
    m_experience = 0.0f;
}

void ship::accelerate(const float _mult)
{
    float energyLoss = 0.6f * _mult;
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

    setAccelerating(true);
}

void ship::accelerate(
        const vec3 _dir,
        const float _mult
        )
{
    float energyLoss = 0.6f;
    float accelMult = 1.0f * _mult * m_inertia * m_enginePower;

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
    m_engineGlow = clamp(m_engineGlow + 5.0f * accelMult, 0.0f, 255.0f);

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

    setAccelerating(true);
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

    if(rand() % 999 == 0) m_health = clamp(m_health + 0.5f, 0.0f, m_maxHealth);

    m_shieldGlow = clamp(m_shieldGlow - 200.0f * _dt, 0.0f, 255.0f);
    if(!m_accelerating) m_engineGlow = clamp(m_engineGlow - 500.0f * _dt, 0.0f, 255.0f);

    m_coolDown = clamp(m_coolDown - _dt, 0.0f, 999.0f);
    m_damageTimer = clamp(m_damageTimer - _dt, 0.0f, 10.0f);

    m_drawShot *= clamp(20.0f * _dt, 0.0f, 0.9f);

    addForce(-getVel() * 0.0001f);

    m_accelerating = false;

    if(m_cargo.isVisible()) m_cargo.update(_dt);
}

void ship::setFiring(const bool _v)
{
    m_shooting = _v;
    if(_v)
    {
        m_damageTimer = 3.0f;
    }
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

void ship::damage(float _d, const vec3 _v)
{
    //Shots to the rear do more damage.
    if(m_canMove)
    {
        _d *= (dotProd( tovec3(vec(m_angle + 90)), unit(_v) ) / 2.0f) + 1.5f;
        vec3 add = {_v.m_x, _v.m_y, 0.0f};
        addForce(add * m_inertia);
    }

    if(getShield() - _d > 0) m_shieldGlow = 255.0f;

    float shieldDmg = clamp(_d, 0.0f, getShield());
    _d -= shieldDmg;
    setShield(getShield() - shieldDmg);

    float healthDmg = clamp(_d, 0.0f, getHealth());
    _d -= healthDmg;
    setHealth(getHealth()-healthDmg);

    m_damageTimer = 10.0f;
}

void ship::damage(float _d, const vec3 _v, uniqueID _id)
{
    //Shots to the rear do more damage.
    if(m_canMove)
    {
        _d *= (dotProd( tovec3(vec(m_angle + 90)), unit(_v) ) / 2.0f) + 1.5f;
        vec3 add = {_v.m_x, _v.m_y, 0.0f};
        addForce(add * m_inertia);
    }

    if(getShield() - _d > 0) m_shieldGlow = 255.0f;

    float shieldDmg = clamp(_d, 0.0f, getShield());
    _d -= shieldDmg;
    setShield(getShield() - shieldDmg);

    float healthDmg = clamp(_d, 0.0f, getHealth());
    _d -= healthDmg;
    setHealth(getHealth()-healthDmg);

    m_damageTimer = 10.0f;
    m_lastAttacker = _id;
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

void ship::setGrade(
        const int _i,
        const int _v
        )
{
    for(int k = 0; k < _v; ++k) upgrade(_i);
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
    if(m_canShoot) return {getCurWeapStat(COLOUR_RED) / 255.0f, getCurWeapStat(COLOUR_GREEN) / 255.0f, getCurWeapStat(COLOUR_BLUE) / 255.0f, 1.0f};
    return {0.0f, 0.0f, 0.0f, 0.0f};
}

std::array<float, 4> ship::getShieldCol() const
{
    if(m_canShoot) return {getCurWeapStat(COLOUR_RED) / 255.0f, getCurWeapStat(COLOUR_GREEN) / 255.0f, getCurWeapStat(COLOUR_BLUE) / 255.0f, getShieldGlow()};
    return {0.1f, 0.4f, 1.0f, 1.0f};
}

float ship::getCurWeapStat(WEAPON_STAT _ws) const
{
    if(m_canShoot) return m_weapons[m_curWeap][_ws];
    return 0.0f;
}

void ship::setParent(uniqueID _p)
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
}

float ship::calcAICost()
{
    return (m_maxHealth + m_maxShield) * 0.5f;
}

void ship::transferCargo(ship *_target, uniqueID _item)
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
