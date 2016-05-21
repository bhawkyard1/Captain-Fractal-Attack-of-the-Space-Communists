#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include <vector>

#include "debris.hpp"

class inventory
{
public:
    inventory();
    bool addItem(const debris _in);
    void update(const float _dt);

    void setDim(const vec2 _dim) {m_dim = _dim;}
    vec2 getDim() const {return m_dim;}

    void toggleVisible() {m_visible = !m_visible;}
    void setVisible(const bool _visible) {m_visible = _visible;}
    bool isVisible() const {return m_visible;}

    std::vector<debris> * getItems() {return &m_contents;}

    debris handleInput(const vec2 _mouse);
private:
    std::vector<debris> m_contents;
    vec2 m_dim;
    float m_mass;
    bool m_visible;
};

#endif
