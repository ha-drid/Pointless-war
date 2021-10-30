#ifndef MAIN_HEADER_FILE
#define MAIN_HEADER_FILE

#include <stdint-gcc.h>

static const float VOXEL_POSITION[18] = {
                     0.0f, 0.0f, 0.0f,

                     1.0f, 1.0f, 0.0f,
                    -1.0f, 1.0f, 0.0f,

                     0.0f, 1.0f, 1.0f,
                     0.0f, 1.0f,-1.0f,
};

static const float VOXEL_SIZE = 1.0f;

static const uint32_t VOXEL_INDEX[6] = {
                     0, 1, 2,
                     0, 3, 4
                     /*0,1,2,  0,2,3,
                     4,5,6,  4,6,7,

                     1,2,5,  6,5,2,
                     0,3,7,  0,4,7,

                     3,2,7,  2,7,6,
                     0,1,4,  1,4,5,
                     */
                     };

static const float CUBE_POSITION[24] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 1.0f,
    1.0f, 1.0f, 1.0f,
};

static const float CUBE_UV[16] = {
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f,
    0.0f, 0.0f,

    1.0f, 0.0f,
    1.0f, 1.0f,
    0.0f, 1.0f,
    0.0f, 0.0f,
};

static const unsigned CUBE_INDEX[36] = {
    0, 1, 2,
    1, 2, 3,

    4, 5, 6,
    5, 6, 7,

    1, 3, 5,
    3, 7, 5,

    2, 3, 6,
    7, 6, 3,

    0, 2, 6,
    0, 4, 6,


};

enum VoxelType
{
    Empty,
    Grass,
    Stone,
    Dirt,
};

static const float RectPosition[18] = {
    0.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,
};

static const float RectUv[18] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.0f, 1.0f,

    1.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
};

static const float RectColor[24] = {
    0.0f, 0.0f, 0.0f, 0.5f,
    1.0f, 0.0f, 0.0f, 0.5f,
    0.0f, 1.0f, 0.0f, 0.5f,

    1.0f, 0.0f, 0.0f, 0.5f,
    0.0f, 1.0f, 0.0f, 0.5f,
    1.0f, 1.0f, 0.0f, 0.5f
};

static const float VoxelColor[5][3] = {
    {  0.0f,  0.0f,   0.0f },
    {  0.23f, 1.0f,   0.24f },
    {  0.7f,  0.7f,   0.7f },
    {  0.85f, 0.32f,  0.12f},
    {  1.0f,  0.0f,   0.0f }
};

#endif // MAIN_HEADER_FILE
