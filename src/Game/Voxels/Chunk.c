#include "Chunk.h"

static void chunkInit(struct Chunk* const chunk, uint32_t width, uint32_t height, uint32_t depth)
{
    chunk->voxels = calloc(sizeof(struct Voxel), (width * height * depth));
    chunk->width = width;
    chunk->height = height;
    chunk->depth = depth;
}

static void chunkFill(struct Chunk* const chunk, struct Voxel voxel)
{
    for (uint32_t i = 0; i < (chunk->width * chunk->height * chunk->depth); ++i)
        chunk->voxels[i] = voxel;
}

static struct Voxel chunkGetVoxel(struct Chunk* const chunk, int x, int y, int z)
{
    if ((x < 0) || (x >= chunk->width) ||
	    (y < 0) || (y >= chunk->height) ||
	    (z < 0) || (z >= chunk->depth))
	    return (struct Voxel){ 0 };

    int iZ = z * (chunk->width * chunk->height);
    int iY = y * chunk->width;

    return (chunk->voxels[iZ + iY + x]);
}

static void chunkRandFill(struct Chunk* const chunk, uint32_t size)
{
    for (uint32_t i = 0; i < (chunk->width * chunk->height * chunk->depth); ++i)
        chunk->voxels[i].iD = (uint8_t)(rand() % size) + 1;
}

static void chunkSetVoxel(struct Chunk* const chunk,
                     uint32_t x,
                     uint32_t y,
                     uint32_t z,
                     struct Voxel vox)
{
    uint32_t iZ = z * (chunk->width * chunk->height);
    uint32_t iY = y * chunk->width;
    chunk->voxels[x + iY + iZ] = vox;
}

static void chunkDelete(struct Chunk* const chunk)
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
    manager.getVoxel = &chunkGetVoxel;

    return manager;
}
