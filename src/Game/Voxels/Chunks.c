#include "Chunks.h"

static void chunksInit(struct Chunks* chunks,
                uint32_t chunk_countX,
                uint32_t chunk_countY,
                uint32_t chunk_countZ,
                uint32_t width_chunk,
                uint32_t height_chunk,
                uint32_t depth_chunk,
                struct ChunkManager* manager)
{
    chunks->chunks.countX = chunk_countX;
    chunks->chunks.countY = chunk_countY;
    chunks->chunks.countZ = chunk_countZ;

    chunks->chunk_width = width_chunk;
    chunks->chunk_height = height_chunk;
    chunks->chunk_depth = depth_chunk;

    chunks->chunks.data = (struct Chunk***)malloc(sizeof(struct Chunk**) * chunk_countZ);

    for (uint32_t i = 0; i < chunk_countZ; ++i) {
        chunks->chunks.data[i] = (struct Chunk**)malloc(sizeof(struct Chunk*) * chunk_countY);
    }

    for (uint32_t i = 0; i < chunk_countZ; ++i) {
        for (uint32_t n = 0; n < chunk_countY; ++n) {
            chunks->chunks.data[i][n] = (struct Chunk*)malloc(sizeof(struct Chunk) * chunk_countX);
        }
    }


    for (uint32_t z = 0; z < chunk_countZ; ++z) {
        for (uint32_t y = 0; y < chunk_countY; ++y) {
            for (uint32_t x = 0; x < chunk_countX; ++x) {
                manager->init(&(chunks->chunks.data[z][y][x]), width_chunk, height_chunk, depth_chunk);
            }
        }
    }

     for (uint32_t z = 0; z < chunk_countZ; ++z) {
        for (uint32_t y = 0; y < chunk_countY; ++y) {
            for (uint32_t x = 0; x < chunk_countX; ++x) {
                struct Voxel vox = { (rand() % 4) };
                manager->fill(&(chunks->chunks.data[z][y][x]), width_chunk, height_chunk, depth_chunk, vox);
            }
        }
    }
}

static bool chunksIsThereAChunk(struct Chunks* chunks, int x, int y, int z)
{
    if ((x < 0) || (x >= chunks->chunks.countX) ||
        (y < 0) || (y >= chunks->chunks.countY) ||
        (z < 0) || (z >= chunks->chunks.countZ))
        return false;

    return true;
}

static void chunksDelete(struct Chunks* chunks, struct ChunkManager* manager)
{
    for (uint32_t z = 0; z < chunks->chunks.countZ; ++z) {
        for (uint32_t y = 0; y < chunks->chunks.countY; ++y) {
            for (uint32_t x = 0; x < chunks->chunks.countX; ++x) {
                manager->delete(&(chunks->chunks.data[z][y][x]));
            }
        }
    }

    for (uint32_t i = 0; i < chunks->chunks.countZ; ++i) {
        for (uint32_t n = 0; n < chunks->chunks.countY; ++n) {
            free(chunks->chunks.data[i][n]);
        }
    }

    for (uint32_t i = 0; i < chunks->chunks.countZ; ++i) {
        free(chunks->chunks.data[i]);
    }

    free(chunks->chunks.data);

    chunks->chunks.countX = 0;
    chunks->chunks.countY = 0;
    chunks->chunks.countZ = 0;
}

struct ChunksManager chunksManagerInit()
{
    struct ChunksManager manager;
    manager.init = &chunksInit;
    manager.isThereAChunk = &chunksIsThereAChunk;
    manager.delete = &chunksDelete;
    return manager;
};
