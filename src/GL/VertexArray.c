#include "VertexArray.h"

static void vertexArrayInit(struct VertexArray* arrays)
{
    arrays->active = bitset_create();
    bitset_resize(arrays->active, 1, 1);
}

static void vertexArrayPosition(struct VertexArray* arrays, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer)
{
    bitset_set_to_value(arrays->active, 0, 1);
    arrays->data[0] = GL_VERTEX_ARRAY;

    glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(size, type, stride, pointer);
    glDisableClientState(GL_VERTEX_ARRAY);
}

static void vertexArrayDrawArrays(struct VertexArray* arrays, uint32_t type, uint32_t count)
{
    for (size_t i = 0; i < arrays->active->capacity; ++i)
        if (bitset_get(arrays->active, i))
            glEnableClientState(arrays->data[i]);

    glDrawArrays(type, 0, count);

    for (size_t i = 0; i < arrays->active->capacity; ++i)
        if (bitset_get(arrays->active, i))
            glDisableClientState(arrays->data[i]);
}

static void vertexArrayDelete(struct VertexArray* arrays)
{
    bitset_free(arrays->active);
}

struct VertexArrayManager vertexArrayManagerInit()
{
    struct VertexArrayManager manager;
    manager.delete = &vertexArrayDelete;
    manager.position = &vertexArrayPosition;
    manager.drawArrays = &vertexArrayDrawArrays;
    manager.init = &vertexArrayInit;
    return manager;
};

