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
    void (*init)(struct Chunk* chunk, uint32_t widthChunk, uint32_t heightChunk, uint32_t depthChunk);
    void (*fill)(struct Chunk* chunk, uint32_t widthChunk, uint32_t heightChunk, uint32_t depthChunk, struct Voxel voxel);
    void (*randFill)(struct Chunk* chunk, uint32_t widthChunk, uint32_t heightChunk, uint32_t depthChunk, uint32_t size);
    void (*setVoxel)(struct Chunk* chunk,
                     uint32_t x,
                     uint32_t y,
                     uint32_t z,
                     uint32_t widthChunk,
                     uint32_t heightChunk,
                     uint32_t depthChunk,
                     struct Voxel vox);
    void (*delete)(struct Chunk* chunk);
};

struct ChunkManager chunkManagerInit();

#endif // CHUNK_HEADER_FILE_INCLUDE
