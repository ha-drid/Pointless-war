#ifndef CVECTOR_HEADER_FILE_INCLUDED
#define CVECTOR_HEADER_FILE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define vector(type) \
    struct \
    { \
        type* data; \
        size_t capacity; \
        size_t size; \
        type element; /*поле element нужен дл€ того чтобы пон€ть сколько байт будет занимат экземпл€р этого типа */ \
    }


#define vectorInit(vector, csize) \
    vector.capacity = csize; \
    vector.size = csize; \
    memset(&(vector.element), 0, sizeof(vector.element)); \
    if (csize == 0) \
        vector.data = NULL;\
    else \
        vector.data = calloc(sizeof(vector.element), csize);

#define vectorPushBack(vector, value) \
    if (vector.size == vector.capacity) \
    { \
        vector.capacity += 3; \
        vector.data = realloc(vector.data, sizeof(vector.element) * vector.capacity); \
    } \
    vector.data[vector.size] = value; \
    ++vector.size;


#define vectorDelete(vector) \
    free(vector.data); \
    vector.size = 0; \
    vector.capacity = 0; \

#endif // CVECTOR_HEADER_FILE_INCLUDED
