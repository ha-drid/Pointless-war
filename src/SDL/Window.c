#include "Window.h"

static void WindowGLinit(WindowGL* const windowGL, const char* name, int width, int height, unsigned sdl_flags)
{
    windowGL->window = SDL_CreateWindow(name,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   width,
                                   height,
                                   sdl_flags | SDL_WINDOW_OPENGL);

    windowGL->contex = SDL_GL_CreateContext(windowGL->window);
}

static void WindowGLdelete(WindowGL* const windowGL)
{
    SDL_GL_DeleteContext(windowGL->contex);
    SDL_DestroyWindow(windowGL->window);
}

static void WindowGLswap(WindowGL* const windowGL)
{
    SDL_GL_SwapWindow(windowGL->window);
}

static void WindowGLsetWindowInputFocus(WindowGL* const win)
{
    SDL_SetWindowInputFocus(win->window);
}

static bool WindowGLisFocused(WindowGL* const win)
{
    return SDL_GetGrabbedWindow != win->window;
}

WindowGLManager windowGLManagerInit()
{
    struct WindowGLManager manager;
    manager.init = &WindowGLinit;
    manager.delete = &WindowGLdelete;
    manager.swap = &WindowGLswap;
    manager.isFocused = &WindowGLisFocused;
    manager.setWindowInputFocus = &WindowGLsetWindowInputFocus;
    return manager;
}
