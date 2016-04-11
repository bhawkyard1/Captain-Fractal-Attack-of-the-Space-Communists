#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>
#include "renderer.hpp"
#include "util.hpp"

#include "faction.hpp"
#include "enemy.hpp"
#include "laser.hpp"
#include "missile.hpp"

renderer::renderer(
    int _w,
    int _h
    )
{
  init();

  m_w = _w;
  m_h = _h;

  m_window = SDL_CreateWindow("Elite: Dangerous v2.0",
                              g_WIN_POS_X, g_WIN_POS_Y,
                              g_WIN_HEIGHT, g_WIN_WIDTH,
                              SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED );

  m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);

  SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

  loadTextures();

  m_cameraShake = 0.0f;
  m_cameraShakeTargetOffset = {0.0f, 0.0f};
  m_cameraShakeOffset = {0.0f, 0.0f};
}

renderer::~renderer()
{
  //Destroy sprites.
  for(auto &i : m_textures)
  {
    for(auto &j : i.second)
    {
      SDL_DestroyTexture( j );
    }
    i.second.clear();
  }
  m_textures.clear();

  for(auto &i : m_letters)
  {
    i.second.destroy();
    i.second.m_sheet.clear();
  }
  m_letters.clear();

  //Destroy the renderer.
  SDL_DestroyRenderer( m_renderer );
  //Destroy the window.
  SDL_DestroyWindow( m_window );
  //Quit SDL.
  SDL_Quit();
}

int renderer::init()
{
  if(SDL_Init(SDL_INIT_VIDEO) != 0)
  {
    std::cerr << "SDL_Init() failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(EXIT_FAILURE);
  }

  if(TTF_Init() != 0)
  {
    std::cerr << "TTF_Init() failed: " << TTF_GetError() << std::endl;
    SDL_Quit();
    exit(EXIT_FAILURE);
  }
  return 1;
}

void renderer::loadTextures()
{
  loadTextureSet("FEDERATION_MKI",      "enemy_1");
  loadTextureSet("FEDERATION_MKII",     "enemy_2");
  loadTextureSet("FEDERATION_MKIII",    "enemy_3");
  loadTextureSet("FEDERATION_MKIV",     "enemy_4");
  loadTextureSet("FEDERATION_GUNSHIP",  "enemy_5");

  loadTextureSet("PIRATE_GNAT",         "pirate_1");
  loadTextureSet("PIRATE_CRUISER",      "pirate_2");
  loadTextureSet("PIRATE_WRANGLER",     "pirate_3");
  loadTextureSet("PIRATE_MARAUDER",     "pirate_4");
  loadTextureSet("PIRATE_GUNSHIP",      "pirate_5");

  loadTextureSet("PLAYER_SHIP",         "player");
  loadTextureSet("PLAYER_HUNTER",       "wingman_1");
  loadTextureSet("PLAYER_DEFENDER",     "wingman_2");
  loadTextureSet("PLAYER_DESTROYER",    "wingman_3");
  loadTextureSet("PLAYER_MINER_DROID",  "miner_1");
  loadTextureSet("PLAYER_TURRET",       "turret_1");
  loadTextureSet("PLAYER_STATION",      "station_1");
  loadTextureSet("PLAYER_GRAVWELL",     "well_1");
  loadTextureSet("PLAYER_BARRACKS",     "barracks_1");

  loadTextureSet("ION_MISSILE_MKI",     "missile");

  loadTextureSet("ASTEROID_SMALL",      "asteroid_1");
  loadTextureSet("ASTEROID_MID",        "asteroid_2");
  loadTextureSet("ASTEROID_LARGE",      "asteroid_3");

  loadTexture("STARDUST_1", "stardust_1", SDL_BLENDMODE_ADD);
  loadTexture("STARDUST_2", "stardust_2", SDL_BLENDMODE_ADD);
  loadTexture("STARDUST_3", "stardust_3", SDL_BLENDMODE_ADD);
  loadTexture("STARDUST_4", "stardust_4", SDL_BLENDMODE_ADD);

  loadTexture("EXPLOSION", "explosion_1", SDL_BLENDMODE_BLEND);
  loadTexture("SMOKE", "smoke_1", SDL_BLENDMODE_BLEND);
  loadTexture("SKY", "sky", SDL_BLENDMODE_NONE);

  loadFontSpriteSheet("pix", g_RESOURCE_LOC + "fonts/pix.TTF", 20);
  loadFontSpriteSheet("minimal", g_RESOURCE_LOC + "fonts/minimal.otf", 25);
}

