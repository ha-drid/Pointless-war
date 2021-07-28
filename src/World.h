#ifndef WORLD_HEADER_FILE
#define WORLD_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint-gcc.h>

#include "GL/CameraGL.h"

#include "Game/World/VoxelWorld.h"
#include "Game/Entity.h"

struct World
{
    struct CameraGL camera;
    struct VoxelWorld voxelWorld;
};


#endif // WORLD_HEADER_FILE
