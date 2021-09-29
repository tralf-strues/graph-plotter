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
#include <inttypes.h>

static const size_t LIST_DEFAULT_CAPACITY  = 8;
static const double LIST_EXPAND_MULTIPLIER = 1.8;

template <typename T>
struct ListNode
{
    T       value;
    int32_t prev;
    int32_t next;

    ListNode(int32_t prev = -1, int32_t next = -1) : prev(prev), next(next) {}
};

template <typename T>
class ListIterator
{
public:
    typedef T*           Pointer; 
    typedef T&           Reference; 
    typedef ListNode<T>* NodePointer; 

    ListIterator(NodePointer nodes, int32_t id) : nodes(nodes), id(id) {}

    // Prefix
    ListIterator<T>& operator++()
    {
        id = nodes[id].next;
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
        id = nodes[id].prev;
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
        return nodes[id].value;
    }

    Pointer operator->()
    {
        return &nodes[id].value;
    }

    bool operator==(const ListIterator<T>& second)
    {
        return nodes == second.nodes && id == second.id;
    }

    bool operator!=(const ListIterator<T>& second)
    {
        return !(*this == second);
    }

    int32_t getId() const
    {
        return id;
    }

private:
    NodePointer nodes;
    int32_t     id;
};

template <typename T>
class List
{
public:
    typedef ListIterator<T> Iterator;

    List(size_t capacity = LIST_DEFAULT_CAPACITY) : size(0), capacity(capacity + 1), 
                                                    head(-1), tail(-1), free(0)
    {
        assert(capacity);

        nodes = new ListNode<T>[capacity];
        updateFreeList();
    }

    ~List()
    {
        delete[] nodes;
    }

    Iterator begin       ()       { return Iterator{nodes, head}; }
    Iterator end         ()       { return Iterator{nodes, 0};    }
    size_t   getSize     () const { return size;                  }
    size_t   getCapacity () const { return capacity;              }

    Iterator insert(Iterator it, const T& value)
    {
        return Iterator{nodes, insertAfter(nodes[it.id].prev, value)};
    }

    Iterator pushBack(const T& value)
    {
        return Iterator{nodes, insertAfter(tail, value)};
    }

    Iterator pushFront(const T& value)
    {
        return Iterator{nodes, insertAfter(0, value)};
    }

    void remove(Iterator iterator)
    {
        remove(iterator.getId());
    }

    void popBack()
    {
        remove(tail);
    }

    void popFront()
    {
        remove(head);
    }

    void clear()
    {
        head           = 0;
        tail           = 0;
        free           = 1;
        size           = 0;

        nodes[0].next  = 0;
        nodes[0].prev  = 0;

        updateFreeList(1);
    }

    Iterator find(const T& value)
    {
        int curId = head;
        for (size_t i = 1; i <= size; i++)
        {
            if (nodes[curId].value == value)
            {
                return {nodes, curId};
            }

            curId = nodes[curId].next;
        }

        return end();
    }

private:
    ListNode<T>* nodes;
    size_t       size;
    size_t       capacity;

    int32_t      head;
    int32_t      tail;
    int32_t      free;

    void resize(size_t newCapacity)
    {
        assert(newCapacity > capacity);

        ListNode<T>* newNodes = new ListNode<T>[newCapacity];
        memcpy(newNodes, nodes, capacity * sizeof(ListNode<T>));
        
        nodes = newNodes;
        capacity = newCapacity;
    }
    
    void updateFreeList(int32_t begin = 1)
    {
        if (free == 0) { free = begin; }

        for (size_t i = begin; i < capacity; ++i)
        {
            nodes[i].prev = -1;

            if (free == begin && i == capacity - 1)
            {
                nodes[i].next = 0;
            }
            else if (i == capacity - 1)
            {
                nodes[i].next = free;
            }
            else
            {
                nodes[i].next = i + 1;
            }
        }
    }

    int32_t insertAfter(int32_t id, const T& value)
    {
        if (size == capacity - 1)
        {
            resize((double) capacity * LIST_EXPAND_MULTIPLIER);
        }

        int32_t newFree = nodes[free].next;

        if (size == 0)
        {
            nodes[free].prev = 0;
            nodes[free].next = 0;
            head             = free;
            tail             = free;
        }
        else if (id == tail)
        {
            nodes[free].next = 0;
            nodes[free].prev = tail;
            nodes[tail].next = free;
            tail             = free;
        }
        else if (id == 0)
        {
            nodes[free].next = head;
            nodes[free].prev = 0;
            nodes[head].prev = free;
            head             = free;
        }
        else
        {
            nodes[free].next            = nodes[id].next;
            nodes[free].prev            = id;
            nodes[nodes[id].next].prev  = free;
            nodes[id].next              = free;
        }

        int insertedId = free;

        nodes[free].value = value;
        free = newFree;
        size++;

        return insertedId;
    }

    void remove(int32_t id)
    {
        if (nodes[id].prev != 0)
        {
            nodes[nodes[id].prev].next = nodes[id].next;
        }

        if (nodes[id].next != 0)
        {
            nodes[nodes[id].next].prev = nodes[id].prev;
        }

        if (id == head)
        {
            head = nodes[id].next;
        }

        if (id == tail)
        {
            tail = nodes[id].prev;
        }

        nodes[id].prev = -1;
        nodes[id].next = free;
        free           = id;

        size--;
    }
};

#endif // LIST_H