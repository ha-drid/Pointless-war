#include "vec3.h"

vec3f vector3f_subtraction(vec3f vec1, vec3f vec2)
{
    vec3f var;
    var.x = vec1.x - vec2.x;
    var.y = vec1.y - vec2.y;
    var.z = vec1.z - vec2.z;

    return var;
}

vec3f vector3f_addition(vec3f vec1, vec3f vec2)
{
    vec3f var;

    var.x = vec1.x + vec2.x;
    var.y = vec1.y + vec2.y;
    var.z = vec1.z + vec2.z;

    return var;
}

vec3f vector3f_normalize(vec3f vec)
{
    vec3f var;

    var.x = vec.x / sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    var.y = vec.y / sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));
    var.z = vec.z / sqrt((vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z));

    return var;
}

vec3f vector3f_cross(vec3f vec1, vec3f vec2)
{
    vec3f var;

    var.x = ((vec1.y * vec2.z) - (vec2.y * vec1.z));
    var.y = ((vec1.z * vec2.x) - (vec2.z * vec1.x));
    var.z = ((vec1.x * vec2.y) - (vec2.x * vec1.y));

    return var;
}

float vector3f_dot(vec3f vec1, vec3f vec2)
{
    return (vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z);
}

void vector3f_set(vec3f* vec, float x, float y, float z)
{
    vec->x = x;
    vec->y = y;
    vec->z = z;
}

struct Vector3fManager vector3fManagerInit()
{
    struct Vector3fManager manager;
    manager.addition = &vector3f_addition;
    manager.cross = &vector3f_cross;
    manager.dot = &vector3f_dot;
    manager.normalize = &vector3f_normalize;
    manager.set = &vector3f_set;
    manager.subtraction = &vector3f_subtraction;

    return manager;
}
