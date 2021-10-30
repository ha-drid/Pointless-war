#ifndef MAINPLAYER_HEADER_FILE
#define MAINPLAYER_HEADER_FILE

#include "Game/Units/Object.h"
#include "GL/CameraGL.h"
#include "SDL/Window.h"
#include "main.h"
#include "AsciiWorld.h"

typedef struct MainPlayer
{
    CameraGL camera;
    vec3 size;
    bool isOnGround;
    CameraGLManager manager;
} MainPlayer;

typedef struct MainPlayerManager
{
    void (*init)(MainPlayer* const player, vec3 position, vec3 size, float speed);
    void (*vertMove)(MainPlayer* const player, float gravity);
    void (*control)(MainPlayer* const player, WindowGL* const window, bool x, bool y, bool z);
    void (*update)(MainPlayer* player, float time, AsciiWorld* const world, AsciiWorldManager* const manager);
    void (*getDirection)(MainPlayer* player, vec3 dir);
    void (*getView)(MainPlayer* player, vec3 pos, vec3 dir, vec3 up);
} MainPlayerManager;

MainPlayerManager mainPlayerManagerInit();

#endif // MAINPLAYER_HEADER_FILE
