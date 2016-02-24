#include "src/base.cpp"

class stardust: public base
{
	float z;
	float col[4];
public:
	stardust(float*);
	stardust(float);
	void gen(bool regen, float * colp);
	void draw(float);
	
	void updatePos(float dt);
	
	void setZ(float pz) {z = pz;}
	float getZ() {return z;}
	float getCol(int i) {return col[i];}
	void setCol(int i, float v) {col[i] = v;}
};
