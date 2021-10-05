//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file list.h
//! @date 2021-09-26
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef LIST_H
#define LIST_H

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <initializer_list>

static const size_t LIST_DEFAULT_CAPACITY  = 8;
static const double LIST_EXPAND_MULTIPLIER = 1.8;

template <typename T>
class List;

template <typename T>
class ListIterator
{
public:
    typedef T*           Pointer; 
    typedef T&           Reference; 
    // typedef ListNode<T>* NodePointer;
    typedef List<T>*     ListPointer;

    ListIterator(ListPointer list = nullptr, int32_t id = 0) : m_List(list), m_Id(id) {}

    // Prefix
    ListIterator<T>& operator++()
    {
        m_Id = m_List->m_Nodes[m_Id].next;
        return *this;
    }

    // Postfix
    ListIterator<T> operator++(int)
    {
        ListIterator<T> tmp = *this;
        ++(*this);
        return tmp;
    }

    // Prefix
    ListIterator<T>& operator--()
    {
        m_Id = m_List->m_Nodes[m_Id].prev;
        return *this;
    }

    // Postfix
    ListIterator<T> operator--(int)
    {
        ListIterator<T> tmp = *this;
        --(*this);
        return tmp;
    }

    Reference operator*()
    {
        return m_List->m_Nodes[m_Id].value;
    }

    Pointer operator->()
    {
        return &m_List->m_Nodes[m_Id].value;
    }

    bool operator==(const ListIterator<T>& second)
    {
        return m_List->m_Nodes == second.m_List->m_Nodes && m_Id == second.m_Id;
    }

    bool operator!=(const ListIterator<T>& second)
    {
        return !(*this == second);
    }

    int32_t getId() const
    {
        return m_Id;
    }

private:
    ListPointer m_List;
    int32_t     m_Id;
};

template <typename T>
class List
{
private:
    struct ListNode
    {
        T       value;
        int32_t prev;
        int32_t next;

        ListNode(int32_t prev = 0, int32_t next = 0) : prev(prev), next(next) {}
    };

public:
    typedef ListIterator<T> Iterator;
    friend ListIterator<T>;

    List(size_t capacity = LIST_DEFAULT_CAPACITY) : m_Size(0), m_Capacity(capacity + 1), 
                                                    m_Head(0), m_Tail(0), m_Free(0)
    {
        assert(m_Capacity);

        m_Nodes = new ListNode[m_Capacity];
        updateFreeList();
    }

    List(std::initializer_list<T> init)
        : List(init.size() == 0 ? LIST_DEFAULT_CAPACITY : init.size())
    {
        for (auto value : init)
        {
            pushBack(value);
        }
    }

    ~List()
    {
        delete[] m_Nodes;
    }

    Iterator begin       ()       { return Iterator{this, m_Head}; }
    Iterator end         ()       { return Iterator{this, 0};      }
    size_t   getSize     () const { return m_Size;                    }
    size_t   getCapacity () const { return m_Capacity;                }

    Iterator insert(Iterator it, const T& value)
    {
        return Iterator{this, insertAfter(m_Nodes[it.id].prev, value)};
    }

    Iterator pushBack(const T& value)
    {
        return Iterator{this, insertAfter(m_Tail, value)};
    }

    Iterator pushFront(const T& value)
    {
        return Iterator{this, insertAfter(0, value)};
    }

    void remove(Iterator iterator)
    {
        remove(iterator.getId());
    }

    Iterator find(const T& value)
    {
        for (Iterator it = begin(); it != end(); ++it)
        {
            if (*it == value)
            {
                return it;
            }
        }

        return end();
    }

    void popBack()
    {
        remove(m_Tail);
    }

    void popFront()
    {
        remove(m_Head);
    }

    void clear()
    {
        m_Head           = 0;
        m_Tail           = 0;
        m_Free           = 1;
        m_Size           = 0;

        m_Nodes[0].next  = 0;
        m_Nodes[0].prev  = 0;

        updateFreeList(1);
    }

private:
    ListNode* m_Nodes;
    size_t    m_Size;
    size_t    m_Capacity;

    int32_t   m_Head;
    int32_t   m_Tail;
    int32_t   m_Free;

    void resize(size_t newCapacity)
    {
        assert(newCapacity > m_Capacity);

        ListNode* newNodes = new ListNode[newCapacity];
        memcpy(newNodes, m_Nodes, m_Capacity * sizeof(ListNode));

        delete[] m_Nodes;
        m_Nodes = newNodes;

        size_t prevCapacity = m_Capacity;
        m_Capacity = newCapacity;

        updateFreeList(prevCapacity);
    }
    
    void updateFreeList(int32_t begin = 1)
    {
        if (m_Free == 0) { m_Free = begin; }

        for (size_t i = begin; i < m_Capacity; ++i)
        {
            m_Nodes[i].prev = -1;

            if (m_Free == begin && i == m_Capacity - 1)
            {
                m_Nodes[i].next = 0;
            }
            else if (i == m_Capacity - 1)
            {
                m_Nodes[i].next = m_Free;
            }
            else
            {
                m_Nodes[i].next = i + 1;
            }
        }
    }

    int32_t insertAfter(int32_t id, const T& value)
    {
        assert(id >= 0 && id < static_cast<int32_t>(m_Capacity));

        if (m_Size == m_Capacity - 1)
        {
            resize((double) m_Capacity * LIST_EXPAND_MULTIPLIER);
        }

        int32_t newFree = m_Nodes[m_Free].next;

        if (m_Size == 0)
        {
            m_Nodes[m_Free].prev = 0;
            m_Nodes[m_Free].next = 0;
            m_Head               = m_Free;
            m_Tail               = m_Free;
        }
        else if (id == m_Tail)
        {
            m_Nodes[m_Free].next = 0;
            m_Nodes[m_Free].prev = m_Tail;
            m_Nodes[m_Tail].next = m_Free;
            m_Tail               = m_Free;
        }
        else if (id == 0)
        {
            m_Nodes[m_Free].next = m_Head;
            m_Nodes[m_Free].prev = 0;
            m_Nodes[m_Head].prev = m_Free;
            m_Head               = m_Free;
        }
        else
        {
            m_Nodes[m_Free].next           = m_Nodes[id].next;
            m_Nodes[m_Free].prev           = id;
            m_Nodes[m_Nodes[id].next].prev = m_Free;
            m_Nodes[id].next               = m_Free;
        }

        int insertedId = m_Free;

        m_Nodes[m_Free].value = value;
        m_Free = newFree;
        m_Size++;

        return insertedId;
    }

    void remove(int32_t id)
    {
        if (m_Nodes[id].prev != 0)
        {
            m_Nodes[m_Nodes[id].prev].next = m_Nodes[id].next;
        }

        if (m_Nodes[id].next != 0)
        {
            m_Nodes[m_Nodes[id].next].prev = m_Nodes[id].prev;
        }

        if (id == m_Head)
        {
            m_Head = m_Nodes[id].next;
        }

        if (id == m_Tail)
        {
            m_Tail = m_Nodes[id].prev;
        }

        m_Nodes[id].prev = 0;
        m_Nodes[id].next = m_Free;
        m_Free           = id;

        m_Size--;
    }
};

#endif // LIST_H