#include "VoxelWorld.h"

static void voxelWorldInit(struct VoxelWorld* world,
                    uint32_t width,
                    uint32_t height,
                    uint32_t depth,
                    struct ChunkManager* chunk,
                    struct ChunksManager* chunks,
                    struct VoxelInstanceManager* voxelInstance)
{
    chunks->init(&(world->world), width, height, depth, 8, 8, 8, chunk);

    world->mesh = (struct VoxelInstance*)malloc(sizeof(struct VoxelInstance) * (width * height * depth));
    for (uint32_t i = 0; i < (width * height * depth); ++i)
        voxelInstance->init(&(world->mesh[i]), 8 * 8 * 8);
};

static void voxelWorldUpdate(struct VoxelWorld* world,
                      float (*getVoxelColor)(struct Voxel vox, uint32_t index),
                      struct ChunkManager* chunk,
                      struct ChunksManager* chunks,
                      struct VoxelInstanceManager* voxelInstance)
{
    uint32_t index = 0;
    for (uint32_t z = 0; z < world->world.chunks.countZ; ++z) {
        for (uint32_t y = 0; y < world->world.chunks.countY; ++y) {
            for (uint32_t x = 0; x < world->world.chunks.countX; ++x) {

                voxelInstance->update(&(world->mesh[index]), &(world->world.chunks.data[z][y][x]), 8, 8, 8, getVoxelColor);
                ++index;
            }
        }
    }
}

static void voxelWorldDelete(struct VoxelWorld* world,
                    struct ChunkManager* chunk,
                    struct ChunksManager* chunks,
                    struct VoxelInstanceManager* voxelInstance)
{
    for (uint32_t i = 0; i < (world->world.chunks.countX * world->world.chunks.countY * world->world.chunks.countZ); ++i)
        voxelInstance->delete(&(world->mesh[i]));

    free(world->mesh);

     chunks->delete(&(world->world), chunk);
};

struct VoxelWorldManager voxelWorldManagerInit()
{
    struct VoxelWorldManager manager;

    manager.init = &voxelWorldInit;
    manager.update = &voxelWorldUpdate;
    manager.delete = &voxelWorldDelete;

    return manager;
};
