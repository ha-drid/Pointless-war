#include "Render.h"

void renderInit(struct Render* render)
{
    render->attributs.size = 0;
    render->attributs.capacity = 2;

    render->attributs.data =  (uint32_t*)malloc(sizeof(int) * render->attributs.capacity);
}

void addAttribute(struct Render* render,
                    Program* program,
                         struct ProgramManager* prog_manager,
                         const char* attrib_name,
                         GLint size,
                         GLboolean normalized,
                         GLsizei stride,
                         const float* array)
{
    GLint attribut = prog_manager->getAttribLocation(program, attrib_name);
    if (attribut == -1)
    {
        printf("Attribute \"%s\" not found\n", attrib_name);
    }
    else
    {
        dynamicArray__push_back(int*,
                                render->attributs.data,
                                render->attributs.capacity,
                                render->attributs.size,
                                attribut);

        glVertexAttribPointer(attribut, size, GL_FLOAT, normalized, stride, array);
    }
}

void draw(struct Render* render, GLenum mode, GLsizei count, GLenum type, const void *indices)
{
    for (uint32_t i = 0; i < render->attributs.size; ++i)
        glEnableVertexAttribArray(render->attributs.data[i]);

    glDrawElements(mode, count, type, indices);

    for (uint32_t i = 0; i < render->attributs.size; ++i)
        glDisableVertexAttribArray(render->attributs.data[i]);
}

void drawInstanced(struct Render* render, GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount)
{
    for (uint32_t i = 0; i < render->attributs.size; ++i)
        glEnableVertexAttribArray(render->attributs.data[i]);

    glDrawElementsInstanced(mode, count, type, indices, instancecount);

    for (uint32_t i = 0; i < render->attributs.size; ++i)
        glDisableVertexAttribArray(render->attributs.data[i]);
}

void deleteRender(struct Render* render)
{
    free(render->attributs.data);
    render->attributs.capacity = 0;
    render->attributs.size = 0;
}

struct RenderManager renderManagerInit()
{
    struct RenderManager manager;
    manager.addAttribute = &addAttribute;
    manager.draw = &draw;
    manager.drawInstanced = &drawInstanced;
    manager.init = &renderInit;
    manager.delete = &deleteRender;

    return manager;
}
