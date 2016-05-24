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

    //Swaps item at index _a with item at index _b
    void swap(size_t _a, size_t _b)
    {
        //Store entry pointed to by the id of _a
        size_t swap = m_indirection[ m_ids[_a].m_id ];

        //Make the entry at _a's id point to _b's future index.
        m_indirection[ m_ids[_a].m_id ] = _b;

        //Make the entry at _b's id point to _a's future index.
        m_indirection[ m_ids[_b].m_id ] = swap;

        std::swap( m_objects[_a], m_objects[_b] );
        std::swap( m_ids[_a], m_ids[_b] );
    }

    void pop()
    {
        //Destroy object.
        m_objects.pop_back();

        //Add to freelist id and incremented version.
        m_freeList.push_back({m_ids.back().m_id, m_ids.back().m_version + 1});
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
