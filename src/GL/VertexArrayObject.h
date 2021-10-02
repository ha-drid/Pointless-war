#ifndef VAO_HEADER_FILE
#define VAO_HEADER_FILE

#include <stdint.h>
#include <glad/glad.h>
#include <assert.h>
#include <cvector/vector.h>

typedef struct VertexArrayObject
{
    vector(GLuint) buffers;

    /**
    * \brief Буфер индексов
    */
    unsigned indicesBuffer;

    /**
    * \brief Количество индексов
    */
    size_t indicesCount;

    GLuint vao;
} VertexArrayObject;

typedef VertexArrayObject VAO;

typedef struct VertexArrayObjectManager
{
    void (*init)(VertexArrayObject* const vao);
    void (*bind)(VertexArrayObject* const vao);
    void (*addVertexBufferObject)(VertexArrayObject* const vao, uint32_t vertexInPoint, size_t arraySize, float* array);
    void (*addIndices)(VertexArrayObject* const vao, size_t array_size, unsigned* array);
    void (*drawElements)(VertexArrayObject* const vao, unsigned type);
    void (*drawElementsInstanced)(VertexArrayObject* const vao, unsigned type, uint32_t instanceCount);
    void (*drawArrays)(VertexArrayObject* const vao, unsigned type, unsigned count);
    void (*delete)(VertexArrayObject* const vao);
} VertexArrayObjectManager;

VertexArrayObjectManager vertexArrayObjectManagerInit();

#endif // VAO_HEADER_FILE
