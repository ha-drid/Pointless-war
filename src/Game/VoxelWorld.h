#ifndef WORLD_HEADER_INCLUDE
#define WORLD_HEADER_INCLUDE

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <malloc.h>

#include "Voxels/Voxel.h"
#include "Voxels/Chunk.h"
#include "Voxels/Chunks.h"
#include "Voxels/VoxelInstance.h"

struct VoxelWorld
{
    struct Chunks world;
    struct VoxelInstance* mesh;
};

struct VoxelWorldManager
{
    void (*init)(struct VoxelWorld* world,
                        uint32_t width,
                        uint32_t height,
                        uint32_t depth,
                        struct ChunkManager* chunk,
                        struct ChunksManager* chunks,
                        struct VoxelInstanceManager* voxelInstance);

    void (*update)(struct VoxelWorld* world,
                        float (*getVoxelColor)(struct Voxel vox, uint32_t index),
                        struct ChunkManager* chunk,
                        struct ChunksManager* chunks,
                        struct VoxelInstanceManager* voxelInstance);

    void (*delete)(struct VoxelWorld* world,
                        struct ChunkManager* chunk,
                        struct ChunksManager* chunks,
                        struct VoxelInstanceManager* voxelInstance);
};

struct VoxelWorldManager voxelWorldManagerInit();


#endif // WORLD_HEADER_INCLUDE
