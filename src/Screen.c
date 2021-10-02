#include "Screen.h"

static void screenInit(Screen* const sc,
                    unsigned width,
                    unsigned height,
                    const char* vertShader,
                    const char* fragShader) {
    sc->width = width;
    sc->height = height;
    sc->data = calloc(sizeof(struct RGBA), width * height);

    sc->shaderManager = programManagerInit();
    sc->vaoManager = vertexArrayObjectManagerInit();

    sc->shaderManager.init(&(sc->shader), vertShader, fragShader);
    sc->shaderManager.bindAttribute(&(sc->shader), 0, "position");
    sc->shaderManager.link(&(sc->shader));

    vec2 rectPos[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
    };

    unsigned rectIndex[6] = {
        0, 1, 2,
        3, 2, 1,
    };

    sc->vaoManager.init(&(sc->pixel));
    sc->vaoManager.addVertexBufferObject(&(sc->pixel), 2, 8, rectPos);
    sc->vaoManager.addIndices(&(sc->pixel), 6, rectIndex);
}

static void screenClear(Screen* const sc, char r, char g, char b, char a) {
    struct RGBA color = { r, g, b, a };
    for (uint32_t i = 0; i < sc->width * sc->height; ++i) {
        sc->data[i] = color;
    }
}

static void screenSwap(Screen* const sc) {
    sc->shaderManager.use(&(sc->shader));

    mat4 view;
    mat4 model;
    mat4 projection;

    memset(model, 0, sizeof(mat4));
    model[0][0] = 1.0f; model[1][1] = 1.0f; model[2][2] = 1.0f; model[3][3] = 1.0f;

    glm_ortho(0.0f, sc->width, sc->height, 0.0f, 0.0f, 100.0f, projection);
    glm_lookat((vec3){ 0.0f, 0.0f, 1.0f }, // camera pos
               (vec3){ 0.0f, 0.0f, -1.0f }, // camera direction
               (vec3){ 0.0f, 1.0f, 0.0f }, //up
               view);

    mat4 mul;
    glm_mat4_mul(projection, view, mul);

    sc->shaderManager.setMat4fv(&(sc->shader), "u_ViewProjection", 1, GL_FALSE, mul);

    size_t iD = 0;
    for (int64_t y = 0; y < sc->height; ++y) {
        for (int64_t x = 0; x < sc->width; ++x) {
            if (sc->data[iD].alpha)
            {
                float r = sc->data[iD].red / 256.0f;
                float g = sc->data[iD].green / 256.0f;
                float b = sc->data[iD].blue / 256.0f;
                float a = sc->data[iD].alpha / 256.0f;
                model[3][0] = x; model[3][1] = y; model[3][2] = 0;

                sc->shaderManager.setVec4f(&(sc->shader), "u_color", r, g, b, a);
                sc->shaderManager.setMat4fv(&(sc->shader), "u_Model", 1, GL_FALSE, model);

                    sc->vaoManager.drawElements(&(sc->pixel), GL_TRIANGLES);

            }
            ++iD;
        }
    }
}

static void screenSet(Screen* const sc, unsigned x, unsigned y, char r, char g, char b, char a)
{
    if ((x >= sc->width) ||
        (y >= sc->height))
        return 0;

    unsigned iY = y * sc->width;

    sc->data[x + iY].red = r;
    sc->data[x + iY].green = g;
    sc->data[x + iY].blue = b;
    sc->data[x + iY].alpha = a;
}

static void screenRect(Screen* const sc, unsigned x1, unsigned y1, unsigned width, unsigned height, char r, char g, char b, char a)
{
    for (uint32_t y = 0; y < height; ++y)
    {
        if ((y == 0) || (y == height - 1)) {

            for (uint32_t x = 0; x < width; ++x)
                screenSet(sc, x + x1, y + y1, r, g, b, a);

        }
        else {
             screenSet(sc, x1, y + y1, r, g, b, a);
             screenSet(sc, x1 + (width - 1), y + y1, r, g, b, a);
        }
    }
}

void screenImage(Screen* const sc, unsigned x, unsigned y, Image* img)
{
    struct RGBA getColor(unsigned x, unsigned y) {
        struct RGBA color;
        memset(&color, 0, sizeof(color));
        color.alpha = __UINT8_MAX__;

        unsigned iy = y * img->width;
        unsigned volume = (iy + x) * img->channels;

        if (img->channels == 1) {
            color.red = img->data[volume];
        }
        else if (img->channels == 2) {
            color.red = img->data[volume];
            color.green = img->data[volume + 1];
        }
        else if (img->channels == 3) {
            color.red = img->data[volume];
            color.green = img->data[volume + 1];
            color.blue = img->data[volume + 2];
        }
        else if (img->channels == 4) {
            color.red = img->data[volume];
            color.green = img->data[volume + 1];
            color.blue = img->data[volume + 2];
            color.alpha = img->data[volume + 3];
        }

        return color;
    }

    for (uint32_t iy = 0; iy < img->height; ++iy) {
        for (uint32_t ix = 0; ix < img->width; ++ix) {
            screenSet(sc, x + ix, y + iy,
                        getColor(ix, iy).red,
                        getColor(ix, iy).green,
                        getColor(ix, iy).blue,
                          getColor(ix, iy).alpha);
        }
    }
}

static void screenRectFill(Screen* const sc, unsigned x1, unsigned y1, unsigned width, unsigned height, char r, char g, char b, char a)
{
    for (uint32_t y = 0; y < height; ++y) {
        for (uint32_t x = 0; x < width; ++x) {
            screenSet(sc, x + x1, y + y1, r, g, b, a);
        }
    }
}

static void screenDelete(Screen* const sc) {
    free(sc->data);
    sc->vaoManager.delete(&(sc->pixel));

    sc->shaderManager.delete(&(sc->shader));
}

ScreenManager screenManagerInit()
{
    ScreenManager manager;
    manager.init = &screenInit;
    manager.clear = &screenClear;
    manager.swap = &screenSwap;
    manager.set = &screenSet;
    manager.rect = &screenRect;
    manager.rectFill = &screenRectFill;
    manager.image = &screenImage;
    manager.delete = &screenDelete;
    return manager;
}

