#include "Button.h"
#include "../Util/Image.h"
#include <stb_image.h>

static void buttonInit(Button* btn, float x, float y, float w, float h,
                             int textureX, int textureY, int textureW, int textureH,
                             const char* image_path)
{
    btn->rect.x = x; btn->rect.y = y;
    btn->rect.w = w; btn->rect.h = h;
    VertexArrayObjectManager vaoManager = vertexArrayObjectManagerInit();
    TextureManager textureMananer = textureManagerInit();

    float uv_x, uv_y, uv_w, uv_h;
    {
        Image img;
        ImageManager imgManager = imageManagerInit();

        stbi_set_flip_vertically_on_load(true);
        imgManager.init(&img, image_path);
        stbi_set_flip_vertically_on_load(false);

        float uv_one_x = 1.0f / img.width;
        float uv_one_y = 1.0f / img.height;
        uv_x = textureX * uv_one_x;
        uv_w = textureW * uv_one_x;
        uv_y = textureY * uv_one_y;
        uv_h = textureH * uv_one_y;

        textureMananer.init(&(btn->texture), GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
        textureMananer.setImage(&(btn->texture), img.width, img.height, img.channels, img.data);

        imgManager.delete(&img);
    }

    vec2 position[4] = { {x, y},
                         {x + w, y},
                         {x, y + h},
                         {x + w, y + h} };
    vec2 uv[4] = {  {uv_x,        uv_y + uv_h},
                    {uv_x + uv_w, uv_y + uv_h},
                    {uv_x,        uv_y       },
                    {uv_x + uv_w, uv_y       } };

    unsigned index[6] = {
        0, 1, 2,
        1, 2, 3
    };

    vaoManager.init(&(btn->vao));
    vaoManager.addVertexBufferObject(&(btn->vao), 2, 2 * 4, position);
    vaoManager.addVertexBufferObject(&(btn->vao), 2, 2 * 4, uv);
    vaoManager.addIndices(&(btn->vao), 6, index);
}

static void buttonDraw(Button* btn)
{
    VertexArrayObjectManager vaoManager = vertexArrayObjectManagerInit();
    TextureManager textureManager = textureManagerInit();

    textureManager.bind(&(btn->texture));
    vaoManager.drawElements(&(btn->vao), GL_TRIANGLES);
}

static void buttonDelete(Button* btn)
{
    TextureManager textureManager = textureManagerInit();
    VertexArrayObjectManager vaoManager = vertexArrayObjectManagerInit();

    textureManager.delete(&(btn->texture));
    vaoManager.delete(&(btn->vao));
}

ButtonManager buttonManagerInit()
{
    ButtonManager manager;
    manager.init = &buttonInit;
    manager.draw = &buttonDraw;
    manager.delete = &buttonDelete;
    return manager;
}
