#include "Objects.h"
#include "Game/Voxels/Voxel.h"
#include "main.h"

static void objectsInit(Objects* const objs, void (*const getPosition)(void* const value,float** const position),
                                        void (*const getVelocity)(void* const value, float** const velocity),
                                        void (*const getSize)(void* const value, float** const size),
                                        void (*const getSpeed)(void* const value, float** const speed),
                                        void (*const getIsOnGround)(void* const value, bool** const isOnGround))
{
    objs->entitys = list_new();
    objs->entitys->free = &free;

    objs->objectGetIsOnGround = getIsOnGround;
    objs->objectGetPosition = getPosition;
    objs->objectGetSize = getSize;
    objs->objectGetSpeed = getSpeed;
    objs->objectGetVelocity = getVelocity;
}

static void objectsNewObject(Objects* const objs, void* const value)
{
    list_rpush(objs->entitys, list_node_new(value));
}

static list_node_t *objectsMainPlayerColision(Objects* const objs, MainPlayer* const player)
{
}

static void objectsUpdate(Objects* const objs, float time, AsciiWorld* const world, AsciiWorldManager* const manager)
{
    bool objectWorldIsColision(float* position, float* size) {
        for (int z = (position[2] - size[2]); z < (position[2] + size[2]); ++z)
        for (int y = (position[1] - size[1]); y < (position[1] + size[1]); ++y)
        for (int x = (position[0] - size[0]); x < (position[0] + size[0]); ++x)
        {
            if (manager->getVoxel(world, x, y, z).iD)
            {
                if (isColision(position[0], position[1], position[2],
                            size[0], size[1], size[2],
                            x, y, z,
                            1, 1, 1))
                {
                    return true;
                }
            }
        }
        return false;
    }

    float * position;
    float * velocity;
    float * size;
    float* speed;
    bool* isOnGround;

    for (list_node_t* i = objs->entitys->head; i != NULL; i = i->next)
    {
        objs->objectGetIsOnGround(i->val, &isOnGround);
        objs->objectGetPosition(i->val, &position);
        objs->objectGetSize(i->val, &size);
        objs->objectGetSpeed(i->val, &speed);
        objs->objectGetVelocity(i->val, &velocity);
        (*isOnGround) = false;

        position[0] += velocity[0] * time * (*speed);
        if (objectWorldIsColision(position, size)) {
            position[0] -= velocity[0] * time * (*speed);
        }

        position[1] += velocity[1] * time * (*speed);
        if (objectWorldIsColision(position, size)) {
            position[1] -= velocity[1] * time * (*speed);
            velocity[1] = 0.0f;
            (*isOnGround) = true;
        }

        position[2] += velocity[2] * time * (*speed);
        if (objectWorldIsColision(position, size)) {
            position[2] -= velocity[2] * time * (*speed);
        }

        velocity[0] = velocity[2] = 0.0f;
    }
}

static void objectsDraw(Objects* const objs, void (*const objectDraw)(void* const value))
{
    for (list_node_t* i = objs->entitys->head; i != NULL; i = i->next)
        objectDraw(i->val);
}

static void objectsVertMove(Objects* const objs, float gravity)
{
    float* velocity;
    for (list_node_t* i = objs->entitys->head; i != NULL; i = i->next)
    {
        objs->objectGetVelocity(i->val, &velocity);
        velocity[1] += gravity;
    }
}

static void objectsDelete(Objects* const objs)
{
    list_destroy(objs->entitys);
}

ObjectsManager objectsManagerInit()
{
    ObjectsManager manager;

    manager.delete = &objectsDelete;
    manager.init = &objectsInit;
    manager.update = &objectsUpdate;
    manager.vertMove = &objectsVertMove;
    manager.draw = &objectsDraw;
    manager.mainPlayerColision = &objectsMainPlayerColision;
    manager.newObject = &objectsNewObject;

    return manager;
}
