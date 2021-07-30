#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <glad/glad.h>

typedef struct Program
{
    unsigned program;
    unsigned vertexShader;
    unsigned fragmentShader;
} Program;

struct ProgramManager
{
    void (*init)(Program* program, const char* vertexShader, const char* fragmentShader);
    void (*link)(Program* program);

    void (*setFloat)(Program* program, const char* name, float x);
    void (*setVec3f)(Program* program, const char* name, float x, float y, float z);
    void (*setVec3fArray)(struct Program* program, const char* name, GLsizei count, const GLfloat* value);
    void (*setInt)(Program* program, const char* name, int x);

    void (*use)(Program* program);
    void (*setMat4fv)(struct Program* program, const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLint (*getUniformLocation)(Program* program, const char* name);
    GLint (*getAttribLocation)(Program* program, const char* name);
    void (*delete)(Program* program);

};

struct ProgramManager programManagerInit();

#endif // PROGRAM_H_INCLUDED
