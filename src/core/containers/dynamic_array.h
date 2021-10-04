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

    DynamicArray(size_t capacity = DYNAMIC_ARRAY_DEFAULT_CAPACITY) : capacity(capacity), size(0)
    {
        assert(capacity != 0);

        data = (T*) calloc(capacity, sizeof(T));
        assert(data);
    }

    DynamicArray(std::initializer_list<T> init)
        : DynamicArray(init.size() == 0 ? DYNAMIC_ARRAY_DEFAULT_CAPACITY : init.size())
    {
        for (auto value : init)
        {
            pushBack(value);
        }
    }

    ~DynamicArray()
    {
        free(data);

        capacity = 0;
        size     = 0;
    }

    T& operator[](size_t i)
    {
        assert(i < size);

        return data[i];
    }

    Iterator begin() { return Iterator{data};        }
    Iterator end  () { return Iterator{data + size}; }

    void insert(T element)
    {
        if (size >= capacity)
        {
            resize(capacity * DYNAMIC_ARRAY_EXPAND_MULT);
        }

        data[size++] = element;
    }

    void pop()
    {
        assert(size > 0);

        --size;
    }

    void clear()
    {
        size = 0;
    }

    T& getFirst() const
    {
        assert(size > 0);
        return data[0];
    }

    T& getLast() const
    {
        assert(size > 0);
        return data[size - 1];
    }

    T* getData() const
    {
        return data;
    }

    size_t getCapacity() const
    {
        return capacity;
    }

    size_t getSize() const
    {
        return size;
    }

private:
    T*     data;
    size_t capacity;
    size_t size;

    void resize(size_t newCapacity)
    {
        assert(newCapacity > capacity);

        T* newData = (T*) realloc(data, newCapacity * sizeof(T));
        assert(newData);

        data     = newData;
        capacity = newCapacity;
    }
};

#endif // DYNAMIC_ARRAY_H