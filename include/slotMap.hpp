#ifndef SLOTMAP_HPP
#define SLOTMAP_HPP

#include <vector>

template<class t>

class slotMap
{
public:
    slotMap<t>() : m_counter(0) {}

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of raw objects.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<t> m_objects;

    t * getByID(long _i) {return &m_objects[m_ids[_i]];}

    void push_back(const t &_obj)
    {
        m_ids.push_back( m_objects.size() );
        m_objects.push_back(_obj);
    }

    void swap(size_t _a, size_t _b)
    {

    }

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The index
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<long> m_ids;
};

#endif
