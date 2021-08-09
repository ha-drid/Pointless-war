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

/**< Эта структура будет отрисовывать мир с помошью карт вершин.
     Например берем монохромную картину, и начинаем покаждому проверять каждый пиксель.
     И чем пиксель будет белее тем выше он находиться.*/
struct Surmap
{
    /**< Это структура будеть хранить материалы для отрсовки.
         Например нужно отрсисовать сто кирпичей. Создаем один кирпич, все сотни отрисовываем этим кирпичем*/
    struct
    {
        struct VoxelInstance* data; // Сами данные о материале. Оговорка мы будем рисовать материалы вокселями.
                                    // То есть например: материал земли будеть 8 в ширину, 8 в высоту, 8 в глубину вокселей,
                                    // в итоге в этом материале будеть 640 вокселей
        uint32_t blockW, blockH, blockD; // размер всех материалов
        uint32_t capacity; // количество материалов
    } materials;

    struct
    {
        struct Chunk data; // Один большой чанк который будеть хранить все воксели этого мира.
        struct bitset_s* isViewVoxel; // Это структура чтобы понимать какие воксели могут юыть видны а какие нет.
                                    // Ведь зачем рисовать то что все равно не будеть видно
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
