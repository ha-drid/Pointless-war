#ifndef OBJECT_HEADER_FILE
#define OBJECT_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#include <assert.h>

struct Object
{
    float x, y, z;
    float xVelocity, yVelocity, zVelocity;
    float width, height, depth;
};

struct ObjectManager
{
    void (*setPosition)(struct Object* obj, float x, float y, float z);
    void (*update)(struct Object* obj, float time);
    void (*init)(struct Object* obj, float x, float y, float z, float width, float height, float depth);
    void (*draw)(struct Object* obj, void (*voxelDraw)(float x, float y, float z));
};

struct ObjectManager objectManagerInit();

#endif // OBJECT_HEADER_FILE