void renderer::loadTexture(std::string _key, std::string _path, SDL_BlendMode _b)
{
  std::vector<SDL_Texture*> temp;

  SDL_Surface * s = IMG_Load( (g_RESOURCE_LOC + "textures/" + _path + "/" + _path + ".png").c_str() );
  if(!s) std::cerr << "Texture load error! " << SDL_GetError() << std::endl;

  SDL_Texture * t = SDL_CreateTextureFromSurface(m_renderer, IMG_Load( (g_RESOURCE_LOC + "textures/" + _path + "/" + _path + ".png").c_str() ) );
  SDL_SetTextureBlendMode(t, _b);
  if(!t) std::cerr << "Texture load error! " << SDL_GetError() << std::endl;

  temp.push_back( t );
  m_textures.insert({_key, temp});

  SDL_FreeSurface(s);
}

void renderer::loadTextureSet(
    std::string _key,
    std::string _set
    )
{
  std::vector<SDL_Surface*> temp_surf;

  temp_surf.push_back( getSurface( g_RESOURCE_LOC + "textures/" + _set + "/" + _set + ".png") );
  temp_surf.push_back( getSurface( g_RESOURCE_LOC + "textures/" + _set + "/" + _set + "_engines.png") );
  temp_surf.push_back( getSurface( g_RESOURCE_LOC + "textures/" + _set + "/" + _set + "_steering.png") );
  temp_surf.push_back( getSurface( g_RESOURCE_LOC + "textures/" + _set + "/" + _set + "_shoot.png") );
  temp_surf.push_back( getSurface( g_RESOURCE_LOC + "textures/" + _set + "/" + _set + "_shield.png") );
  temp_surf.push_back( getSurface( g_RESOURCE_LOC + "textures/" + _set + "/" + _set + "_static.png") );

  std::vector<SDL_Texture*> temp_tex;
  for(auto &i : temp_surf)
  {
    if(i)
    {
      SDL_Texture * t = SDL_CreateTextureFromSurface( m_renderer, i );
      if(!t) std::cerr << "Texture set load error! " << SDL_GetError() << std::endl;
      temp_tex.push_back( t );
    }
    else
    {
      temp_tex.push_back( nullptr );
    }
    SDL_FreeSurface(i);
  }
  m_textures.insert({_key, temp_tex});
}

void renderer::update(const float _dt)
{
  m_cameraShake = clamp(m_cameraShake - _dt * 10.0f, 0.0f, 15.0f);

  //m_cameraShake = 15.0f;

  if(magns(m_cameraShakeTargetOffset - m_cameraShakeOffset) < 40.0f)
  {
    m_cameraShakeTargetOffset = randVec2(m_cameraShake);
  }
  m_cameraShakeOffset += (m_cameraShakeTargetOffset - m_cameraShakeOffset) * _dt * 4.0f;

  g_ZOOM_LEVEL +=  m_cameraShake * 0.0001f;
}

void renderer::loadFontSpriteSheet(
    std::string _name,
    std::string _path,
    int _size
    )
{
  sprite_sheet sheet;

  TTF_Font * fnt = TTF_OpenFont(_path.c_str(), _size);

  if(!fnt)
  {
    std::cerr << "Font loading error! " << SDL_GetError() << std::endl;
    TTF_CloseFont(fnt);
    return;
  }

  std::string ascii;
  for(int i = 0; i < 33; ++i) ascii += ' ';
  for(int i = 33; i < 127; ++i)
  {
    ascii += static_cast<char>(i);
  }
  ascii += "\0";

  for(size_t i = 0; i < ascii.length(); ++i)
  {
    //We need to first render to a surface as that's what TTF_RenderText
    //returns, then load that surface into a texture
    SDL_Surface * surf = TTF_RenderGlyph_Blended(fnt, ascii[i], {255,255,255});

    if(!surf)
    {
      std::cerr << "Font loading error! " << SDL_GetError() << std::endl;
      SDL_FreeSurface(surf);
      return;
    }

    SDL_Texture * texture = SDL_CreateTextureFromSurface(m_renderer, surf);

    //Clean up the surface.
    SDL_FreeSurface(surf);

    if(!texture)
    {
      std::cerr << "Font loading error! " << SDL_GetError() << std::endl;
      SDL_DestroyTexture(texture);
      return;
    }

    sheet.m_sheet.push_back(texture);
  }

  TTF_CloseFont(fnt);

  m_letters.insert({_name, sheet});
}

