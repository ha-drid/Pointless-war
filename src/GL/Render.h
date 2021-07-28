#ifndef RENDER_HEADER_FILE
#define RENDER_HEADER_FILE

#include <glad/glad.h>
#include <stdint-gcc.h>
#include <malloc.h>

#include "../Util/dynamic_array.h"
#include "Program.h"

struct Render
{
    struct
    {
        GLuint* data;
        uint32_t size;
        uint32_t capacity;
    } attributs;

};

struct RenderManager
{
    void (*init)(struct Render* render);
    void (*addAttribute)(struct Render* render,
                         Program* program,
                         struct ProgramManager* prog_manager,
                         const char* attrib_name,
                         GLint size,
                         GLboolean normalized,
                         GLsizei stride,
                         const float* array);

    void (*draw)(struct Render* render, GLenum mode, GLsizei count, GLenum type, const void *indices);
    void (*drawInstanced)(struct Render* render, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
    void (*delete)(struct Render* render);
};

struct RenderManager renderManagerInit();

#endif // RENDER_HEADER_FILE
