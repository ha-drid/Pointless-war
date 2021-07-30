#ifndef CHUNK_HEADER_FILE_INCLUDE
#define CHUNK_HEADER_FILE_INCLUDE

#include <float.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Voxel.h"

struct Chunk
{
    struct Voxel* voxels;
};

struct ChunkManager
{
    void (*init)(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth);
    void (*fill)(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth, struct Voxel voxel);
    void (*delete)(struct Chunk* chunk);
};

struct ChunkManager chunkManagerInit();

#endif // CHUNK_HEADER_FILE_INCLUDE
