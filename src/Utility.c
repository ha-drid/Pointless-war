#include "Utility.h"

/**< Порядок обьявлений этих структур управленцев важен.
    Так как чем ниже структура обьявлена чем позже она добавлена.
    Так можно понимать как развивался проект.
    */
struct GlobalManager globalManagerInit()
{
    struct GlobalManager manager;
    manager.windowGL = windowGLManagerInit();
    manager.render = renderManagerInit();
    manager.program = programManagerInit();
    manager.cameraGL = cameraGlManagerInit();
    manager.vector3f = vector3fManagerInit();
    manager.chunk = chunkManagerInit();
    manager.voxelInstance = voxelInstanceManagerInit();
    manager.chunks = chunksManagerInit();
    manager.matrix = matrixManagerInit();
    manager.voxelWorld = voxelWorldManagerInit();
    manager.mvp = modelViewProjectionMatrixManagerInit();
    manager.surmap = surmapManagerInit();
    manager.object = objectManagerInit();
    manager.time = timeManagerInit();
    manager.inventory = inventoryManagerInit();
    manager.vao = vertexArrayObjectManagerInit();

    return manager;
}

float voxelGetColor(struct Voxel vox, uint32_t index)
{
    return VoxelColor[vox.iD][index];
}
