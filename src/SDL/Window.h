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
    void (*init)(WindowGL* const win, const char* name, int width, int height, unsigned sdl_flags);
    void (*swap)(WindowGL* const win);
    bool (*isFocused)(WindowGL* const win);
    void (*setWindowInputFocus)(WindowGL* const win);
    void (*delete)(WindowGL* const win);
} WindowGLManager;

WindowGLManager windowGLManagerInit();

#endif // WINDOW_HEADER_FILE
