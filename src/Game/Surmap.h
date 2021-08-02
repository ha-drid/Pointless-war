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

    /**< а вот и сами данные храняшие карты высот*/
    struct
    {
        // ширина и высота загружемого файла
        uint32_t width, height;
        // формат файла (rgb или rgba)
        int format;
        // это данные о каждом пикселе файла
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
