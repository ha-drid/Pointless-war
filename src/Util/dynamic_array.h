#ifndef C_VECTOR_H_INCLUDED
#define C_VECTOR_H_INCLUDED

#include <stdint-gcc.h>
#include <malloc.h>

#define dynamicArray__push_back(type, data, capacity, size, value)   \
    if (capacity == size)                                            \
    {                                                                \
        capacity += 3;                                               \
        data = realloc(data, sizeof(type) * capacity);         \
    }                                                                \
                                                                     \
    data[size] = value;                                              \
    ++size;                                                          \


#endif // C_VECTOR_H_INCLUDED
