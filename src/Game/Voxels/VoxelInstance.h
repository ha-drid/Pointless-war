#ifndef VOXELINSTANCE_HEADER_FILE
#define VOXELINSTANCE_HEADER_FILE

#include "Voxel.h"
#include "Chunk.h"

/**< Draw instansting */
struct VoxelInstance
{
    float* positionInstances;
    float* colorInstances; // RGB
    uint32_t voxel_size;
};
/**
    float triangles[9] = { 0,0,0,  0,1,0, 1,0,0 };
    struct VoxelInstance ins;

    ..
    ..
    ..
    glUniform3fv(position_uniform_id, ins.voxel_size, ins.positionInstances);
    glUniform3fv(color_uniform_id, ins.voxel_size, ins.colorInstances);

    glDrawArraysInstanced(GL_TRIANGLES, 0, 3, ins.voxel_size);
*/


struct VoxelInstanceManager
{
    void (*init)(struct VoxelInstance* mesh, uint32_t max_voxel_size);
    void (*update)(struct VoxelInstance* mesh,
                         struct Chunk* chunk,
                         uint32_t chunk_width,
                         uint32_t chunk_height,
                         uint32_t chunk_depth,
                         float (*getVoxelColor)(struct Voxel vox, uint32_t index));
    void (*delete)(struct VoxelInstance* mesh);
};

struct VoxelInstanceManager voxelInstanceManagerInit();

#endif // VOXELINSTANCE_HEADER_FILE
