#include <chrono>

class sim_time
{
	typedef std::chrono::high_resolution_clock hr_clock;
	
	const double tickRate = static_cast<const double>(1000000000.0f); 
	double time_since_creation;
	//Variables to hold the start and end measurements.
	long int start, cur;
	//The time (in seconds) between 'start' and 'end'.
	double diff;
	float sim_fps, sim_dt, sim_accumulator;
public:
	sim_time(float);
	void setStart();
	void setCur();
	double getDiff() {return diff;}
	
	double getTime() {return time_since_creation;}
	
	double getFrame() {return sim_dt;}
	double getAcc() {return sim_accumulator;}
	
	void incrAcc(float incr) {sim_accumulator += incr;}
};
