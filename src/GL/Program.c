#include "Program.h"
#include <assert.h>

// такая фукция уже есть в файле Model3D.c
static inline  char* getFileText(const char* path)
{
   FILE* file = fopen(path, "rb");
   if (file == 0)
   {
       printf("%s \"%s\" %s\n", "File", path, "not found.");
   }

   fseek(file, 0, SEEK_END);
   unsigned file_lentgh = ftell(file);
   char* text = malloc(file_lentgh + 1);
   memset(text, 0, file_lentgh + 1);

   fseek(file, 0, SEEK_SET);
   fread(text, 1, file_lentgh, file);
   fclose(file);

   return text;
}

static unsigned loadShader(const char* path, unsigned shader_type)
{
    char* text = getFileText(path);
    GLuint shader = glCreateShader(shader_type);

    glShaderSource(shader, 1, &text, NULL);
    glCompileShader(shader);
    free(text);

    GLuint ok;
    const GLuint infoLog_length = 1000;
    GLchar infoLog[infoLog_length];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok == 0)
    {
        glGetShaderInfoLog(shader, infoLog_length, NULL, infoLog);
        printf("%s\n", infoLog);
        assert(0);
    }

    return shader;
}

static void programInit(Program* const program, const char* vertexShader, const char* fragmentShader)
{
    program->vertexShader = loadShader(vertexShader, GL_VERTEX_SHADER);
    program->fragmentShader = loadShader(fragmentShader, GL_FRAGMENT_SHADER);

    program->program = glCreateProgram();
}

static void programLink(Program* const program)
{
    glAttachShader(program->program, program->vertexShader);
    glAttachShader(program->program, program->fragmentShader);

    glLinkProgram(program->program);

    unsigned ok;
    const unsigned infoLog_length = 1000;
    char infoLog[infoLog_length];

    glGetProgramiv(program->program, GL_LINK_STATUS, &ok);
    if (ok == 0)
    {
        glGetProgramInfoLog(program->program, infoLog_length, NULL, infoLog);
        printf("%s\n", infoLog);
        assert(0);
    }
}

static void programDelete(Program* const program)
{
    glDetachShader(program->program, program->vertexShader);
    glDetachShader(program->program, program->fragmentShader);

    glDeleteShader(program->vertexShader);
    glDeleteShader(program->fragmentShader);

    glDeleteProgram(program->program);
}

static GLint programGetAttribLocation(Program* const program, const char* name)
{
    return glGetAttribLocation(program->program, name);
}

static GLint programGetUniformLocation(Program* const program, const char* name)
{
    return glGetUniformLocation(program->program, name);
}

static void programSetFloat(Program* const program, const char* name, float x)
{
    glUniform1f(programGetUniformLocation(program, name), x);
}

static void programSetVec2f(Program* const program, const char* name, float x, float y)
{
    glUniform2f(programGetUniformLocation(program, name), x, y);
}


static void programSetVec3f(Program* const program, const char* name, float x, float y, float z)
{
    glUniform3f(programGetUniformLocation(program, name), x, y, z);
}

static void programSetVec4f(Program* const program, const char* name, float x, float y, float z, float w)
{
    glUniform4f(programGetUniformLocation(program, name), x, y, z, w);
}

static void programSetInt(Program* const program, const char* name, int x)
{
    glUniform1i(programGetUniformLocation(program, name), x);
}

static void programBindAttribute(Program* const program, GLuint index, const GLchar *name)
{
    glBindAttribLocation(program->program, index, name);
}

static void programSetMat4fv(Program* const program, const char* name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(programGetUniformLocation(program, name), count, transpose, value);
}

static void programSetVec2fArray(Program* const program, const char* name, GLsizei count, const GLfloat* value)
{
    glUniform2fv(programGetUniformLocation(program, name), count, value);
}

static void programSetVec3fArray(Program* const program, const char* name, GLsizei count, const GLfloat* value)
{
    glUniform3fv(programGetUniformLocation(program, name), count, value);
}

static void programUse(Program* const program)
{
    glUseProgram(program->program);
}

struct ProgramManager programManagerInit()
{
    struct ProgramManager manager;
    manager.delete = &programDelete;
    manager.link = &programLink;
    manager.bindAttribute = &programBindAttribute;
    manager.use = &programUse;
    manager.setFloat = &programSetFloat;
    manager.setInt = &programSetInt;
    manager.setVec2f = &programSetVec2f;
    manager.setVec3f = &programSetVec3f;
    manager.setVec4f = &programSetVec4f;
    manager.setVec2fArray = &programSetVec2fArray;
    manager.setVec3fArray = &programSetVec3fArray;
    manager.init = &programInit;
    manager.getAttribLocation = &programGetAttribLocation;
    manager.getUniformLocation = &programGetUniformLocation;
    manager.setMat4fv = &programSetMat4fv;
    return manager;
}
