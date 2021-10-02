#ifndef OBJECT_HEADER_FILE
#define OBJECT_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint-gcc.h>
#include <stdbool.h>
#include <assert.h>
#include <colision.h>
#include <cglm/cglm.h>

typedef enum {
    objectMoveForward = 0,
    objectMoveBackward = 1,
    objectMoveRight = 2,
    objectMoveLeft = 3,
} ObjectMove;

typedef struct Object
{
    vec3 position;
    vec3 velocity;
    vec3 size;
    float xAngle, yAngle;
    float speed;
    bool isOnGround;
} Object;

typedef struct ObjectManager
{
    void (*setPosition)(Object* const obj, vec3 pos);
    void (*setAngle)(Object* const obj, float xAngle, float yAngle);
    void (*setVelocity)(Object* const obj, vec3 value);
    bool (*isColision)(Object* const obj, float x, float y, float z, float width, float height, float depth);
    void (*update)(Object* const obj,
                         float time,
                         bool (*objectWorldIsColision)(vec3 position, vec3 size));
    void (*init)(Object* const obj, vec3 pos, vec3 size, float xAngle, float yAngle, float speed);
    void (*move)(Object* const obj, ObjectMove move, bool x, bool y, bool z);
    void (*vertMove)(Object* const obj, float gravity);
    void (*draw)(Object* const obj, void (*const voxelDraw)(float x, float y, float z));
} ObjectManager;

ObjectManager objectManagerInit();

void objectGetPosition(void* const value, float** position);
void objectGetVelocity(void* const value, float** velocity);
void objectGetSize(void* const value, float** size);
void objectGetSpeed(void* const value, float** speed);
void objectGetIsOnGround(void* const value, bool** isOnGround);

#endif // OBJECT_HEADER_FILE

