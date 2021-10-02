#include "Object.h"

static void objectSetPosition(struct Object* const obj, vec3 pos)
{
    glm_vec3_copy(pos, obj->position);
}

static void objectSetAngle(struct Object* const obj, float xAngle, float yAngle)
{
    obj->xAngle = xAngle;
    obj->yAngle = yAngle;
}

static void objectSetVelocity(struct Object* const obj, vec3 value)
{
    glm_vec3_copy(value, obj->velocity);
}

static void objectMove(struct Object* const obj, ObjectMove move, bool x, bool y, bool z)
{
    if (x) {
        if (move == objectMoveForward) obj->velocity[0] = sin(obj->xAngle) * obj->speed;
        if (move == objectMoveBackward) obj->velocity[0] = -sin(obj->xAngle) * obj->speed;
        if (move == objectMoveRight) obj->velocity[0] = sin(obj->xAngle + glm_rad(-90)) * obj->speed;
        if (move == objectMoveLeft) obj->velocity[0] = sin(obj->xAngle + glm_rad(90)) * obj->speed;
    }

    if (y) {
        if (move == objectMoveForward) obj->velocity[1] = tan(obj->yAngle) * obj->speed;
        if (move == objectMoveBackward) obj->velocity[1] = -tan(obj->yAngle) * obj->speed;
    }

    if (z) {
        if (move == objectMoveForward) obj->velocity[2] = cos(obj->xAngle) * obj->speed;
        if (move == objectMoveBackward) obj->velocity[2] = -cos(obj->xAngle) * obj->speed;
        if (move == objectMoveRight) obj->velocity[2] = cos(obj->xAngle + glm_rad(-90)) * obj->speed;
        if (move == objectMoveLeft) obj->velocity[2] = cos(obj->xAngle + glm_rad(90)) * obj->speed;
    }
}

static void objectInit(struct Object* const obj, vec3 pos, vec3 size, float xAngle, float yAngle, float speed)
{
    objectSetPosition(obj, pos);
    objectSetAngle(obj, xAngle, yAngle);
    objectSetVelocity(obj, (float[]){0.0f, 0.0f, 0.0f});
    glm_vec3_copy(size, obj->size);
    obj->speed = speed;
    obj->isOnGround = false;
}

static void objectUpdate(struct Object* const obj,
                         float time,
                         bool (*const objectWorldIsColision)(vec3 position, vec3 size))
{
    obj->isOnGround = false;

    obj->position[0] += obj->velocity[0] * time;
    if (objectWorldIsColision(obj->position, obj->size)) {
        obj->position[0] -= obj->velocity[0] * time;
    }

    obj->position[1] += obj->velocity[1] * time;
    if (objectWorldIsColision(obj->position, obj->size)) {
        obj->position[1] -= obj->velocity[1] * time;
        obj->velocity[1] = 0.0f;
        obj->isOnGround = true;
    }

    obj->position[2] += obj->velocity[2] * time;
    if (objectWorldIsColision(obj->position, obj->size)) {
        obj->position[2] -= obj->velocity[2] * time;
    }

    obj->velocity[0] = obj->velocity[2] = 0.0f;
}

static void objectDraw(struct Object* const obj, void (*const voxelDraw)(float x, float y, float z))
{
    for (int z = obj->position[2]; z < (obj->position[2] + obj->size[2]); ++z) {
        for (int y = obj->position[1]; y < (obj->position[1] + obj->size[1]); ++y) {
            for (int x = obj->position[0]; x < (obj->position[0] + obj->size[0]); ++x) {
                voxelDraw(x, y, z);
            }
        }
    }
}

static void objectVertMove(struct Object* const obj, float gravity)
{
    obj->velocity[1] += gravity;
}

static bool objectIsColision(struct Object* const obj, float x, float y, float z, float width, float height, float depth) {
    return isColision(obj->position[0], obj->position[1], obj->position[2], obj->size[0], obj->size[1], obj->size[2],
                      x, y, z, width, height, depth);
}

void objectGetPosition(void* const value, float** position) {
    Object* o = value;
    (*position) = o->position;
}

void objectGetVelocity(void* const value, float** velocity) {
    Object* o = value;
    (*velocity) = o->velocity;
}

void objectGetSize(void* const value, float** size) {
    Object* o = value;
    (*size) = o->size;
}

void objectGetSpeed(void* const value, float** speed) {
    Object* o = value;
    (*speed) = &(o->speed);
}

void objectGetIsOnGround(void* const value, bool** isOnGround) {
    Object* o = value;
    (*isOnGround) = &(o->isOnGround);
}

struct ObjectManager objectManagerInit()
{
    struct ObjectManager manager;
    manager.draw = &objectDraw;
    manager.init = &objectInit;
    manager.setAngle = &objectSetAngle;
    manager.setPosition = &objectSetPosition;
    manager.setVelocity = &objectSetVelocity;
    manager.update = &objectUpdate;
    manager.move = &objectMove;
    manager.vertMove = &objectVertMove;
    manager.isColision = &objectIsColision;
    return manager;
};
