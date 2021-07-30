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

void matrixInit(struct Matrix* mat, uint32_t width, uint32_t height);
void matrixSetFloat(struct Matrix* mat, float value);
void matrixTranslate3f(struct Matrix* mat, float x, float y, float z);
float* matrixGetFloat(struct Matrix* mat, uint32_t x, uint32_t y);
void matrixLookAt(struct Matrix* mat,
                  float eyex, float eyey, float eyez,
                  float centerx, float centery, float centerz,
                  float upx, float upy, float upz,
                  struct Vector3fManager* manager);
void matrixPerpective(struct Matrix* mat, float angle_radians, float ratio, float near, float far);
void matrixDelete(struct Matrix* mat);

#endif // MATH_3D___HEADER_FILE
