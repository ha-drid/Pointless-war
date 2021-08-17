#ifndef VEC_HEADER_FILE
#define VEC_HEADER_FILE

typedef struct vec3f
{
    float x, y, z;
} vec3f;

#define Vec3f vec3f

struct Vector3fManager
{
    vec3f* (*new)(float x,  float y, float z);
    vec3f (*subtraction)(vec3f vec1, vec3f vec2);
    vec3f (*addition)(vec3f vec1, vec3f vec2);
    vec3f (*normalize)(vec3f vec);
    vec3f (*cross)(vec3f vec1, vec3f vec2);
    float (*dot)(vec3f vec1, vec3f vec2);
    void  (*set)(vec3f* vec, float x, float y, float z);
    void (*delete)(vec3f* vec);
};

struct Vector3fManager vector3fManagerInit();

#endif // VEC_HEADER_FILE