void renderer::queryTexture(
    std::string _identifier,
    int index,
    int *_w,
    int *_h
    )
{
  int rw = 0, rh = 0;
  SDL_QueryTexture( m_textures[_identifier][index], NULL, NULL, &rw, &rh );
  *_w = rw;
  *_h = rh;
}

void renderer::loadSpriteSheet()
{

}

void renderer::clear()
{
  //Clear renderer.
  SDL_SetRenderDrawColor( m_renderer, 4, 1, 8, 255);
  SDL_RenderClear( m_renderer );
}

void renderer::drawTextureSet(
    std::string _key,
    vec2 _pos,
    float _orient,
    std::array<float,4> _alphaMod
    )
{
  int w, h;
  SDL_QueryTexture(m_textures[_key].at(0), NULL, NULL, &w, &h);

  _pos *= g_ZOOM_LEVEL;
  _pos += g_HALFWIN;
  _pos += m_cameraShakeOffset;

  w *= g_ZOOM_LEVEL / 2;
  h *= g_ZOOM_LEVEL / 2;

  SDL_Rect dst;
  dst.x = _pos.m_x - (w/2);
  dst.y = _pos.m_y - (h/2);
  dst.w = w;
  dst.h = h;

  SDL_SetTextureAlphaMod(m_textures.at(_key).at(1), _alphaMod[0]);
  SDL_SetTextureAlphaMod(m_textures.at(_key).at(2), _alphaMod[1]);
  SDL_SetTextureAlphaMod(m_textures.at(_key).at(3), _alphaMod[2]);
  SDL_SetTextureAlphaMod(m_textures.at(_key).at(4), _alphaMod[3]);
  //SDL_SetTextureColorMod(m_textures.at(key).at(3), weapons[curWeap][4], weapons[curWeap][5], weapons[curWeap][6]);

  SDL_RenderCopyEx(m_renderer, m_textures[_key][5], NULL, &dst, 0, NULL, SDL_FLIP_NONE);
  for(int i = 0; i < 5; ++i) SDL_RenderCopyEx(m_renderer, m_textures[_key][i], NULL, &dst, _orient, NULL, SDL_FLIP_NONE);

}

void renderer::drawText(
    std::string _text,
    std::string _font,
    vec2 _pos,
    bool _ss,
    const float _mul
    )
{
  if(_ss)
  {
    _pos *= g_ZOOM_LEVEL;
    _pos += g_HALFWIN;
  }

  sprite_sheet * tmp = &m_letters[_font];

  SDL_Rect dst = {static_cast<int>(_pos.m_x), static_cast<int>(_pos.m_y), 0, 0};
  for(int i = 0; i < _text.length(); ++i)
  {
    SDL_Texture * draw = tmp->m_sheet[_text[i]];
    SDL_QueryTexture( draw, NULL, NULL, &dst.w, &dst.h);
    if(_ss)
    {
      dst.w *= g_ZOOM_LEVEL;
      dst.h *= g_ZOOM_LEVEL;
    }
    dst.w *= _mul;
    dst.h *= _mul;
    SDL_RenderCopy( m_renderer, draw, NULL, &dst );
    dst.x += dst.w;
  }
}

