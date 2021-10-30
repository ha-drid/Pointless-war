#ifndef GAME_HEADER_FILE
#define GAME_HEADER_FILE

#include <windows.h>

#include "Game/Units/Item.h"
#include "Utility.h"
#include "Time.h"
#include "SDL/Window.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"
#include "Game/Units/Object.h"
#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Gui/Menu.h"
#include "Objects.h"
#include "Items.h"
#include "Canvas.h"

struct Game
{
    struct MainPlayer player;
    struct Time ltime;
    struct Canvas gameCanvas;
};

void gameInit(struct Game* game, struct GlobalManager* manager);
void gameLoop(struct Game* game, struct WindowGL* const win, struct GlobalManager* manager);
void gameDelete(struct Game* game, struct GlobalManager* manager);

static inline Object* createObject(float x, float y, float z, float speed, float w, float h, float d)
{
    Object* new_o = calloc(sizeof(Object), 1);

    glm_vec3_copy((vec3){ x, y, z }, new_o->position);
    glm_vec3_copy((vec3){ w, h, d }, new_o->size);
    new_o->speed = speed;

    return new_o;
}

#endif // GAME_HEADER_FILE
