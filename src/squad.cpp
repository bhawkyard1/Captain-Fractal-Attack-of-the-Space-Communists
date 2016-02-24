#ifndef SQUAD_HPP
#define SQUAD_HPP

#include "squad.hpp"

squad::squad()
{

}

void squad::populate(ai_team t, size_t n)
{
  size_t rn = randNum(2, n);
  for(size_t i = 0; i < rn; ++i)
  {

  }
}

void update(float dt)
{
	for(auto i = squaddies.begin(); i != squaddies.end(); ++i) squaddies.at(i).update(dt);
}

void updateGoal()
{

}

#endif