void renderer::drawTexture(
    std::string _key,
    size_t _index,
    vec2 _pos,
    float _orient,
    std::array<float,4> _col
    )
{
  int w, h;
  SDL_QueryTexture(m_textures[_key][_index], NULL, NULL, &w, &h);

  _pos *= g_ZOOM_LEVEL;
  _pos += g_HALFWIN;
  _pos += m_cameraShakeOffset;

  w *= g_ZOOM_LEVEL / 2;
  h *= g_ZOOM_LEVEL / 2;

  SDL_Rect dst;
  dst.x = _pos.m_x - (w/2);
  dst.y = _pos.m_y - (h/2);
  dst.w = w;
  dst.h = h;

  SDL_SetTextureColorMod(m_textures[_key][_index], _col[0], _col[1], _col[2]);
  SDL_SetTextureAlphaMod(m_textures[_key][_index], _col[3]);

  SDL_RenderCopyEx(m_renderer, m_textures[_key][_index], NULL, &dst, _orient, NULL, SDL_FLIP_NONE);

  SDL_SetTextureAlphaMod(m_textures[_key][_index], 255);
}

void renderer::drawLine(
    vec2 _start,
    vec2 _end,
    std::array<float, 4> _col
    )
{
  std::array<int, 4> _pcol = {static_cast<int>(_col[0]), static_cast<int>(_col[1]), static_cast<int>(_col[2]), static_cast<int>(_col[3])};
  drawLine(_start, _end, _pcol);
}

void renderer::drawLine(
    vec2 _start,
    vec2 _end,
    std::array<int, 4> _col
    )
{
  _start *= g_ZOOM_LEVEL;
  _start += g_HALFWIN;
  _start += m_cameraShakeOffset;

  _end *= g_ZOOM_LEVEL;
  _end += g_HALFWIN;
  _end += m_cameraShakeOffset;

  SDL_SetRenderDrawColor(m_renderer, _col[0], _col[1], _col[2], _col[3]);
  SDL_RenderDrawLine(m_renderer, _start.m_x, _start.m_y, _end.m_x, _end.m_y);
}

void renderer::drawLineGr(
    vec2 _start,
    vec2 _end,
    std::array<float, 4> _scol,
    std::array<float, 4> _ecol
    )
{
  _start *= g_ZOOM_LEVEL;
  _start += g_HALFWIN;
  _start += m_cameraShakeOffset;

  _end *= g_ZOOM_LEVEL;
  _end += g_HALFWIN;
  _end += m_cameraShakeOffset;

  SDL_SetRenderDrawColor(m_renderer, _scol[0], _scol[1], _scol[2], _scol[3]);
  int p0[2] = {static_cast<int>(_start.m_x), static_cast<int>(_start.m_y)};
  int p1[2] = {static_cast<int>(_end.m_x), static_cast<int>(_end.m_y)};
  int dx = p1[0] - p0[0], dy = p1[1] - p0[1];
  int cur[2] = {0, 0};

  int octant = 0;
  int M, m;

  if(abs(dx) > abs(dy))
  {
    M = abs(dx);
    m = abs(dy);

    if(dx > 0 and dy >= 0) octant = 0;
    else if(dx > 0 and dy < 0) octant = 7;
    else if(dx < 0 and dy > 0) octant = 3;
    else if(dx < 0 and dy <= 0) octant = 4;
  }
  else
  {
    M = abs(dy);
    m = abs(dx);

    if(dx > 0 and dy > 0) octant = 1;
    else if(dx >= 0 and dy < 0) octant = 6;
    else if(dx <= 0 and dy > 0) octant = 2;
    else if(dx < 0 and dy < 0) octant = 5;
  }

  float len = abs(dx) + abs(dy);

  int d = 2 * m - M;

  int count = 0;
  for(int i = 0; i < M; ++i)
  {
    float t = static_cast<float>(count) / len;

    int draw[2] = {cur[0], cur[1]};
    toOctant(&draw[0], &draw[1], octant);
    SDL_SetRenderDrawColor(m_renderer, t * _ecol[0] + (1 - t) * _scol[0], t * _ecol[1] + (1 - t) * _scol[1], t * _ecol[2] + (1 - t) * _scol[2], t * _ecol[3] + (1 - t) * _scol[3]);
    SDL_RenderDrawPoint(m_renderer, draw[0] + p0[0], draw[1] + p0[1]);

    d = d + 2 * m;
    cur[0]++;
    count++;
    if(d > 0)
    {
      cur[1]++;
      count++;
      d = d - (2 * M);
    }
  }
}

