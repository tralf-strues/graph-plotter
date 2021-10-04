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

    ArrayIterator(Pointer ptr) : m_Ptr(ptr) {}

    // Prefix
    ArrayIterator<T>& operator++()
    {
        ++m_Ptr;
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
        --m_Ptr;
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
        return *(m_Ptr);
    }

    Pointer operator->()
    {
        return m_Ptr;
    }

    bool operator==(const ArrayIterator<T>& second)
    {
        return m_Ptr == second.m_Ptr;
    }

    bool operator!=(const ArrayIterator<T>& second)
    {
        return m_Ptr != second.m_Ptr;
    }

private:
    Pointer m_Ptr;
};

template<typename T>
class Array
{
public:
    typedef ArrayIterator<T> Iterator;

    Array(size_t size) : m_Size(size)
    {
        assert(m_Size);

        m_Data = new T[m_Size];
    }

    Array(std::initializer_list<T> init) : Array(init.size())
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
        assert(m_Data);
        delete[] m_Data;
    }

    T& operator[](size_t i)
    {
        assert(i < m_Size);

        return m_Data[i];
    }

    Iterator begin() { return Iterator{m_Data};        }
    Iterator end  () { return Iterator{m_Data + m_Size}; }

    T* getData() const
    {
        assert(m_Data);

        return m_Data;
    }

    size_t getSize() const
    {
        return m_Size;
    }

private:
    T*     m_Data;
    size_t m_Size;
};

#endif // ARRAY_H