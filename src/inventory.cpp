#include "inventory.hpp"
#include "shapes.hpp"

inventory::inventory()
{
    m_dim = {0.0f, 0.0f};
    m_invMass = 1.0f;
    m_visible = false;
    m_used = 0.0f;
}

bool inventory::addItem(debris _in)
{
    m_used += sqr(_in.getRadius());

    if(full())
    {
        //If it would be full with this item, remove and return.
        m_used -= sqr(_in.getRadius());
        return false;
    }

    _in.setPos(
                tovec3(randVec2(-m_dim / 2.0f, m_dim / 2.0f))
                );

    _in.setVel(tovec3(randVec2(1.0f)));

    m_contents.push_back(_in);

    m_invMass = 1.0f / (1.0f / m_invMass + 1.0f / _in.getInertia());

    return true;
}

void inventory::removeItem(const size_t _index)
{
    m_used -= sqr(m_contents[_index].getRadius());
    std::cout << "  used " << m_used << '\n';
    m_invMass = 1.0f / (1.0f / m_invMass - 1.0f / m_contents[_index].getInertia());
    m_contents.free(_index);
}

void inventory::update(const float _dt)
{
    if(!m_visible) return;

    for(auto &i : m_contents.m_objects)
    {
        /*for(auto &j : m_contents)
        {
            if(&i == &j) continue;

            sphereSphereCollision(&i, &j);
        }*/

        //Apply drag.
        i.setVel( i.getVel() * 0.995f );
        i.updatePos(_dt);

        vec3 pos = i.getPos();
        vec3 vel = i.getVel();
        vec2 dim = m_dim / 2.0f;
        float COR = 0.6f;

        if(pos.m_x < -dim.m_x)
        {
            pos.m_x = -dim.m_x;
            vel.m_x = -COR * vel.m_x;
        }
        else if(pos.m_x > dim.m_x)
        {
            pos.m_x = dim.m_x;
            vel.m_x = -COR * vel.m_x;
        }

        if(pos.m_y < -dim.m_y)
        {
            pos.m_y = -dim.m_y;
            vel.m_y = -COR * vel.m_y;
        }
        else if(pos.m_y > dim.m_y)
        {
            pos.m_y = dim.m_y;
            vel.m_y = -COR * vel.m_y;
        }

        i.setPos(pos);
        i.setVel(vel);

        //}
    }
}

bool inventory::handleInput(const vec2 _mouse, std::vector<debris> * _fill)
{
    vec2 hdim = m_dim / 2.0f;
    if(!pointInRect( _mouse, -hdim, m_dim))
        return false;

    std::cout << "pickup check " << _mouse.m_x << ", " << _mouse.m_y << '\n';
    vec3 test = {_mouse.m_x, _mouse.m_y, 0.0f};
    for(size_t i = 0; i < m_contents.m_objects.size(); ++i)
    {
        std::cout << "dist " << mag(m_contents.m_objects[i].getPos() - test) << '\n';
        if(magns(m_contents.m_objects[i].getPos() - test) < sqr(m_contents.m_objects[i].getRadius()))
        {
            _fill->push_back(m_contents.m_objects[i]);
            removeItem(i);
            std::cout << "PICKUP!\n";
            break;
        }
    }

    return true;
}
