#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>

#include "debris.hpp"
#include "slotMap.hpp"

class inventory
{
public:
    inventory();
    bool addItem(const debris _in);
    void removeItem(const size_t index);
    void update(const float _dt);

    void setDim(const vec2 _dim) {m_dim = _dim;}
    vec2 getDim() const {return m_dim;}

    void toggleVisible() {m_visible = !m_visible;}
    void setVisible(const bool _visible) {m_visible = _visible;}
    bool isVisible() const {return m_visible;}

    slotMap<debris> * getItems() {return &m_contents;}

    void handleInput(const vec2 _mouse, std::vector<debris> * _fill);

    float getInvMass() const {return m_invMass;}
private:
    slotMap<debris> m_contents;
    vec2 m_dim;
    float m_invMass;
    bool m_visible;
};

#endif
