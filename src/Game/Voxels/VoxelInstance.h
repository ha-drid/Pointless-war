#ifndef VOXELINSTANCE_HEADER_FILE
#define VOXELINSTANCE_HEADER_FILE

#include "Voxel.h"
#include "Chunk.h"
#include <cvector/vector.h>

typedef struct VoxelInstance {
    vector( vector(float) ) buffer;
    uint32_t voxelSize;
} VoxelInstance;

typedef struct VoxelInstanceManager {
    void (*init)(VoxelInstance* const ins, uint32_t bufferSize);
    void (*updateBuffers)(VoxelInstance* const ins, int chunkPos[3],
                                                                void (*const instanceUpdate)
                                (uint32_t* voxelSize, int chunkPos[3], vector(vector(float))* data));
    void (*delete)(VoxelInstance* const ins);
} VoxelInstanceManager;

VoxelInstanceManager voxelInstanceManagerInit();

#endif // VOXELINSTANCE_HEADER_FILE
