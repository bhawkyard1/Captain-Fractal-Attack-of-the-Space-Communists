#ifndef SIM_TIME
#define SIM_TIME

#include "sim_time.hpp"

sim_time::sim_time(float fps)
{
	start = hr_clock::now().time_since_epoch().count();
	cur = hr_clock::now().time_since_epoch().count();
	
	sim_fps = fps;
	sim_dt = 1.0 / sim_fps;
	sim_accumulator = 0.0;
	
	time_since_creation = 0.0;
}

void sim_time::setStart()
{
	start = hr_clock::now().time_since_epoch().count();
	diff = (cur - start) / tickRate;
	
	time_since_creation += diff;
}

void sim_time::setCur()
{
	cur = hr_clock::now().time_since_epoch().count();
	
	//cout << hr_clock::now().time_since_epoch().count() << endl;
	
	diff = (cur - start) / tickRate;
	sim_accumulator += diff;
	
	start = cur;
	
	if(sim_accumulator > 0.2) sim_accumulator = 0.2;
	
	time_since_creation += diff;
}

#endif
