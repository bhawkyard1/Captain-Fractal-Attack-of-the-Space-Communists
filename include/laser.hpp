#include "src/base.cpp"

class laser: public base
{
	int dmg, spd;
	ai_team team;
  float ang, col[3], power, stop;
public:
	laser(vec2,vec2,float,float pData[], ai_team);
	int getDmg() {return dmg;}
  float getStop() {return stop;}
	void update(float);
	void draw(float);
	ai_team getTeam() {return team;}
	
	float getPower() {return power;}
};
