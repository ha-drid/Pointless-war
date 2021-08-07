#include "VoxelInstance.h"

static void voxelInstanceInit(struct VoxelInstance* mesh, uint32_t max_voxel_size)
{
    mesh->voxel_size = 0;

    uint32_t capacity = max_voxel_size * 3;
    mesh->colorInstances = (float*)malloc(sizeof(float) * capacity);
    mesh->positionInstances = (float*)malloc(sizeof(float) * capacity);
}

static void voxelInstanceUpdate(struct VoxelInstance* mesh,
                         struct Chunk* chunk,
                         uint32_t chunk_width,
                         uint32_t chunk_height,
                         uint32_t chunk_depth,
                         float (*getVoxelColor)(struct Voxel vox, uint32_t index))
{
    uint32_t index = 0;
    for (uint32_t iZ = 0; iZ < chunk_depth; ++iZ)
    {
        for (uint32_t iY = 0; iY < chunk_height; ++iY)
        {
            for (uint32_t iX = 0; iX < chunk_width; ++iX)
            {
                if (chunk->voxels[index].iD != 0)
                {
                    mesh->positionInstances[(index * 3) + 0] = iX;
                    mesh->positionInstances[(index * 3) + 1] = iY;
                    mesh->positionInstances[(index * 3) + 2] = iZ;

                    mesh->colorInstances[(index * 3) + 0] = getVoxelColor(chunk->voxels[index], 0); // red
                    mesh->colorInstances[(index * 3) + 1] = getVoxelColor(chunk->voxels[index], 1); // green
                    mesh->colorInstances[(index * 3) + 2] = getVoxelColor(chunk->voxels[index], 2); // blue

                    ++index;
                }
            }
        }
    }
    mesh->voxel_size = index;
}

static void voxelInstanceDelete(struct VoxelInstance* mesh)
{
    mesh->voxel_size = 0;
    free(mesh->positionInstances);
    free(mesh->colorInstances);
}

struct VoxelInstanceManager voxelInstanceManagerInit()
{
    struct VoxelInstanceManager manager;
    manager.delete = &voxelInstanceDelete;
    manager.init = &voxelInstanceInit;
    manager.update = &voxelInstanceUpdate;
    return manager;
};
