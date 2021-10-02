#ifndef OBJECTS_HEADER_FILE
#define OBJECTS_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <clist/list.h>
#include <cvector/vector.h>

#include "Game/Voxels/Voxel.h"
#include "Game/Units/Object.h"
#include "MainPlayer.h"

// эта структура отвечает за хранение и исползование обьетов
typedef struct Objects
{
    list_t* entitys;
    void (*objectGetPosition)(void* const value,float** const position);
    void (*objectGetVelocity)(void* const value, float** const velocity);
    void (*objectGetSize)(void* const value, float** const size);
    void (*objectGetSpeed)(void* const value, float** const speed);
    void (*objectGetIsOnGround)(void* const value, bool** const isOnGround);
} Objects;

typedef struct ObjectsManager
{
    void (*init)(Objects* const objs, void (*const getPosition)(void* const value,float** const position),
                                        void (*const getVelocity)(void* const value, float** const velocity),
                                        void (*const getSize)(void* const value, float** const size),
                                        void (*const getSpeed)(void* const value, float** const speed),
                                        void (*const getIsOnGround)(void* const value, bool** const isOnGround));

    void (*newObject)(Objects* const objs, void* const value);
    void (*update)(Objects* const objs, float time, AsciiWorld* const world, AsciiWorldManager* const manager);
    list_node_t* (*mainPlayerColision)(Objects* const objs, MainPlayer* const player);
    void (*draw)(Objects* const objs, void (*const objectDraw)(void* const value));
    void (*vertMove)(Objects* const objs, float gravity);
    void (*delete)(Objects* const objs);
} ObjectsManager;

ObjectsManager objectsManagerInit();

#endif // OBJECTS_HEADER_FILE
