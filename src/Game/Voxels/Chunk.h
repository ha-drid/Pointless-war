#ifndef CHUNK_HEADER_FILE_INCLUDE
#define CHUNK_HEADER_FILE_INCLUDE

#include <float.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Voxel.h"

typedef struct Chunk
{
    struct Voxel* voxels;
    uint8_t width, height, depth;
} Chunk;

typedef struct ChunkManager
{
    void (*init)(struct Chunk* const chunk, uint32_t widthChunk, uint32_t heightChunk, uint32_t depthChunk);
    void (*fill)(struct Chunk* const chunk, struct Voxel voxel);
    void (*randFill)(struct Chunk* const chunk, uint32_t size);
    void (*setVoxel)(struct Chunk* const chunk,
                     uint32_t x,
                     uint32_t y,
                     uint32_t z,
                     struct Voxel vox);
    Voxel (*getVoxel)(struct Chunk* const chunk, int x, int y, int z);
    void (*delete)(struct Chunk* const chunk);
} ChunkManager;

struct ChunkManager chunkManagerInit();

#endif // CHUNK_HEADER_FILE_INCLUDE
