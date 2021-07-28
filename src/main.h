#ifndef MAIN_HEADER_FILE
#define MAIN_HEADER_FILE

#include <stdint-gcc.h>

static const float cube_position[18] = {
                     0.0f, 1.0f, 0.0f,

                     1.0f, 0.0f, 0.0f,
                    -1.0f, 0.0f, 0.0f,

                     0.0f, 0.0f, 1.0f,
                     0.0f, 0.0f,-1.0f,
};

static const uint32_t cube_index[6] = {
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

static const uint32_t cube_lines_index[36] = {
                     0,1,  1,2,  2,3,  3,0,
                     4,5,  5,6,  6,7,  7,4,
                     3,7,  2,6,  0,4,  1,5
                     };

enum VoxelType
{
    Empty,
    Grass,
    Stone,
    Wood,
    Red,
};

static const float VoxelColor[5][3] = {
    {  0.0f,  0.0f,   0.0f },
    {  0.23f, 1.0f,   0.24f },
    {  0.7f,  0.7f,   0.7f },
    {  0.85f, 0.32f,  0.12f},
    {  1.0f,  0.0f,   0.0f }
};

#endif // MAIN_HEADER_FILE
