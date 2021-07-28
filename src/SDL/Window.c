#include "Window.h"
void WindowGLinit(WindowGL* windowGL, const char* name, int width, int height, unsigned sdl_flags)
{
    windowGL->window = SDL_CreateWindow(name,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   width,
                                   height,
                                   sdl_flags);

    windowGL->contex = SDL_GL_CreateContext(windowGL->window);
}

void WindowGLdelete(WindowGL* windowGL)
{
    SDL_GL_DeleteContext(windowGL->contex);
    SDL_DestroyWindow(windowGL->window);
}

void WindowGLswap(WindowGL windowGL)
{
    SDL_GL_SwapWindow(windowGL.window);
}


WindowGLManager windowGLManagerInit()
{
    struct WindowGLManager manager;
    manager.init = &WindowGLinit;
    manager.delete = &WindowGLdelete;
    manager.swap = &WindowGLswap;
    return manager;
}
