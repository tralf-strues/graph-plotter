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

template<typename T>
class Array
{
public:
    Array(size_t size) : size(size)
    {
        assert(size != 0);

        data = (T*) calloc(size, sizeof(T));
        assert(data);
    }

    ~Array()
    {
        free(data);

        size = 0;
    }

    T& operator[](size_t i)
    {
        assert(i < size);

        return data[i];
    }


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