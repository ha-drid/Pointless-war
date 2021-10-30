#ifndef CANVAS_HEADER_FILE
#define CANVAS_HEADER_FILE

#include <stdint.h>
#include <cglm/cglm.h>

#include "GL/Texture.h"
#include "GL/Program.h"
#include "GL/VertexArrayObject.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Util/Image.h"

typedef struct Canvas {
    VertexArrayObjectManager vaoManager;
    ProgramManager programManager;
    TextureManager textureManager;

    Texture texture;
    VertexArrayObject model;
    Program program;
} Canvas;

typedef struct CanvasManager {
    void (*init)(Canvas* canvas,
            int32_t size, vec3 position[size], vec2 uv[size],
            int32_t indexSize, uint32_t index[indexSize],
            const char* img_path,
            const char* vert, const char* frag);
    void (*perspective)(Canvas* canvas, float fovy, float aspect, float nearZ, float farZ);
    void (*ortho)(Canvas* canvas, float left,    float right,
                        float bottom,  float top,
                        float nearZ,   float farZ);
    void (*projection)(Canvas* canvas, mat4 proj);
    void (*lookAt)(Canvas* canvas, float positionX, float positionY, float positionZ,
                        float directionX, float directionY, float directionZ,
                        float upX, float upY, float upZ);
    void (*draw)(Canvas* canvas, float x, float y, float z, float uv_x, float uv_y);
    void (*drawInstanced)(Canvas* canvas, float x, float y, float z, VoxelInstance* const mesh);
    void (*delete)(Canvas* canvas);
} CanvasManager;

CanvasManager canvasManagerInit();

#endif // CANVAS_HEADER_FILE
