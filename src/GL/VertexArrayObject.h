#ifndef VAO_HEADER_FILE
#define VAO_HEADER_FILE

#include <stdint.h>
#include <glad/glad.h>
#include <assert.h>

struct VertexArrayObject
{
    struct
    {
        GLuint* data;
        uint32_t size;
    } buffers;

    /**
    * \brief Буфер индексов
    */
    unsigned indicesBuffer;

    /**
    * \brief Количество индексов
    */
    size_t indicesCount;

    GLuint vao;
};

struct VertexArrayObjectManager
{
    void (*init)(struct VertexArrayObject* vao);
    void (*bind)(struct VertexArrayObject* vao);
    void (*addVertexBufferObject)(struct VertexArrayObject* vao, uint32_t vertexInPoint, size_t arraySize, float* array);
    void (*addIndices)(struct VertexArrayObject* vao, size_t array_size, unsigned* array);
    void (*drawElements)(struct VertexArrayObject* vao, unsigned type);
    void (*drawElementsInstanced)(struct VertexArrayObject* vao, unsigned type, uint32_t instanceCount);
    void (*drawArrays)(struct VertexArrayObject* vao, unsigned type, unsigned count);
    void (*delete)(struct VertexArrayObject* vao);
};

struct VertexArrayObjectManager vertexArrayObjectManagerInit();

#endif // VAO_HEADER_FILE
