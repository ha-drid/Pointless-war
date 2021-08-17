#include "Time.h"

static void timeInit(struct Time* t)
{
    t->lastTime = 0.0;
    t->nowTime = 0.0;
    t->deltaTime = 0.0;
    t->fps = 0;
}

static int timeUpdate(struct Time* t)
{
    t->nowTime = SDL_GetTicks();
    t->deltaTime = (t->nowTime - t->lastTime) * 0.001;

    if (t->nowTime > t->lastTime + 1000) {
        //printf("fps: %u\n", t->fps);
        int fps = t->fps;
        t->fps = 0;
        t->lastTime = t->nowTime;
        return fps;
    }

    ++t->fps;
    return 0;
}

struct TimeManager timeManagerInit()
{
    struct TimeManager manager;

    manager.init = &timeInit;
    manager.update = &timeUpdate;

    return manager;
};