void renderer::drawCircle(int _x,
                          int _y,
                          int _radius,
                          std::array<float, 4> _col
                          )
{
  _x = _x * g_ZOOM_LEVEL + g_HALFWIN.m_x;
  _y = _y * g_ZOOM_LEVEL + g_HALFWIN.m_y;
  _x += m_cameraShakeOffset.m_x;
  _y += m_cameraShakeOffset.m_y;

  _radius *= g_ZOOM_LEVEL;

  SDL_SetRenderDrawColor(m_renderer, _col[0], _col[1], _col[2], _col[3]);
  for(int j = _y - _radius; j < _y + _radius; j++)
  {
    for(int i = _x - _radius; i < _x + _radius; i++)
    {
      if((i-_x)*(i-_x)+(j-_y)*(j-_y)<_radius*_radius)
      {
        SDL_RenderDrawPoint(m_renderer,i,j);
      }
    }
  }
}

void renderer::drawCircleUI(
    int _x,
    int _y,
    int _radius,
    std::array<int, 4> _col
    )
{
  SDL_SetRenderDrawColor(m_renderer, _col[0], _col[1], _col[2], _col[3]);
  for(int j = _y - _radius; j < _y + _radius; j++)
  {
    for(int i = _x - _radius; i < _x + _radius; i++)
    {
      if( sqr(i-_x) + sqr(j-_y) < sqr(_radius) )
      {
        SDL_RenderDrawPoint(m_renderer,i,j);
      }
    }
  }
}

void renderer::drawRect(
    vec2 _p,
    vec2 _d,
    std::array<int, 4> _col,
    bool _wire
    )
{
  SDL_Rect r = {static_cast<int>(_p.m_x), static_cast<int>(_p.m_y), static_cast<int>(_d.m_x), static_cast<int>(_d.m_y)};
  SDL_SetRenderDrawColor( m_renderer, _col[0], _col[1], _col[2], _col[3]);

  if(!_wire) SDL_RenderFillRect( m_renderer, &r );
  else if(_wire) SDL_RenderDrawRect( m_renderer, &r );
}

void renderer::finalise()
{
  //Show the contents of the renderer.
  SDL_RenderPresent(m_renderer);
}

