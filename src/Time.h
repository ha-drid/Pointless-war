#ifndef TIME_HEADER_FILE
#define TIME_HEADER_FILE

#include <stdint-gcc.h>
#include <stdbool.h>
#include <SDL2/SDL_timer.h>

/**<  Это структура нужна для того чтобы понять сколько кадр длился и сколько fps в секунду*/
struct Time
{
    double nowTime;
    double lastTime;
    double deltaTime;

    uint32_t fps;
};

struct TimeManager
{
    void (*init)(struct Time* t);
    bool (*update)(struct Time* t);
};

struct TimeManager timeManagerInit();

#endif // TIME_HEADER_FILE
