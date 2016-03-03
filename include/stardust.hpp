#ifndef STARDUST_HPP
#define STARDUST_HPP

#include "base.hpp"

class stardust: public base
{
	float z;
	float col[4];
public:
	stardust(float*);
	stardust(float);
	void gen(bool regen, float * colp);
	
	void updatePos(float dt);
	
	void setZ(float pz) {z = pz;}
	float getZ() {return z;}
	float getCol(int i) {return col[i];}
	void setCol(int i, float v) {col[i] = v;}
};

#endif
