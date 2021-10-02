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
    void (*perspective)(MainPlayer* const player, float radian, float aspect, float nearZ, float farZ);
    void (*vertMove)(MainPlayer* const player, float gravity);
    void (*control)(MainPlayer* const player, WindowGL* const window, bool x, bool y, bool z);
    void (*update)(MainPlayer* player, float time, AsciiWorld* const world, AsciiWorldManager* const manager);
    void (*apply)(struct MainPlayer* const player,
                  struct Program* const program,
                  const char* u_ProjectionView,
                  struct ProgramManager* const manager);
} MainPlayerManager;

MainPlayerManager mainPlayerManagerInit();

#endif // MAINPLAYER_HEADER_FILE
