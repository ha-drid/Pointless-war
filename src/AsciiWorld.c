#include "AsciiWorld.h"
#include <cbitset/bitset.h>

static void asciiWorldInit(AsciiWorld* const world,
                           uint32_t w, uint32_t h, uint32_t d)
{
    world->depth = d;
    world->height = h;
    world->width = w;

    world->chunk = chunkManagerInit();
    world->voxelInstance = voxelInstanceManagerInit();

    world->data = (Chunk***)calloc(sizeof(Chunk**), world->depth);

    for (uint32_t z = 0; z < world->depth; ++z) {
        world->data[z] = (Chunk**)calloc(sizeof(Chunk*), world->height);
    }

    for (uint32_t z = 0; z < world->depth; ++z) {
        for (uint32_t y = 0; y < world->height; ++y) {
            world->data[z][y] = (Chunk*)calloc(sizeof(Chunk), world->width);
        }
    }

    world->chunkSize.x = 8;
    world->chunkSize.y = 8;
    world->chunkSize.z = 8;

    for (uint32_t z = 0; z < world->depth; ++z) {
        for (uint32_t y = 0; y < world->height; ++y) {
            for (uint32_t x = 0; x < world->width; ++x) {
                world->chunk.init(&(world->data[z][y][x]), world->chunkSize.x,
                                                       world->chunkSize.y,
                                                       world->chunkSize.z);
            }
        }
    }

    world->mesh = calloc(sizeof(VoxelInstance), w * h * d);
    for (uint32_t i = 0; i < (world->width * world->height * world->depth); ++i)
        world->voxelInstance.init(&(world->mesh[i]), 2);
}

static Voxel asciiWorldGetVoxel(AsciiWorld* const world, int x, int y, int z)
{
    if ((x < 0) || (x >= world->width * world->chunkSize.x) ||
        (y < 0) || (y >= world->height * world->chunkSize.y) ||
        (z < 0) || (z >= world->depth * world->chunkSize.z))
    {
        return (Voxel){ NULL };
    }

    struct { int x, y, z; } chunkPos;

    chunkPos.x = (x - (x % world->chunkSize.x)) / world->chunkSize.x;
    chunkPos.y = (y - (y % world->chunkSize.y)) / world->chunkSize.y;
    chunkPos.z = (z - (z % world->chunkSize.z)) / world->chunkSize.z;

    int ix = x % world->chunkSize.x;
    int iy = y % world->chunkSize.y;
    int iz = z % world->chunkSize.z;

    return world->chunk.getVoxel(&(world->data[chunkPos.z][chunkPos.y][chunkPos.x]), ix, iy, iz);
}

static void asciiWorldUpdate(AsciiWorld* const world)
{
    bool isViewVoxel(int x, int y, int z) {
        if ((asciiWorldGetVoxel(world, x, y, z).iD == NULL))
            return false;
        else if (
            (asciiWorldGetVoxel(world, x + 1, y, z).iD == NULL) ||
            (asciiWorldGetVoxel(world, x - 1, y, z).iD == NULL) ||
            (asciiWorldGetVoxel(world, x, y + 1, z).iD == NULL) ||
            (asciiWorldGetVoxel(world, x, y - 1, z).iD == NULL) ||
            (asciiWorldGetVoxel(world, x, y, z + 1).iD == NULL) ||
            (asciiWorldGetVoxel(world, x, y, z - 1).iD == NULL) )
        {
            return true;
        }
        else
            return false;
    }

    void instanceUpdate(uint32_t* voxelSize, int chunkPos[3], vector(vector(float))* data)
    {
        data->data[0].size = 0;
        data->data[1].size = 0;
        const static float CHARSIZE = 1.0f / 16.0f;
        uint32_t size = 0;
        int w = chunkPos[0] * world->chunkSize.x;
        int h = chunkPos[1] * world->chunkSize.y;
        int d = chunkPos[2] * world->chunkSize.z;
        for (int z = d; z < d + world->chunkSize.z; ++z) {
            for (int y = h; y < h + world->chunkSize.y; ++y) {
                for (int x = w; x < w + world->chunkSize.x; ++x) {
                    if (isViewVoxel(x, y, z)) {

                        vectorPushBack((data->data[0]), (float)x);
                        vectorPushBack((data->data[0]), (float)y);
                        vectorPushBack((data->data[0]), (float)z);

                        int iX = asciiWorldGetVoxel(world, x, y, z).iD & 0b1111;
                        int iY = asciiWorldGetVoxel(world, x, y, z).iD >> 4;

                        vectorPushBack((data->data[1]), iX * CHARSIZE);
                        vectorPushBack((data->data[1]), iY * CHARSIZE);
                        ++size;
                    }
                }
            }
        }
        (*voxelSize) = size;
    }

    uint32_t index = 0;
    for (int z = 0; z < world->depth; ++z) {
        for (int y = 0; y < world->height; ++y) {
            for (int x = 0; x < world->width; ++x) {
                world->voxelInstance.updateBuffers(&(world->mesh[index]), (int[]){x, y, z}, instanceUpdate);
                ++index;
            }
        }
    }
}

