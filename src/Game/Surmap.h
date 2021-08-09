#ifndef SURMAP_HEADER_FILE
#define SURMAP_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <stdint-gcc.h>
#include <cbitset/bitset.h>
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
        struct VoxelInstance* data; // ���� ������ � ���������. �������� �� ����� �������� ��������� ���������.
                                    // �� ���� ��������: �������� ����� ������ 8 � ������, 8 � ������, 8 � ������� ��������,
                                    // � ����� � ���� ��������� ������ 640 ��������
        uint32_t blockW, blockH, blockD; // ������ ���� ����������
        uint32_t capacity; // ���������� ����������
    } materials;

    struct
    {
        struct Chunk data; // ���� ������� ���� ������� ������ ������� ��� ������� ����� ����.
        struct bitset_s* isViewVoxel; // ��� ��������� ����� �������� ����� ������� ����� ���� ����� � ����� ���.
                                    // ���� ����� �������� �� ��� ��� ����� �� ������ �����
        uint32_t height, depth, width;
    } world;

    struct { uint32_t width, height, depth; } renderDistance;
};

struct SurmapManager
{
    void (*init)(struct Surmap* surmap,
                const char* path,
                uint32_t blockWidth, uint32_t blockHeight, uint32_t blockDepth,
                void (*setBlockInWorld)(unsigned char* image_data, int channels,
                                        struct Chunk* world, uint32_t widthWorld, uint32_t heightWorld, uint32_t depthWorld,
                                        struct ChunkManager* manage),
                struct ChunkManager* chunkManager);

    void (*addLoadMaterial)(struct Surmap* surmap, const char* path, struct VoxelInstanceManager* voxelInstance);
    void (*setRenderDistance)(struct Surmap* surmap, uint32_t width, uint32_t height, uint32_t depth);
    void (*draw)(struct Surmap* surmap,
                    float x_pos, float y_pos, float z_pos,
                    void (*renderVoxels)(struct VoxelInstance* mesh, float x, float y, float z));

    void (*delete)(struct Surmap* surmap, struct VoxelInstanceManager* voxelInstance, struct ChunkManager* chunk);
};

struct SurmapManager surmapManagerInit();

#endif // SURMAP_HEADER_FILE
