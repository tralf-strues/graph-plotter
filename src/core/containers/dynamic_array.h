//------------------------------------------------------------------------------
//! @brief General dynamic array container.
//! 
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file dynamic_array.h
//! @date 2021-09-15
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <assert.h>
#include <stdlib.h>
#include <initializer_list>
#include "array.h"

static const double DYNAMIC_ARRAY_EXPAND_MULT      = 1.8; ///< Expand multiplier for data realloc.
static const size_t DYNAMIC_ARRAY_DEFAULT_CAPACITY = 4;   ///< Default capacity (when unspecified).

template<typename T>
class DynamicArray
{
public:
    typedef ArrayIterator<T> Iterator;

    DynamicArray(size_t capacity = DYNAMIC_ARRAY_DEFAULT_CAPACITY) : m_Capacity(capacity), m_Size(0)
    {
        assert(m_Capacity != 0);

        m_Data = new T[m_Capacity];
        assert(m_Data);
    }

    DynamicArray(std::initializer_list<T> init)
        : DynamicArray(init.size() == 0 ? DYNAMIC_ARRAY_DEFAULT_CAPACITY : init.size())
    {
        assert(m_Capacity != 0);

        for (auto value : init)
        {
            pushBack(value);
        }
    }

    ~DynamicArray()
    {
        assert(m_Data);
        delete[] m_Data;

        m_Capacity = 0;
        m_Size     = 0;
    }

    T& operator[](size_t i)
    {
        assert(i < m_Size);

        return m_Data[i];
    }

    Iterator begin() { return Iterator{m_Data};        }
    Iterator end  () { return Iterator{m_Data + m_Size}; }

    void insert(T element)
    {
        if (m_Size >= m_Capacity)
        {
            resize(m_Capacity * DYNAMIC_ARRAY_EXPAND_MULT);
        }

        m_Data[m_Size++] = element;
    }

    void pop()
    {
        assert(m_Size > 0);

        --m_Size;
    }

    void clear()
    {
        m_Size = 0;
    }

    T& getFirst() const
    {
        assert(m_Size > 0);
        return m_Data[0];
    }

    T& getLast() const
    {
        assert(m_Size > 0);
        return m_Data[m_Size - 1];
    }

    T* getData() const
    {
        return m_Data;
    }

    size_t getCapacity() const
    {
        return m_Capacity;
    }

    size_t getSize() const
    {
        return m_Size;
    }

private:
    T*     m_Data;
    size_t m_Capacity;
    size_t m_Size;

    void resize(size_t newCapacity)
    {
        assert(newCapacity > m_Capacity);

        T* newData = (T*) realloc(m_Data, newCapacity * sizeof(T));
        assert(newData);

        m_Data     = newData;
        m_Capacity = newCapacity;
    }
};

#endif // DYNAMIC_ARRAY_H