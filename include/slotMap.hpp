#ifndef SLOTMAP_HPP
#define SLOTMAP_HPP

#include <stddef.h>
#include <vector>

struct uniqueID
{
    long m_id;
    long m_version;
};

bool operator ==(const uniqueID &_lhs, const uniqueID &_rhs);
bool operator !=(const uniqueID &_lhs, const uniqueID &_rhs);

template<class t>

class slotMap
{
public:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Vector of raw objects.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<t> m_objects;

    t * getByID(uniqueID _i)
    {
        if(m_ids[ m_indirection[ _i.m_id ] ].m_version == _i.m_version)
            return &m_objects[ m_indirection[ _i.m_id ] ];
        return nullptr;
    }

    void push_back(const t &_obj)
    {
        if(m_freeList.size() > 0)
        {
            m_indirection[ m_freeList.back().m_id ] = m_objects.size();
            m_ids.push_back( m_freeList.back() );
            m_freeList.pop_back();
        }
        else
        {
            m_indirection.push_back( m_objects.size() );
            uniqueID id = {static_cast<long>(m_objects.size()), 0};
            m_ids.push_back( id );
        }
        m_objects.push_back(_obj);

    }

    void swap(size_t _a, size_t _b)
    {
        size_t swap = m_indirection[_a];
        m_indirection[_a] = _b;
        m_indirection[_b] = swap;

        iter_swap( m_objects.begin() + _a, m_objects.begin() + _b );
        iter_swap( m_ids.begin() + _a, m_ids.begin() + _b );
    }

    void pop()
    {
        //Destroy object.
        m_objects.pop_back();

        //Add to freelist id and incremented version.
        m_freeList.push_back({m_ids.back().m_id, ++m_ids.back().m_version});
        m_ids.pop_back();
    }

    void free(size_t _i)
    {
        swap(_i, m_objects.size() - 1);
        pop();
    }

    t& back() const {return m_objects.back();}

    void clear()
    {
        m_objects.clear();
        m_ids.clear();
        m_indirection.clear();
    }

    size_t size() const {return m_objects.size();}

    uniqueID getID(size_t _i) const {return m_ids[_i];}

    t operator [](size_t _i) const {return m_objects[_i];}
    t & operator [](size_t _i) {return m_objects[_i];}

private:
    //----------------------------------------------------------------------------------------------------------------------
    /// \brief The index of each entry is the id of the object. The contents is the index of the object. Confused? Me too.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector<long> m_indirection;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief Means we do not have to store ids in the object, this matches movements of m_objects by index.
    /// Nested std::pair goes id, version
    //----------------------------------------------------------------------------------------------------------------------
    std::vector< uniqueID > m_ids;

    //----------------------------------------------------------------------------------------------------------------------
    /// \brief List of all free IDs.
    //----------------------------------------------------------------------------------------------------------------------
    std::vector< uniqueID > m_freeList;
};

#endif
