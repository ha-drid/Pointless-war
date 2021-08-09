#ifndef GAME_HEADER_FILE
#define GAME_HEADER_FILE

#include "Utility.h"
#include "Time.h"
#include "MVP.h"
#include "SDL/Window.h"
#include "GL/Render.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"
#include "Util/matrix.h"
#include "Game/Objects/Object.h"
#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/Chunks.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Game/VoxelWorld.h"
#include "Game/Surmap.h"

struct Game
{
    struct CameraGL camera;
    struct Surmap surmap;
    struct Time ltime;
};

void gameInit(struct Game* game, struct GlobalManager* manager);
void gameLoop(struct Game* game, struct WindowGL* win, struct GlobalManager* manager);
void gameDelete(struct Game* game, struct GlobalManager* manager);

#endif // GAME_HEADER_FILE
