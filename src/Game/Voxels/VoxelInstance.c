#include "VoxelInstance.h"

static void voxelInstanceInit(VoxelInstance* const ins, uint32_t bufferSize)
{
    ins->voxelSize = 0;
    vectorInit((ins->buffer), bufferSize);
}

void voxelInstanceUpdateBuffers(VoxelInstance* const ins, int chunkPos[3],
                                                                void (*const instanceUpdate)
                                (uint32_t* voxelSize, int chunkPos[3], vector(vector(float))* data))
{
    instanceUpdate(&(ins->voxelSize), chunkPos, &(ins->buffer));
}

static void voxelInstanceDelete(struct VoxelInstance* const mesh)
{
    mesh->voxelSize = 0;

    for (uint32_t i = 0; i < mesh->buffer.capacity; ++i) {
        vectorDelete(mesh->buffer.data[i])
    }

    vectorDelete(mesh->buffer);
}

VoxelInstanceManager voxelInstanceManagerInit()
{
    struct VoxelInstanceManager manager;
    manager.delete = &voxelInstanceDelete;
    manager.updateBuffers = &voxelInstanceUpdateBuffers;
    manager.init = &voxelInstanceInit;
    return manager;
};
