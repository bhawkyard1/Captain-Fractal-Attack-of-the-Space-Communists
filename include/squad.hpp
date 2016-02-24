#ifndef SQUAD_HPP
#define SQUAD_HPP

#include "enemy.hpp"
#include <vector>

class squad
{
  ai_goal team_goal;
  void updateGoal();
  std::vector<enemy> squaddies;
public:
  squad();
  void populate(ai_team, int n);
  void update(float);
};

#endif
