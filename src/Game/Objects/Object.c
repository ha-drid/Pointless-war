#include "Object.h"

static void objectSetPosition(struct Object* obj, float x, float y, float z)
{
    obj->x = x;
    obj->y = y;
    obj->z = z;
}

static void objectInit(struct Object* obj, float x, float y, float z, float width, float height, float depth)
{
    objectSetPosition(obj, x, y, z);
    obj->height = height;
    obj->width = width;
    obj->depth = depth;
    obj->xVelocity = 0.0f;
    obj->yVelocity = 0.0f;
    obj->zVelocity = 0.0f;
}

static void objectUpdate(struct Object* obj, float time)
{
    obj->x += obj->xVelocity * time;
    obj->y += obj->yVelocity * time;
    obj->z += obj->zVelocity * time;
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

struct ObjectManager objectManagerInit()
{
    struct ObjectManager manager;
    manager.draw = &objectDraw;
    manager.init = &objectInit;
    manager.setPosition = &objectSetPosition;
    manager.update = &objectUpdate;
    return manager;
};

