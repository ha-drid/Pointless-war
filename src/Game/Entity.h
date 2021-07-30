#ifndef ENTYTI_HEADER_FILE_INCLUDED
#define ENTYTI_HEADER_FILE_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdarg.h>
#include <colision.h>
#include <math.h>

#include "VoxelWorld.h"

#define ENTITY_FORWARD 0
#define ENTITY_BACKWARD 1
#define ENTITY_LEFT 2
#define ENTITY_RIGHT 3

struct Entity
{
    float x, y, z;
    float width, height, depth;
    float xAngle, yAngle;
    float vertSpeed;
    bool isOnGround;
};

void entityInit(struct Entity* entity,
                float x,
                float y,
                float z,
                float width,
                float height,
                float depth);

void entityVertMove(struct Entity* entity, struct VoxelWorld* world, struct VoxelWorldManager worldManager);
void entityMove(struct Entity* entity, struct VoxelWorld* world, uint32_t move, float time, float speed);
void entitySetPosition(struct Entity* entity, float x, float y, float z);
void entityColisionInVoxelWorld(struct Entity* entity, struct VoxelWorld* world, float dx, float dy, float dz);
bool entityColision(struct Entity* entity, float x, float y, float z, float width, float height, float depth);
void entityDraw(struct Entity* entity, void (*drawVoxel)(float x, float y, float z));

#endif // ENTYTI_HEADER_FILE_INCLUDED
