#include "squad.hpp"
#include "util.hpp"

squad::squad()
{

}

void squad::populate(ai_team t, int n)
{
  size_t rn = randNum(2, n);
  for(size_t i = 0; i < rn; ++i)
  {

  }
}

void squad::update(float dt)
{
  for(auto i = squaddies.begin(); i != squaddies.end(); ++i) i->update(dt);
}

void updateGoal()
{

}
