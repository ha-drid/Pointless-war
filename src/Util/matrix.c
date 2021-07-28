#include "matrix.h"

void createMatrix(Matrix* matrix, uint32_t width, uint32_t height)
{
    matrix->height = height;
    matrix->width = width;

    matrix->data = (float**)malloc(sizeof(float*) * height);
    for (int i = 0; i < height; ++i)
    {
        matrix->data[i] = (float*)malloc(sizeof(float) * width);
    }

    for (uint32_t y = 0; y < height; ++y)
    {
        for (uint32_t x = 0; x < width; ++x)
        {
            matrix->data[y][x] = 0;
        }
    }
}

void matrixPerpective(Matrix* matrix, float angle_radians, float ratio, float near, float far)
{
    if ( (matrix->height > 4) || (matrix->width > 4))
    {
        return;
    }

    float tangens_half_angle = tan(angle_radians / 2.0f);

    Matrix* m = matrix;
    m->data[0][0] = 1.0f / (ratio * tangens_half_angle);
    m->data[1][1] = 1.0f / (tangens_half_angle);
    m->data[2][2] = -(far + near) / (far - near);
    m->data[2][3] = -1.0f;
    m->data[3][2] = -(2.0f * far * near) / (far - near);
}

void matrixLookAt(Matrix* matrix, vec3f position, vec3f direction, vec3f up)
{
    const vec3f f = vector3f_normalize(vector3f_subtraction(direction, position));
    const vec3f s = vector3f_normalize(vector3f_cross(f, up));
    const vec3f u = vector3f_cross(s, f);

    Matrix* m = matrix;
    for (uint32_t y = 0; y < m->height; ++y)
    {
        for (uint32_t x = 0; x < m->width; ++x)
        {
            if (y == x)
                m->data[y][x] = 1.0f;
            else
                m->data[y][x] = 0.0f;
        }
    }

    m->data[0][0] = s.x;
    m->data[1][0] = s.y;
    m->data[2][0] = s.z;

    m->data[0][1] = u.x;
    m->data[1][1] = u.y;
    m->data[2][1] = u.z;

    m->data[0][2] =-f.x;
    m->data[1][2] =-f.y;
    m->data[2][2] =-f.z;

    m->data[3][0] =-vector3f_dot(s, position);
    m->data[3][1] =-vector3f_dot(u, position);
    m->data[3][2] = vector3f_dot(f, position);
}

void deleteMatrix(Matrix* matrix)
{
    for (int i = 0; i < matrix->height; ++i)
    {
        free(matrix->data[i]);
    }

    free(matrix->data);


    matrix->height = 0;
    matrix->width = 0;
}
