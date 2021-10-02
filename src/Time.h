#ifndef TIME_HEADER_FILE
#define TIME_HEADER_FILE

#include <stdint-gcc.h>
#include <stdbool.h>
#include <SDL2/SDL_timer.h>

/**<  ��� ��������� ����� ��� ���� ����� ������ ������� ���� ������ � ������� fps � �������*/
typedef struct Time
{
    uint32_t nowTime;
    uint32_t lastTime;
    uint32_t deltaTime;

    uint32_t fps;
} Time;

struct TimeManager
{
    void (*init)(struct Time* const t);
    void (*update)(struct Time* const t);
    void (*frameEnded)(struct Time* const t);
};

struct TimeManager timeManagerInit();

#endif // TIME_HEADER_FILE
