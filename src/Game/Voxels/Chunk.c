#include "Chunk.h"

void chunkInit(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth)
{
    chunk->voxels = (struct Voxel*)malloc(sizeof(struct Voxel) * (width * height * depth));
}

void chunkFill(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth, struct Voxel voxel)
{
    for (uint32_t i = 0; i < (width * height * depth); ++i)
        chunk->voxels[i] = voxel;
}

void chunkRandFill(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth, uint32_t size)
{
    for (uint32_t i = 0; i < (width * height * depth); ++i)
        chunk->voxels[i].iD = rand() % size + 1;
}

void chunkSetVoxel(struct Chunk* chunk,
                     uint32_t x,
                     uint32_t y,
                     uint32_t z,
                     uint32_t width,
                     uint32_t height,
                     uint32_t depht,
                     struct Voxel vox)
{
    uint32_t iZ = z * (width * height);
    uint32_t iY = y * width;
    chunk->voxels[x + iY + iZ] = vox;
}

void chunkDelete(struct Chunk* chunk)
{
    free(chunk->voxels);
}

struct ChunkManager chunkManagerInit()
{
    struct ChunkManager manager;
    manager.delete = &chunkDelete;
    manager.fill = &chunkFill;
    manager.init = &chunkInit;
    manager.randFill = &chunkRandFill;
    manager.setVoxel = &chunkSetVoxel;

    return manager;
}
