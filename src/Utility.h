#ifndef UTILITY_HEADER_FILE
#define UTILITY_HEADER_FILE

#include <glad/glad.h>

#include "main.h"
#include "World.h"

#include "Game/Objects/Object.h"
#include "SDL/Window.h"
#include "GL/Render.h"
#include "GL/Program.h"
#include "Util/matrix.h"
#include "GL/CameraGL.h"
#include "Game/Voxels/Chunk.h"
#include "Game/VoxelWorld.h"
#include "Game/Surmap.h"
#include "MVP.h"
#include "Time.h"

struct GlobalManager
{
    struct WindowGLManager windowGL;
    struct RenderManager render;
    struct ProgramManager program;
    struct CameraGLManager cameraGL;
    struct Vector3fManager vector3f;
    struct VoxelWorldManager voxelWorld;
    struct MVPManager mvp;
    struct ChunkManager chunk;
    struct ChunksManager chunks;
    struct VoxelInstanceManager voxelInstance;
    struct MatrixManager matrix;
    struct SurmapManager surmap;
    struct ObjectManager object;
    struct TimeManager time;
};

struct GlobalManager globalManagerInit();
float voxelGetColor(struct Voxel vox, uint32_t index);


#endif // UTILITY_HEADER_FILE
