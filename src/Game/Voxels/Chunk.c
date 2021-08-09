#include "Chunk.h"

static void chunkInit(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth)
{
    chunk->voxels = (struct Voxel*)malloc(sizeof(struct Voxel) * (width * height * depth));
}

static void chunkFill(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth, struct Voxel voxel)
{
    for (uint32_t i = 0; i < (width * height * depth); ++i)
        chunk->voxels[i] = voxel;
}

static struct Voxel chunkGetVoxel(struct Chunk* chunk, int x, int y, int z, int width, int height, int depth)
{
    if ((x < 0) || (x >= width) ||
	    (y < 0) || (y >= height) ||
	    (z < 0) || (z >= depth))
	    return (struct Voxel){ 0 };

    int iZ = z * (width * height);
    int iY = y * width;

    return (chunk->voxels[iZ + iY + x]);
}

static void chunkRandFill(struct Chunk* chunk, uint32_t width, uint32_t height, uint32_t depth, uint32_t size)
{
    for (uint32_t i = 0; i < (width * height * depth); ++i)
        chunk->voxels[i].iD = rand() % size + 1;
}

static void chunkSetVoxel(struct Chunk* chunk,
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

static void chunkDelete(struct Chunk* chunk)
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
