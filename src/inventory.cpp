#include "inventory.hpp"
#include "shapes.hpp"

inventory::inventory()
{
    m_dim = {0.0f, 0.0f};
    m_mass = 0.0f;
    m_visible = false;
}

void inventory::addItem(debris _in)
{
    float area = m_dim.m_x * m_dim.m_y;
    for(auto &i : m_contents) area -= sqr(i.getRadius());

    if(area < sqr(_in.getRadius())) return;

    _in.setPos(
                tovec3(randVec2({0.0f, 0.0f}, m_dim))
                       );

    m_contents.push_back(_in);
}

void inventory::update(const float _dt)
{
    if(!m_visible) return;

    for(auto &i : m_contents)
    {
        //Apply drag.
        i.setVel( i.getVel() * 0.95f );
        i.updatePos(_dt);

        vec3 pos = i.getPos();
        float radius = i.getRadius();

        //Collision with edge of inventory.
        if(!circleInRect(tovec2(pos), radius, {0.0f, 0.0f}, m_dim))
        {
            //Closest point in rect to the entity
            vec3 closest = {0.0f, 0.0f, 0.0f};
            closest.m_x = clamp(pos.m_x, 0.0f, m_dim.m_x);
            closest.m_y = clamp(pos.m_y, 0.0f, m_dim.m_y);

            vec3 normal = closest - pos;
            float dist = mag(normal);
            normal /= dist;

            i.setPos({closest.m_x, closest.m_y, 0.0f});

            normal *= mag(i.getVel()) * 0.5f;

            i.setVel(-normal);
        }

        for(auto &j : m_contents)
        {
            if(&i == &j) continue;

            sphereSphereCollision(&i, &j);
        }
    }
}
