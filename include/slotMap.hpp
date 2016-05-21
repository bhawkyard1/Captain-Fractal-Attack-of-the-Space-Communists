#ifndef SLOTMAP_HPP
#define SLOTMAP_HPP

#include <vector>

template<class t>
class slotMap<t>
{
public:
    std::vector<t> m_objects;

    t * getByID(long _i) {return &m_objects[m_ids[_i]];
    void push_back(const t _obj);
private:
    std::vector<long> m_ids;

    unsigned long m_counter;
};

#endif
