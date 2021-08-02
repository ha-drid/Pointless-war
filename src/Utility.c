#include "Utility.h"

struct GlobalManager globalManagerInit()
{
    struct GlobalManager manager;
    manager.cameraGL = cameraGlManagerInit();
    manager.windowGL = windowGLManagerInit();
    manager.render = renderManagerInit();
    manager.vector3f = vector3fManagerInit();
    manager.program = programManagerInit();
    manager.voxelWorld = voxelWorldManagerInit();
    manager.mvp = mvpManagerInit();
    manager.chunk = chunkManagerInit();
    manager.chunks = chunksManagerInit();
    manager.voxelInstance = voxelInstanceManagerInit();
    manager.matrix = matrixManagerInit();

    return manager;
}

float voxelGetColor(struct Voxel vox, uint32_t index)
{
    return VoxelColor[vox.iD][index];
}
