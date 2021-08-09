#ifndef OBJECT_HEADER_FILE
#define OBJECT_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#include <assert.h>
#include <colision.h>

typedef enum {
    objectMoveForward = 0,
    objectMoveBackward = 1,
    objectMoveRight = 2,
    objectMoveLeft = 3,
} ObjectMove;

struct Object
{
    float x, y, z;
    float xVelocity, yVelocity, zVelocity;
    float width, height, depth;
    float xAngle, yAngle;
    float speed;
};

struct ObjectManager
{
    void (*setPosition)(struct Object* obj, float x, float y, float z);
    void (*setAngle)(struct Object* obj, float xAngle, float yAngle);
    void (*setVelocity)(struct Object* obj, float x, float y, float z);
    bool (*isColision)(struct Object* obj, float x, float y, float z, float width, float height, float depth);
    void (*update)(struct Object* obj,
                         double time,
                         bool (*objectWorldIsColision)(struct Object* obj));
    void (*init)(struct Object* obj,
                 float x, float y, float z,
                 float width, float height, float depth,
                 float xAngle, float yAngle,
                 float speed);
    void (*move)(struct Object* obj, ObjectMove move, bool x, bool y, bool z);
    void (*draw)(struct Object* obj, void (*voxelDraw)(float x, float y, float z));
};

struct ObjectManager objectManagerInit();

#endif // OBJECT_HEADER_FILE
