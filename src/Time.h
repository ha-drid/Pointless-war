#ifndef TIME_HEADER_FILE
#define TIME_HEADER_FILE

#include <stdint-gcc.h>
#include <stdbool.h>
#include <SDL2/SDL_timer.h>

/**<  ��� ��������� ����� ��� ���� ����� ������ ������� ���� ������ � ������� fps � �������*/
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
