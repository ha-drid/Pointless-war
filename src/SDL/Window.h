#ifndef WINDOW_HEADER_FILE
#define WINDOW_HEADER_FILE

#include <stdlib.h>
#include <malloc.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <glad/glad.h>

typedef struct WindowGL
{
    SDL_Window* window;
    SDL_GLContext contex;
} WindowGL;

typedef struct WindowGLManager
{
    void (*init)(WindowGL* win, const char* name, int width, int height, unsigned sdl_flags);
    void (*swap)(WindowGL win);
    void (*delete)(WindowGL* win);
} WindowGLManager;

WindowGLManager windowGLManagerInit();

#endif // WINDOW_HEADER_FILE
