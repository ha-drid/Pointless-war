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

    return manager;
}
