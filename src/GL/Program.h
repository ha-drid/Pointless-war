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

typedef struct ProgramManager
{
    void (*init)(Program* const program, const char* vertexShaderFile, const char* fragmentShaderFile);
    void (*link)(Program* const program);

    void (*bindAttribute)(Program* const program, GLuint index, const GLchar *name);
    void (*setFloat)(Program* const program, const char* name, float x);
    void (*setVec2f)(Program* const program, const char* name, float x, float y);
    void (*setVec3f)(Program* const program, const char* name, float x, float y, float z);
    void (*setVec4f)(Program* const program, const char* name, float x, float y, float z, float w);
    void (*setVec2fArray)(Program* const program, const char* name, GLsizei count, const GLfloat* value);
    void (*setVec3fArray)(Program* const program, const char* name, GLsizei count, const GLfloat* value);
    void (*setInt)(Program* const program, const char* name, int x);

    void (*use)(Program* const program);
    void (*setMat4fv)(Program* const program, const char* name, GLsizei count, GLboolean transpose, const GLfloat *value);
    GLint (*getUniformLocation)(Program* const program, const char* name);
    GLint (*getAttribLocation)(Program* const program, const char* name);
    void (*delete)(Program* const program);

} ProgramManager;

ProgramManager programManagerInit();

#endif // PROGRAM_H_INCLUDED
