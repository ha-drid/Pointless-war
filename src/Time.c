#include "Time.h"

static void timeInit(struct Time* const t)
{
    t->lastTime = 0;
    t->nowTime = 0;
    t->deltaTime = 0;
    t->fps = 0;
}

static void timeUpdate(struct Time* const t)
{
   t->nowTime = SDL_GetTicks();
   t->deltaTime = t->nowTime - t->lastTime;
}

static void timeFrameEnded(struct Time* const t)
{
    t->lastTime = t->nowTime;
}

struct TimeManager timeManagerInit()
{
    struct TimeManager manager;

    manager.init = &timeInit;
    manager.update = &timeUpdate;
    manager.frameEnded = &timeFrameEnded;

    return manager;
};
