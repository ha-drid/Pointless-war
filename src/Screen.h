#ifndef SCREEN_HEADER_FILE
#define SCREEN_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <cvector/vector.h>
#include <cglm/cglm.h>

#include "GL/Program.h"
#include "GL/VertexArrayObject.h"
#include "GL/Texture.h"
#include "Util/Image.h"

struct RGBA {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;
};

typedef struct Screen
{
    VertexArrayObject pixel;
    Program shader;
    ProgramManager shaderManager;
    VertexArrayObjectManager vaoManager;

    unsigned width, height;
    struct RGBA* data;
} Screen;

typedef struct ScreenManager
{
    void (*init)(Screen* const sc,
                    unsigned width,
                    unsigned height,
                    const char* vertShader,
                    const char* fragShader);
    void (*clear)(Screen* const sc, char r, char g, char b, char a);
    void (*set)(Screen* const sc, unsigned x, unsigned y, char r, char g, char b, char a);
    void (*rect)(Screen* const sc, unsigned x1, unsigned y1, unsigned width, unsigned height, char r, char g, char b, char a);
    void (*rectFill)(Screen* const sc, unsigned x1, unsigned y1, unsigned width, unsigned height, char r, char g, char b, char a);
    void (*image)(Screen* const sc, unsigned x, unsigned y, Image* img);
    void (*swap)(Screen* const sc);
    void (*delete)(Screen* const sc);
} ScreenManager;

ScreenManager screenManagerInit();

#endif // SCREEN_HEADER_FILE
