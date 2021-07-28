#ifndef MODEL3D_HEADER_FILE\
#define MODEL3D_HEADER_FILE\

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <malloc.h>

#include "vec3.h"

struct Model3D
{
    struct
    {
        struct vec3f* data_position;
        struct vec3f* data_color;

        uint32_t size;
        uint32_t capacity;
    } cells;
};

void model3DInit(struct Model3D* model, const char* file_name);

#endif // MODEL3D_HEADER_FILE\
