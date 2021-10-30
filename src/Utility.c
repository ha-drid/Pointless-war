#include "Utility.h"

/**< Порядок обьявлений этих структур управленцев важен.
    Так как чем ниже структура обьявлена чем позже она добавлена.
    Так можно понимать как развивался проект.
    */
struct GlobalManager globalManagerInit()
{
    struct GlobalManager manager;
    manager.windowGL = windowGLManagerInit();
    manager.program = programManagerInit();
    manager.cameraGL = cameraGlManagerInit();
    manager.chunk = chunkManagerInit();
    manager.voxelInstance = voxelInstanceManagerInit();
    manager.object = objectManagerInit();
    manager.time = timeManagerInit();
    manager.vao = vertexArrayObjectManagerInit();
    manager.objects = objectsManagerInit();
    manager.mainPlayer = mainPlayerManagerInit();
    manager.asciiWorld = asciiWorldManagerInit();
    manager.image = imageManagerInit();
    manager.texture = textureManagerInit();
    manager.canvas = canvasManagerInit();
    manager.button = buttonManagerInit();
    manager.menu = menuManagerInit();

    return manager;
}

float voxelGetColor(struct Voxel vox, uint32_t index)
{
    return VoxelColor[vox.iD][index];
}
