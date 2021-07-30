#ifndef GAME_HEADER_FILE
#define GAME_HEADER_FILE

#include "main.h"
#include "Utility.h"
#include "World.h"

#include "SDL/Window.h"

#include "GL/Render.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"

#include "Util/matrix.h"

#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/Chunks.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Game/VoxelWorld.h"
#include "Util/vec3.h"
#include "Game/Entity.h"

struct Game
{
    struct Program shader;
    struct Render render;
    struct VoxelWorld world;
    struct CameraGL camera;
    struct vec3f drawingDistance;
};

void gameProgramInit(struct Game* game, const char* vert, const char* frag, struct GlobalManager* manager);
void gameRenderInit(struct Game* game,
                    const char* attrib_name,
                    GLint size,
                    GLboolean normalized,
                    GLsizei stride,
                    const float* array,
                    struct GlobalManager* manager);
void gameVoxelWorldInit(struct Game* game, uint32_t w, uint32_t h, uint32_t d, struct GlobalManager* manager);

void gameCameraInit(struct Game* game,
                    float x,
                    float y,
                    float z,
                    float xAngle,
                    float yAngle,
                    float speed,
                    struct GlobalManager* manager);
void gameDrawingDistance(struct Game* game, float x_size, float y_size, float z_size);
void gameDraw(struct Game* game,
              unsigned* indeces,
              uint32_t indeces_size,
              struct Matrix* model,
              const char* positionInstance,
              const char* colorInstance,
              const char* model_name,
              struct GlobalManager* manager);
void gameDelete(struct Game* game, struct GlobalManager* manager);

#endif // GAME_HEADER_FILE
