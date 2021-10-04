//------------------------------------------------------------------------------
//! @author Nikita Mochalov (github.com/tralf-strues)
//! @file array.h
//! @date 2021-10-01
//! 
//! @copyright Copyright (c) 2021
//------------------------------------------------------------------------------

#ifndef ARRAY_H
#define ARRAY_H

#include <assert.h>
#include <stdlib.h>
#include <initializer_list>

template <typename T>
class ArrayIterator
{
public:
    typedef T* Pointer; 
    typedef T& Reference; 

    ArrayIterator(Pointer ptr) : ptr(ptr) {}

    // Prefix
    ArrayIterator<T>& operator++()
    {
        ++ptr;
        return *this;
    }

    // Postfix
    ArrayIterator<T> operator++(int)
    {
        ArrayIterator<T> it = *this;
        --(*this);
        return it;
    }

    // Prefix
    ArrayIterator<T>& operator--()
    {
        --ptr;
        return *this;
    }

    // Postfix
    ArrayIterator<T> operator--(int)
    {
        ArrayIterator<T> it = *this;
        --(*this);
        return it;
    }

    Reference operator*()
    {
        return *(ptr);
    }

    Pointer operator->()
    {
        return ptr;
    }

    bool operator==(const ArrayIterator<T>& second)
    {
        return ptr == second.ptr;
    }

    bool operator!=(const ArrayIterator<T>& second)
    {
        return ptr != second.ptr;
    }

private:
    Pointer ptr;
};

template<typename T>
class Array
{
public:
    typedef ArrayIterator<T> Iterator;

    Array(size_t size)
    {
        this->size = (size == 0) ? 1 : size;

        data = new T[this->size];
    }

    Array(std::initializer_list<T> init)
        : Array(init.size())
    {
        auto arrayIt = begin();
        auto initIt  = init.begin();

        while (arrayIt != end())
        {
            *arrayIt = *initIt;
            ++arrayIt;
            ++initIt;
        }
    }

    Array() : Array(1) {}

    ~Array()
    {
        assert(data);
        delete[] data;
    }

    T& operator[](size_t i)
    {
        assert(i < size);

        return data[i];
    }

    Iterator begin() { return Iterator{data};        }
    Iterator end  () { return Iterator{data + size}; }

    T* getData() const
    {
        return data;
    }

    size_t getSize() const
    {
        return size;
    }

private:
    T*     data;
    size_t size;
};

#endif // ARRAY_H