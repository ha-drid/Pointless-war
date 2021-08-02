#ifndef SURMAP_HEADER_FILE
#define SURMAP_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <stdint-gcc.h>
#include "Voxels/Chunk.h"
#include "Voxels/VoxelInstance.h"

/**< ��� ��������� ����� ������������ ��� � ������� ���� ������.
     �������� ����� ����������� �������, � �������� ��������� ��������� ������ �������.
     � ��� ������� ����� ����� ��� ���� �� ����������.*/
struct Surmap
{
    /**< ��� ��������� ������ ������� ��������� ��� ��������.
         �������� ����� ����������� ��� ��������. ������� ���� ������, ��� ����� ������������ ���� ��������*/
    struct
    {
        struct VoxelInstance* data;
        uint32_t capacity; // ���������� ����������
    } materials;

    /**< � ��� � ���� ������ �������� ����� �����*/
    struct
    {
        // ������ � ������ ����������� �����
        uint32_t width, height;
        // ������ ����� (rgb ��� rgba)
        int format;
        // ��� ������ � ������ ������� �����
        unsigned char* data;
    } heightMap;
    struct { uint32_t width, depth; } renderDistance;
};

void surmapInit(struct Surmap* surmap, const char* path);
void surmapAddMaterial(struct Surmap* surmap,
                       const const struct Chunk* chunk,
                       uint32_t width_chunk,
                       uint32_t height_chunk,
                       uint32_t depth_chunk,
                       float (*voxelGetColor)(struct Voxel voxel, uint32_t index),
                       struct VoxelInstanceManager* voxelInstance);
void surmapAddLoadMaterial(struct Surmap* surmap,
                           const char* path,
                           uint32_t modelWidth,
                           uint32_t modelHeight,
                           uint32_t modelDepth,
                           struct VoxelInstanceManager* voxelInstance);
void surmapSetRenderDistance(struct Surmap* surmap, uint32_t width, uint32_t depth);
void surmapDraw(struct Surmap* surmap,
                float x_pos,
                float z_pos,
                void (*renderVoxels)(struct VoxelInstance* mesh, float x, float y, float z));

void surmapDelete(struct Surmap* surmap, struct VoxelInstanceManager* voxelInstance);

#endif // SURMAP_HEADER_FILE
