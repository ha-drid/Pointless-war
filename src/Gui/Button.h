#ifndef BUTTON_HEADER_FILE
#define BUTTON_HEADER_FILE

#include <string.h>
#include <cglm/cglm.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "../GL/Texture.h"
#include "../GL/VertexArrayObject.h"

typedef struct Button
{
    VertexArrayObject vao;
    Texture texture;
    SDL_Rect rect;
} Button;


typedef struct ButtonManager {
    void (*init)(Button* btn, float x, float y, float w, float h,
                    int textureX, int textureY, int textureW, int textureH,
                    const char* image_path);
    void (*draw)(Button* btn);
    void (*delete)(Button* btn);
} ButtonManager;

ButtonManager buttonManagerInit();

#endif // BUTTON_HEADER_FILE
