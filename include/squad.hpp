#include "enemy.cpp"
#include <vector>

class squad
{
  void updateGoal();
  vector<enemy> squaddies;
public:
  squad();
  void populate(ai_team, size_t n);
  void update(float);
};
