#include "Object.h"

#define M_PI    3.14159265358979323846264338327950288

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

static void objectSetPosition(struct Object* obj, float x, float y, float z)
{
    obj->x = x;
    obj->y = y;
    obj->z = z;
}

static void objectSetAngle(struct Object* obj, float xAngle, float yAngle)
{
    obj->xAngle = xAngle;
    obj->yAngle = yAngle;
}

static void objectSetVelocity(struct Object* obj, float x, float y, float z)
{
    obj->xVelocity = x;
    obj->yVelocity = y;
    obj->zVelocity = z;
}

static void objectMove(struct Object* obj, ObjectMove move, bool x, bool y, bool z)
{
    if (x) {
        if (move == objectMoveForward) obj->xVelocity = sin(obj->xAngle) * obj->speed;
        if (move == objectMoveBackward) obj->xVelocity = -sin(obj->xAngle) * obj->speed;
        if (move == objectMoveRight) obj->xVelocity = sin(obj->xAngle + radians_s(-90)) * obj->speed;
        if (move == objectMoveLeft) obj->xVelocity = sin(obj->xAngle + radians_s(90)) * obj->speed;
    }

    if (y) {
        if (move == objectMoveForward) obj->yVelocity = tan(obj->yAngle) * obj->speed;
        if (move == objectMoveBackward) obj->xVelocity = -tan(obj->yAngle) * obj->speed;
    }

    if (z) {
        if (move == objectMoveForward) obj->zVelocity = cos(obj->xAngle) * obj->speed;
        if (move == objectMoveBackward) obj->zVelocity = -cos(obj->xAngle) * obj->speed;
        if (move == objectMoveRight) obj->zVelocity = cos(obj->xAngle + radians_s(-90)) * obj->speed;
        if (move == objectMoveLeft) obj->zVelocity = cos(obj->xAngle + radians_s(90)) * obj->speed;
    }
}

static void objectInit(struct Object* obj,
                       float x, float y, float z,
                       float width, float height, float depth,
                       float xAngle, float yAngle,
                       float speed)
{
    objectSetPosition(obj, x, y, z);
    objectSetAngle(obj, xAngle, yAngle);
    objectSetVelocity(obj, 0.0f, 0.0f, 0.0f);
    obj->speed = speed;
    obj->height = height;
    obj->width = width;
    obj->depth = depth;
    obj->isOnGround = false;
}

static void objectUpdate(struct Object* obj,
                         double time,
                         bool (*objectWorldIsColision)(struct Object* obj))
{
    obj->isOnGround = false;

    obj->x += obj->xVelocity * time;
    if (objectWorldIsColision(obj)) {
        obj->x -= obj->xVelocity * time;
    }

    obj->y += obj->yVelocity * time;
    if (objectWorldIsColision(obj)) {
        obj->y -= obj->yVelocity * time;
        obj->yVelocity = 0.0f;
        obj->isOnGround = true;
    }

    obj->z += obj->zVelocity * time;
    if (objectWorldIsColision(obj)) {
        obj->z -= obj->zVelocity * time;
    }

    obj->zVelocity = obj->xVelocity = 0.0f;
}

static void objectDraw(struct Object* obj, void (*voxelDraw)(float x, float y, float z))
{
    for (int z = obj->z; z < (obj->z + obj->depth); ++z) {
        for (int y = obj->y; y < (obj->y + obj->height); ++y) {
            for (int x = obj->x; x < (obj->x + obj->width); ++x) {
                voxelDraw(x, y, z);
            }
        }
    }
}

static void objectVertMove(struct Object* obj, float gravity)
{
    obj->yVelocity += gravity;
}

static bool objectIsColision(struct Object* obj, float x, float y, float z, float width, float height, float depth) {
    return isColision(obj->x, obj->y, obj->z, obj->width, obj->height, obj->depth,
                      x, y, z, width, height, depth);
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

