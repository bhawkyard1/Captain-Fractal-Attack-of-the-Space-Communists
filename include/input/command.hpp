#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "ship.hpp"

class command
{
public:
    virtual void execute() = 0;
};
/*
class accelerate<vec3> : public command
{
public:
    accelerate(ship * _actor, vec3 _vec);
    void execute();
private:
    ship * m_actor;
    vec3 m_vec;
}
*/
#endif
