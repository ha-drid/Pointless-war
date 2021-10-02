#ifndef UTILITY_HEADER_FILE
#define UTILITY_HEADER_FILE

#include <glad/glad.h>

#include "main.h"

#include "Game/Units/Object.h"
#include "Game/Units/Inventory.h"
#include "SDL/Window.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"
#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/VoxelInstance.h"
#include "GL/VertexArrayObject.h"
#include "Time.h"
#include "GL/VertexArray.h"
#include "Objects.h"
#include "MainPlayer.h"
#include "AsciiWorld.h"
#include "Util/Image.h"
#include "GL/Texture.h"
#include "Screen.h"
#include "Items.h"

typedef struct GlobalManager
{
    struct WindowGLManager windowGL;
    struct ProgramManager program;
    struct CameraGLManager cameraGL;
    struct ChunkManager chunk;
    struct VoxelInstanceManager voxelInstance;
    struct ObjectManager object;
    struct TimeManager time;
//    struct InventoryManager inventory;
    struct VertexArrayObjectManager vao;
    struct VertexArrayManager vertexArray;
    struct ObjectsManager objects;
    struct MainPlayerManager mainPlayer;
    struct ScreenManager screen;
    struct AsciiWorldManager asciiWorld;
    struct ImageManager image;
    struct TextureManager texture;
    struct InventoryIconsManager inventoryIcon;
} GlobalManager;

struct GlobalManager globalManagerInit();
float voxelGetColor(struct Voxel vox, uint32_t index);


#endif // UTILITY_HEADER_FILE
