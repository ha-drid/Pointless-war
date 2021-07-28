#ifndef MATH_3D___HEADER_FILE
#define MATH_3D___HEADER_FILE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <math.h>
#include <SDL2/SDL_stdinc.h>

#include "vec3.h"

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

typedef struct Matrix
{
    float** data;
    uint32_t width;
    uint32_t height;
} Matrix;

typedef struct MathCalculator
{

} MathCalculator;

void createMatrix(Matrix* matrix, uint32_t width, uint32_t height);
void matrixPerpective(Matrix* matrix, float angle_radians, float ratio, float near, float far);
//
void matrixLookAt(Matrix* matrix, vec3f position, vec3f direction, vec3f up);
void deleteMatrix(Matrix* matrix);

#endif // MATH_3D___HEADER_FILE
