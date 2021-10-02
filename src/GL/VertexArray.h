#ifndef VERTEX_ARRAY_HEADER_FILE_INCLUDED
#define VERTEX_ARRAY_HEADER_FILE_INCLUDED

#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <cbitset/bitset.h>

/// use opengl 1.1v
struct VertexArray
{
    size_t data[6];
    bitset_t* active;
};

struct VertexArrayManager
{
    void (*init)(struct VertexArray* arrays);
    void (*position)(struct VertexArray* arrays, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer);
    void (*drawArrays)(struct VertexArray* arrays, uint32_t type, uint32_t count);
    void (*delete)(struct VertexArray* arrays);
};

struct VertexArrayManager vertexArrayManagerInit();

#endif // VERTEX_ARRAY_HEADER_FILE_INCLUDED
