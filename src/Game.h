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

#include "Game/World/Chunk.h"
#include "Game/World/Chunks.h"
#include "Game/World/VoxelInstance.h"
#include "Game/World/VoxelWorld.h"

#include "Game/Entity.h"

struct Game
{
    struct Program shader;
    struct Render render;
    struct VoxelWorld world;
    struct CameraGL camera;
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

void gameDraw(struct Game* game,
              unsigned* indeces,
              uint32_t indeces_size,
              const char* positionInstance,
              const char* colorInstance,
              const char* translate,
              struct GlobalManager* manager);
void gameDelete(struct Game* game, struct GlobalManager* manager);

#endif // GAME_HEADER_FILE
