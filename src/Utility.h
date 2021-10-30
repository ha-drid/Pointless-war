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
#include "Objects.h"
#include "MainPlayer.h"
#include "AsciiWorld.h"
#include "Util/Image.h"
#include "GL/Texture.h"
#include "Items.h"
#include "Gui/Menu.h"
#include "Canvas.h"

typedef struct GlobalManager
{
    struct WindowGLManager windowGL;
    struct ProgramManager program;
    struct CameraGLManager cameraGL;
    struct ChunkManager chunk;
    struct VoxelInstanceManager voxelInstance;
    struct ObjectManager object;
    struct TimeManager time;
    struct VertexArrayObjectManager vao;
    struct ObjectsManager objects;
    struct MainPlayerManager mainPlayer;
    struct AsciiWorldManager asciiWorld;
    struct ImageManager image;
    struct TextureManager texture;
    struct CanvasManager canvas;
    struct ButtonManager button;
    struct MenuManager menu;
} GlobalManager;

struct GlobalManager globalManagerInit();
float voxelGetColor(struct Voxel vox, uint32_t index);


#endif // UTILITY_HEADER_FILE
