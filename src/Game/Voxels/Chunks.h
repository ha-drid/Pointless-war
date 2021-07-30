#ifndef CHUNKS_HEADER_FILE
#define CHUNKS_HEADER_FILE

#include <stdio.h>
#include <stdint-gcc.h>
#include <stdlib.h>

#include "Voxel.h"
#include "Chunk.h"

struct Chunks
{
    struct
    {
        struct Chunk*** data;
        uint32_t countX;
        uint32_t countY;
        uint32_t countZ;

    } chunks;
    // размер одного чанка
    uint32_t chunk_width;
    uint32_t chunk_height;
    uint32_t chunk_depth;
};

struct ChunksManager
{
    void (*init)(struct Chunks* chunks,
                uint32_t chunk_countX,
                uint32_t chunk_countY,
                uint32_t chunk_countZ,
                uint32_t width_chunk,
                uint32_t height_chunk,
                uint32_t depth_chunk,
                struct ChunkManager* manager);
    bool (*isThereAChunk)(struct Chunks* chunks, int x, int y, int z);
    void (*delete)(struct Chunks* chunks, struct ChunkManager* manager);
};

struct ChunksManager chunksManagerInit();

#endif // CHUNKS_HEADER_FILE
