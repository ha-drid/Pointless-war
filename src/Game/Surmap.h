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

/**< Эта структура будет отрисовывать мир с помошью карт вершин.
     Например берем монохромную картину, и начинаем покаждому проверять каждый пиксель.
     И чем пиксель будет белее тем выше он находиться.*/
struct Surmap
{
    /**< Это структура будеть хранить материалы для отрсовки.
         Например нужно отрсисовать сто кирпичей. Создаем один кирпич, все сотни отрисовываем этим кирпичем*/
    struct
    {
        struct VoxelInstance* data;
        uint32_t capacity; // количество материалов
    } materials;

    struct
    {
        struct Chunk data;
        uint32_t height, depth, width;
    } world;

    struct { uint32_t width, height, depth; } renderDistance;
};

struct SurmapManager
{
    void (*init)(struct Surmap* surmap,
                    const char* path,
                    void (*setBlockInWorld)(unsigned char* image_data,
                        int channels,
                        struct Chunk* worldData,
                        uint32_t width_world,
                        uint32_t height_world,
                        uint32_t depth_world,
                        struct ChunkManager* manage),
                    struct ChunkManager* manager);
    void (*addLoadMaterial)(struct Surmap* surmap,
                            const char* path,
                            uint32_t modelWidth,
                            uint32_t modelHeight,
                            uint32_t modelDepth,
                            struct VoxelInstanceManager* voxelInstance);
    void (*setRenderDistance)(struct Surmap* surmap, uint32_t width, uint32_t height, uint32_t depth);
    void (*draw)(struct Surmap* surmap,
                    float x_pos, float y_pos, float z_pos,
                    void (*renderVoxels)(struct VoxelInstance* mesh, float x, float y, float z));

    void (*delete)(struct Surmap* surmap, struct VoxelInstanceManager* voxelInstance, struct ChunkManager* chunk);
};

struct SurmapManager surmapManagerInit();

#endif // SURMAP_HEADER_FILE
