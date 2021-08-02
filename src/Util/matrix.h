#ifndef MATH_3D___HEADER_FILE
#define MATH_3D___HEADER_FILE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#include <math.h>
#include <assert.h>
#include <SDL2/SDL_stdinc.h>

#include "vec3.h"

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

struct Matrix
{
    float* data;
    uint32_t width;
    uint32_t height;
};

struct MatrixManager
{
    void (*init)(struct Matrix* mat, uint32_t width, uint32_t height);
    void (*setFloat)(struct Matrix* mat, float value);
    void (*translate3f)(struct Matrix* mat, float x, float y, float z);
    float* (*getFloat)(struct Matrix* mat, uint32_t x, uint32_t y);
    void (*lookAt)(struct Matrix* mat,
                    float eyex, float eyey, float eyez,
                    float centerx, float centery, float centerz,
                    float upx, float upy, float upz,
                    struct Vector3fManager* manager);
    void (*perpective)(struct Matrix* mat, float angle_radians, float ratio, float near, float far);
    void (*delete)(struct Matrix* mat);
};

struct MatrixManager matrixManagerInit();

#endif // MATH_3D___HEADER_FILE
