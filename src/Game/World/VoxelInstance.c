#include "VoxelInstance.h"

void voxelInstanceInit(struct VoxelInstance* mesh, uint32_t max_voxel_size)
{
    mesh->voxel_size = 0;

    uint32_t capacity = max_voxel_size * 3;
    mesh->colorInstances = (float*)malloc(sizeof(float) * capacity);
    mesh->positionInstances = (float*)malloc(sizeof(float) * capacity);
}

void voxelInstanceUpdate(struct VoxelInstance* mesh,
                         struct Chunk* chunk,
                         uint32_t chunk_width,
                         uint32_t chunk_height,
                         uint32_t chunk_depth,
                         float (*getVoxelColor)(struct Voxel vox, uint32_t index))
{
    uint32_t index = 0;
    for (uint32_t z = 0; z < chunk_depth; ++z)
    {
        for (uint32_t y = 0; y < chunk_height; ++y)
        {
            for (uint32_t x = 0; x < chunk_width; ++x)
            {
                if (chunk->voxels[index].iD != 0)
                {
                    mesh->positionInstances[(index * 3) + 0] = x;
                    mesh->positionInstances[(index * 3) + 1] = y;
                    mesh->positionInstances[(index * 3) + 2] = z;

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

void voxelInstanceDelete(struct VoxelInstance* mesh)
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
