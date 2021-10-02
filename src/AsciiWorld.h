#ifndef ASCIIWORLD_HEADER_FILE
#define ASCIIWORLD_HEADER_FILE

#include "Game/Voxels/Voxel.h"
#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/VoxelInstance.h"
#include "GL/VertexArrayObject.h"

typedef Voxel Symbol;

typedef struct AsciiWorld
{
    Chunk*** data;
    uint32_t width, height, depth;
    struct { uint8_t x, y, z; } chunkSize;
    VoxelInstance* mesh;
    ChunkManager chunk;
    VoxelInstanceManager voxelInstance;
} AsciiWorld;

typedef struct AsciiWorldManager
{
    void (*init)(AsciiWorld* const world,
                           uint32_t w, uint32_t h, uint32_t d);
    Voxel (*getVoxel)(AsciiWorld* const world, int x, int y, int z);
    void (*setVoxel)(AsciiWorld* const world, int x, int y, int z, Voxel value);
    void (*update)(AsciiWorld* const world);
    void (*delete)(AsciiWorld* const world);
    void (*draw)(AsciiWorld* const world, void (*const drawInstanced)(VoxelInstance* const mesh, float x, float y, float z));
} AsciiWorldManager;

AsciiWorldManager asciiWorldManagerInit();

#endif // ASCIIWORLD_HEADER_FILE

