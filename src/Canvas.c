#include "Canvas.h"

static void canvasInit(Canvas* canvas,
                uint32_t size, vec3 position[size], vec2 uv[size],
                uint32_t indexSize, uint32_t index[indexSize],
                const char* img_path,
                const char* vert, const char* frag)
{
    canvas->programManager = programManagerInit();
    canvas->vaoManager = vertexArrayObjectManagerInit();
    canvas->textureManager = textureManagerInit();

    VertexArrayObject* vao = &(canvas->model);
    Texture* texture = &(canvas->texture);
    Program* prog = &(canvas->program);

    canvas->vaoManager.init(vao);
    canvas->vaoManager.addVertexBufferObject(vao, 3, 3 * size, position);
    canvas->vaoManager.addVertexBufferObject(vao, 2, 2 * size, uv);
    canvas->vaoManager.addIndices(vao, indexSize, index);

    canvas->textureManager.init(texture, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    {
        Image img;
        ImageManager manager = imageManagerInit();
            manager.init(&img, img_path);
            canvas->textureManager.setImage(texture, img.width, img.height, img.channels, img.data);
            manager.delete(&img);
    }
    canvas->programManager.init(prog, vert, frag);
    canvas->programManager.link(prog);
}

static void canvasPerspective(Canvas* canvas, float fovy, float aspect, float nearZ, float farZ)
{
    canvas->programManager.use(&(canvas->program));
    mat4 projection;
    glm_perspective(fovy, aspect, nearZ, farZ, projection);
    canvas->programManager.setMat4fv(&(canvas->program), "u_Projection", 1, GL_FALSE, projection);
}

static void canvasOrtho(Canvas* canvas, float left,    float right,
          float bottom,  float top,
          float nearZ,   float farZ)
{
    canvas->programManager.use(&(canvas->program));
    mat4 projection;
    glm_ortho(left, right, bottom, top, nearZ, farZ, projection);
    canvas->programManager.setMat4fv(&(canvas->program), "u_Projection", 1, GL_FALSE, projection);
}

static void canvasProjection(Canvas* canvas, mat4 projection)
{
    canvas->programManager.use(&(canvas->program));
    canvas->programManager.setMat4fv(&(canvas->program), "u_Projection", 1, GL_FALSE, projection);
}

static void canvasLookAt(Canvas* canvas, float positionX, float positionY, float positionZ,
                                  float directionX, float directionY, float directionZ,
                                  float upX, float upY, float upZ)
{
    canvas->programManager.use(&(canvas->program));
    mat4 view;
    vec3 position = { positionX, positionY, positionZ };
    vec3 direction = { directionX, directionY, directionZ };
    vec3 up = { upX, upY, upZ };

    glm_lookat(position, direction, up, view);

    canvas->programManager.setMat4fv(&(canvas->program), "u_View", 1, GL_FALSE, view);
}

static void canvasDrawInstanced(Canvas* canvas, float x, float y, float z, VoxelInstance* const mesh)
{
    VertexArrayObject* vao = &(canvas->model);
    Texture* texture = &(canvas->texture);
    Program* prog = &(canvas->program);

    glActiveTexture(GL_TEXTURE0);
    canvas->programManager.use(prog);
    canvas->textureManager.bind(texture);

    mat4 model = { {1.0f, 0.0f, 0.0f, 0.0f},
                   {0.0f, 1.0f, 0.0f, 0.0f},
                   {0.0f, 0.0f, 1.0f, 0.0f},
                   {x,    y,    z,    1.0f} };

    canvas->programManager.setMat4fv(prog, "u_Model", 1, GL_FALSE, model);

    canvas->programManager.setVec3fArray(prog, "positionInstances", mesh->voxelSize, mesh->buffer.data[0].data);
    canvas->programManager.setVec2fArray(prog, "uvInstances", mesh->voxelSize, mesh->buffer.data[1].data);

    canvas->vaoManager.drawElementsInstanced(vao, GL_TRIANGLES, mesh->voxelSize);
}

static void canvasDraw(Canvas* canvas, float x, float y, float z, float uv_x, float uv_y) {
    VertexArrayObject* vao = &(canvas->model);
    Texture* texture = &(canvas->texture);
    Program* prog = &(canvas->program);

    glActiveTexture(GL_TEXTURE0);
    canvas->programManager.use(prog);
    canvas->textureManager.bind(texture);

    mat4 model = { {1.0f, 0.0f, 0.0f, 0.0f},
                   {0.0f, 1.0f, 0.0f, 0.0f},
                   {0.0f, 0.0f, 1.0f, 0.0f},
                   {x,    y,    z,    1.0f} };

    canvas->programManager.setMat4fv(prog, "u_Model", 1, GL_FALSE, model);

    canvas->programManager.setVec3f(prog, "positionInstances", 0.0f, 0.0f, 0.0f);
    canvas->programManager.setVec2f(prog, "uvInstances", uv_x, uv_y);

    canvas->vaoManager.drawElements(vao, GL_TRIANGLES);
}

static void canvasDelete(Canvas* canvas)
{
    VertexArrayObject* vao = &(canvas->model);
    Texture* texture = &(canvas->texture);
    Program* prog = &(canvas->program);

    canvas->vaoManager.delete(vao);
    canvas->programManager.delete(prog);
    canvas->textureManager.delete(texture);
}

CanvasManager canvasManagerInit()
{
    CanvasManager manager;
    manager.delete = &canvasDelete;
    manager.draw = &canvasDraw;
    manager.drawInstanced = &canvasDrawInstanced;
    manager.init = &canvasInit;
    manager.lookAt = &canvasLookAt;
    manager.ortho = &canvasOrtho;
    manager.perspective = &canvasPerspective;
    manager.projection = &canvasProjection;
    return manager;
}