static void asciiWorldSetVoxel(AsciiWorld* const world, int x, int y, int z, Voxel value)
{
    if ((x < 0) || (x >= world->width * world->chunkSize.x) ||
        (y < 0) || (y >= world->height * world->chunkSize.y) ||
        (z < 0) || (z >= world->depth * world->chunkSize.z))
    {
        return;
    }

    struct { int x, y, z; } chunkPos;

    chunkPos.x = (x - (x % world->chunkSize.x)) / world->chunkSize.x;
    chunkPos.y = (y - (y % world->chunkSize.y)) / world->chunkSize.y;
    chunkPos.z = (z - (z % world->chunkSize.z)) / world->chunkSize.z;

    int ix = x % world->chunkSize.x;
    int iy = y % world->chunkSize.y;
    int iz = z % world->chunkSize.z;

    world->chunk.setVoxel(&(world->data[chunkPos.z][chunkPos.y][chunkPos.x]), ix, iy, iz, value);
}

static void asciiWorldDraw(AsciiWorld* const world, void (*const drawInstanced)(VoxelInstance* const mesh, float x, float y, float z))
{
    for (int z = 0; z < world->depth; ++z) {
        for (int y = 0; y < world->height; ++y) {
            for (int x = 0; x < world->width; ++x) {
                int iZ = z * (world->height * world->width);
                int iY = y * world->width;
                drawInstanced(&(world->mesh[iZ + iY + x]), 0.0f, 0.0f, 0.0f);
            }
        }
    }
}

static void asciiWorldDelete(AsciiWorld* const world)
{
    for (uint32_t z = 0; z < world->depth; ++z) {
        for (uint32_t y = 0; y < world->height; ++y) {
            for (uint32_t x = 0; x < world->width; ++x) {
                world->chunk.delete(&(world->data[z][y][x]));
            }
        }
    }
    for (uint32_t i = 0; i < (world->width * world->height * world->depth); ++i)
        world->voxelInstance.delete(&(world->mesh[i]));

    for (uint32_t z = 0; z < world->depth; ++z) {
        for (uint32_t y = 0; y < world->height; ++y) {
            free(world->data[z][y]);
        }
    }

    for (uint32_t z = 0; z < world->depth; ++z) {
        free(world->data[z]);
    }

    free(world->data);

    world->depth = 0;
    world->height = 0;
    world->width = 0;
}

AsciiWorldManager asciiWorldManagerInit()
{
    AsciiWorldManager manager;
    manager.draw = &asciiWorldDraw;
    manager.delete = &asciiWorldDelete;
    manager.update = &asciiWorldUpdate;
    manager.getVoxel = &asciiWorldGetVoxel;
    manager.setVoxel = &asciiWorldSetVoxel;
    manager.init = &asciiWorldInit;

    return manager;
}
