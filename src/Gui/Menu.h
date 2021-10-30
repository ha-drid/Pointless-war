#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <cvector/vector.h>
#include "Button.h"
#include "../GL/Program.h"

typedef struct Menu
{
    vector(struct Button) buttons;
    Program program;
    ProgramManager programManager;
    mat4 projection;
} Menu;


typedef struct MenuManager {
    void (*init)(Menu* const menu, const char* vertShader, const char* fragShader);
    void (*addButton)(Menu* const menu, float x, float y, float w, float h,
                        int textureX, int textureY, int textureW, int textureH,
                const char* image_path);
    void (*setWindowSize)(Menu* const menu, int width, int height);
    void (*draw)(Menu* const menu);
    void (*delete)(Menu* const menu);
} MenuManager;

MenuManager menuManagerInit();

#endif // MENU_H_INCLUDED
