#include "matrix.h"

void matrixInit(struct Matrix* mat, uint32_t width, uint32_t height)
{
    mat->data = (float*)malloc(sizeof(float) * (width * height));
    mat->height = height;
    mat->width = width;
}

void matrixSetFloat(struct Matrix* mat, float value)
{
    for (uint32_t y = 0; y < mat->height; ++y)
    {
        for (uint32_t x = 0; x < mat->width; ++x)
        {
            if (y == x)
            {
                mat->data[x + (y * mat->width)] = value;
            }
            else
            {
                mat->data[x + (y * mat->width)] = 0.0f;
            }
        }
    }
}

void matrixTranslate3f(struct Matrix* mat, float x, float y, float z)
{
    if (mat->width != 4 || mat->height != 4)
        assert(false);

    matrixSetFloat(mat, 1.0f);
    mat->data[0 + (3 * mat->width)] = x;
    mat->data[1 + (3 * mat->width)] = y;
    mat->data[2 + (3 * mat->width)] = z;
}

float* matrixGetFloat(struct Matrix* mat, uint32_t x, uint32_t y)
{
    return &(mat->data[x + (mat->width * y)]);
}

void matrixLookAt(struct Matrix* mat,
                  float eyex, float eyey, float eyez,
                  float centerx, float centery, float centerz,
                  float upx, float upy, float upz,
                  struct Vector3fManager* manager)
{
    vec3f eye = { eyex, eyey, eyez };
    vec3f center = { centerx, centery, centerz };
    vec3f up = { upx, upy, upz };

    struct vec3f f = manager->normalize(manager->subtraction(center, eye));
	struct vec3f s = manager->normalize(manager->cross(f, up));
	struct vec3f u = manager->cross(s, f);

	matrixSetFloat(mat, 1.0f);

	(*matrixGetFloat(mat, 0, 0)) = s.x;
	(*matrixGetFloat(mat, 0, 1)) = s.y;
	(*matrixGetFloat(mat, 0, 2)) = s.z;

	(*matrixGetFloat(mat, 1, 0)) = u.x;
	(*matrixGetFloat(mat, 1, 1)) = u.y;
	(*matrixGetFloat(mat, 1, 2)) = u.z;

	(*matrixGetFloat(mat, 2, 0)) =-f.x;
	(*matrixGetFloat(mat, 2, 1)) =-f.y;
	(*matrixGetFloat(mat, 2, 2)) =-f.z;

	(*matrixGetFloat(mat, 0, 3)) =-manager->dot(s, eye);
	(*matrixGetFloat(mat, 1, 3)) =-manager->dot(u, eye);
	(*matrixGetFloat(mat, 2, 3)) = manager->dot(f, eye);

}

void matrixPerpective(struct Matrix* mat, float angle_radians, float ratio, float near, float far)
{
    if (mat->width != 4 || mat->height != 4)
        assert(false);

    float tanHalfFovy = tan(angle_radians / 2.0);
    matrixSetFloat(mat, 0.0f);

	(*matrixGetFloat(mat, 0, 0)) = 1.0 / (ratio * tanHalfFovy);
	(*matrixGetFloat(mat, 1, 1)) = 1.0 / (tanHalfFovy);
	(*matrixGetFloat(mat, 2, 2)) = - (far + near) / (far - near);
	(*matrixGetFloat(mat, 3, 2)) = - 1;
	(*matrixGetFloat(mat, 2, 3)) = - (2 * far * near) / (far - near);
}

void matrixDelete(struct Matrix* mat)
{
    free((mat->data));
    mat->height = 0;
    mat->width = 0;
}