//UI CODE
void renderer::drawMap(std::vector<missile> *_mp,
                       std::vector<enemy> *_ep,
                       std::vector<ship> *_ap,
                       std::vector<laser> *_lp,
                       std::vector<faction> * _fp
                       )
{
  SDL_Rect map;
  map.w = 256;
  map.h = 256;
  map.x = g_WIN_WIDTH - 256;
  map.y = 0;

  SDL_SetRenderDrawColor(m_renderer, 200, 200, 255, 100);
  SDL_RenderFillRect(m_renderer,&map);

  SDL_SetRenderDrawColor(m_renderer,0, 0, 0, 255);
  SDL_RenderDrawLine(m_renderer, g_WIN_WIDTH-128, 124, g_WIN_WIDTH-128, 132);
  SDL_RenderDrawLine(m_renderer, g_WIN_WIDTH-124, 128, g_WIN_WIDTH-132, 128);

  SDL_SetRenderDrawColor(m_renderer, 0, 0, 255, 255);
  for(unsigned int i = 0; i < _lp->size(); i++)
  {
    vec3 lpp = _lp->at(i).getPos();

    double x = clamp(lpp.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    double y = clamp(lpp.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    SDL_RenderDrawPoint(m_renderer, x, y);
  }

  SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
  for(unsigned int i = 0; i < _mp->size(); i++)
  {
    vec3 mpp = _mp->at(i).getPos();

    double x = clamp(mpp.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    double y = clamp(mpp.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    SDL_RenderDrawPoint(m_renderer, x, y);
  }

  SDL_SetRenderDrawColor(m_renderer, 200, 200, 200, 255);
  for(unsigned int i = 0; i < _ap->size(); i++)
  {
    vec3 app = _ap->at(i).getPos();

    double x = clamp(app.m_x / 156.0f + g_WIN_WIDTH - 128.0f,  g_WIN_WIDTH - 256.0f,  static_cast<float>(g_WIN_WIDTH));
    double y = clamp(app.m_y / 156.0f + 128.0f,  0.0f,  256.0f);

    int radius = 1;
    if(_ap->at(i).getClassification() == ASTEROID_MID) radius = 2;
    else if(_ap->at(i).getClassification() == ASTEROID_LARGE) radius = 3;

    drawCircleUI(x, y, radius, {200,200,200,255});
  }

  for(unsigned int i = 0; i < _ep->size(); i++)
  {
    vec3 epp = _ep->at(i).getPos();
    int radius = clamp( _ep->at(i).getRadius() / 16.0f,  1.0f,  5.0f );

    std::array<int, 4> col;
    col = _fp->at(_ep->at(i).getTeam()).m_colour;
    col[3] = 255;

    float x = clamp(epp.m_x / 156.0f + g_WIN_WIDTH - 128.0f, g_WIN_WIDTH - 256.0f, static_cast<float>(g_WIN_WIDTH));
    float y = clamp(epp.m_y / 156.0f + 128.0f, 0.0f, 256.0f);

    drawCircleUI(x,y,radius,col);
  }
}

void renderer::statusBars(player * _ply)
{
  //health base
  std::array<int, 4> col = {100, 20, 20, 255};
  drawRect({0,0}, {256, 16}, col, false);

  //health
  col = {230, 50, 50, 255};
  drawRect({0,0}, {(_ply->getHealth() / _ply->getMaxHealth()) * 256, 16}, col, false);

  //shield base
  col = {20, 20, 100, 255};
  drawRect({0,16}, {256, 16}, col, false);

  //shield
  col = {50, 50, 230, 255};
  drawRect({0,16}, {(_ply->getShield() / _ply->getMaxShield()) * 256, 16}, col, false);

  //energy base
  col = {20, 100, 20, 255};
  drawRect({0,32}, {256, 16}, col, false);

  //energy
  col = {50, 230, 50, 255};
  drawRect({0,32}, {(_ply->getEnergy() / _ply->getMaxEnergy()) * 256, 16}, col, false);
}

SDL_Surface * renderer::getSurface(std::string _path)
{
  SDL_Surface * s = IMG_Load( _path.c_str() );
  if(!s)
  {
    std::cerr << "Surface get load error! " << SDL_GetError() << std::endl;
    SDL_FreeSurface(s);
  }
  return s;
}

float renderer::getTextureRadius(std::string _identifier)
{
  int w = 0, h = 0;
  SDL_QueryTexture(m_textures.at(_identifier).at(0), NULL, NULL, &w, &h);
  return static_cast<float>(std::max(w, h)) / 4.0f;
}

void renderer::drawWeaponStats(player * _ply)
{
  SDL_Rect weap;
  weap.w = g_WIN_WIDTH * 0.1;
  weap.h = g_WIN_HEIGHT * 0.2;
  weap.x = g_WIN_WIDTH - weap.w;
  weap.y = g_WIN_HEIGHT - 1.4 * weap.h;

  std::array<float, 10> ws = _ply->getWeap();

  SDL_SetRenderDrawColor(m_renderer, ws[4] * 0.6, ws[5] * 0.6, ws[6] * 0.6, 255);
  SDL_RenderFillRect(m_renderer,&weap);

  float fWIN_WIDTH = static_cast<float>(g_WIN_WIDTH), fWIN_HEIGHT = static_cast<float>(g_WIN_HEIGHT);
  std::string damageText = "Damage " + std::to_string(ws[2]);
  drawText(damageText, "minimal", {fWIN_WIDTH - weap.w, fWIN_HEIGHT - 1.4f * weap.h}, false, 1.0f);

  std::string noText = "\nLasers " + std::to_string(static_cast<int>(ws[0]));
  drawText(noText, "minimal", {fWIN_WIDTH - weap.w, fWIN_HEIGHT - 1.2f * weap.h}, false, 1.0f);

  std::string spreadText = "\nSpread " + std::to_string(ws[1]);
  drawText(spreadText, "minimal", {fWIN_WIDTH - weap.w, fWIN_HEIGHT - 1.0f * weap.h}, false, 1.0f);

  std::string rateText = "\nRate ";
  float rate = 1.0f / ws[8];
  rateText += std::to_string( static_cast<int>( rate ) );
  drawText(rateText, "minimal", {fWIN_WIDTH - weap.w, fWIN_HEIGHT - 0.8f * weap.h}, false, 1.0f);
}

void renderer::addShake(float _s)
{
  m_cameraShake += _s;
  m_cameraShakeTargetOffset = randVec2(m_cameraShake);
}
