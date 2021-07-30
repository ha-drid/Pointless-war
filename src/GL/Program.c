#include "Program.h"

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

unsigned loadShader(const char* path, unsigned shader_type)
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
    }

    return shader;
}

void programInit(Program* program, const char* vertexShader, const char* fragmentShader)
{
    program->vertexShader = loadShader(vertexShader, GL_VERTEX_SHADER);
    program->fragmentShader = loadShader(fragmentShader, GL_FRAGMENT_SHADER);

    program->program = glCreateProgram();
}

void programLink(Program* program)
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
    }
}

void programDelete(Program* program)
{
    glDetachShader(program->program, program->vertexShader);
    glDetachShader(program->program, program->fragmentShader);

    glDeleteShader(program->vertexShader);
    glDeleteShader(program->fragmentShader);

    glDeleteProgram(program->program);
}

GLint programGetAttribLocation(Program* program, const char* name)
{
    return glGetAttribLocation(program->program, name);
}

GLint programGetUniformLocation(Program* program, const char* name)
{
    return glGetUniformLocation(program->program, name);
}

void programSetFloat(Program* program, const char* name, float x)
{
    glUniform1f(programGetUniformLocation(program, name), x);
}

void programSetVec3f(Program* program, const char* name, float x, float y, float z)
{
    glUniform3f(programGetUniformLocation(program, name), x, y, z);
}

void programSetInt(Program* program, const char* name, int x)
{
    glUniform1i(programGetUniformLocation(program, name), x);
}

void programSetMat4fv(struct Program* program, const char* name, GLsizei count, GLboolean transpose, const GLfloat *value)
{
    glUniformMatrix4fv(programGetUniformLocation(program, name), count, transpose, value);
}
void programSetVec3fArray(struct Program* program, const char* name, GLsizei count, const GLfloat* value)
{
    glUniform3fv(programGetUniformLocation(program, name), count, value);
}

void programUse(Program* program)
{
    glUseProgram(program->program);
}

struct ProgramManager programManagerInit()
{
    struct ProgramManager manager;
    manager.delete = &programDelete;
    manager.link = &programLink;
    manager.use = &programUse;
    manager.setFloat = &programSetFloat;
    manager.setInt = &programSetInt;
    manager.setVec3f = &programSetVec3f;
    manager.setVec3fArray = &programSetVec3fArray;
    manager.init = &programInit;
    manager.getAttribLocation = &programGetAttribLocation;
    manager.getUniformLocation = &programGetUniformLocation;
    manager.setMat4fv = &programSetMat4fv;
    return manager;
}
