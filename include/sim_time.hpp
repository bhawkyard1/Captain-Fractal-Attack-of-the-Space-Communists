#ifndef SIM_TIME_HPP
#define SIM_TIME_HPP

#include <chrono>

class sim_time
{
	typedef std::chrono::high_resolution_clock hr_clock;
	
  const double m_tickRate = static_cast<const double>(1000000000.0f);
  double m_time_since_creation;
	//Variables to hold the start and end measurements.
  long int m_start;
  long int m_cur;
	//The time (in seconds) between 'start' and 'end'.
  double m_diff;
  float m_sim_fps;
  float m_sim_dt;
  float m_sim_accumulator;
public:
  sim_time(float _fps);
	void setStart();
	void setCur();
  double getDiff() const {return m_diff;}
	
  double getTime() const {return m_time_since_creation;}
	
  double getFrame() const {return m_sim_dt;}
  double getAcc() const {return m_sim_accumulator;}
	
  void incrAcc(float _incr) {m_sim_accumulator += _incr;}
#endif
};
