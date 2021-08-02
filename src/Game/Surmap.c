#include "Surmap.h"
#include <stb_image.h>

void surmapInit(struct Surmap* surmap, const char* path)
{
    surmap->heightMap.data = stbi_load(path,
                                       &(surmap->heightMap.width),
                                       &(surmap->heightMap.height),
                                       &(surmap->heightMap.format),
                                       0);

    surmap->materials.data = NULL;
    surmap->materials.capacity = 0;
}

void surmapAddMaterial(struct Surmap* surmap,
                       const const struct Chunk* chunk,
                       uint32_t width_chunk,
                       uint32_t height_chunk,
                       uint32_t depth_chunk,
                       float (*voxelGetColor)(struct Voxel voxel, uint32_t index),
                       struct VoxelInstanceManager* voxelInstance)
{
    uint32_t i = surmap->materials.capacity;
    ++surmap->materials.capacity;

    surmap->materials.data = realloc(surmap->materials.data, surmap->materials.capacity);

    voxelInstance->init(&(surmap->materials.data[i]), width_chunk * height_chunk * depth_chunk);
    voxelInstance->update(&(surmap->materials.data[i]), chunk, width_chunk, height_chunk, depth_chunk, voxelGetColor);
}

void surmapDraw(struct Surmap* surmap, float x_pos, float z_pos, void (*renderVoxels)(struct VoxelInstance* mesh, float x, float y, float z))
{
    for (int z = z_pos - surmap->renderDistance.depth; z < z_pos + surmap->renderDistance.depth; ++z)
    {
        for (int x = x_pos - surmap->renderDistance.width; x < x_pos + surmap->renderDistance.width; ++x)
        {
            if ((x < 0) || (x >= surmap->heightMap.width) ||
                (z < 0) || (z >= surmap->heightMap.height))
                continue;

            renderVoxels(&(surmap->materials.data[0]),  x*8,(surmap->heightMap.data[x + (z*surmap->heightMap.width)]/15)*8,z*8);
            renderVoxels(&(surmap->materials.data[1]),  x*8,((surmap->heightMap.data[x + (z*surmap->heightMap.width)]/15) - 1)*8,z*8);

        }
    }

}

void surmapAddLoadMaterial(struct Surmap* surmap,
                           const char* path,
                           uint32_t modelWidth,
                           uint32_t modelHeight,
                           uint32_t modelDepth,
                           struct VoxelInstanceManager* voxelInstance)
{
    uint32_t i = surmap->materials.capacity;
    ++surmap->materials.capacity;

    surmap->materials.data = realloc(surmap->materials.data, sizeof(struct VoxelInstance) * surmap->materials.capacity);
    voxelInstance->init(&(surmap->materials.data[i]), modelDepth * modelHeight * modelWidth);

    // load image file
    stbi_set_flip_vertically_on_load(true);
    int imageWidth, imageHeight, imageChannels;
    unsigned char* imageData = stbi_load(path, &imageWidth, &imageHeight, &imageChannels, 0);
    stbi_set_flip_vertically_on_load(false);

    if ((imageWidth * imageHeight) != (modelWidth * modelHeight * modelDepth))
    {
        printf ("The image %s and the size of the model does not fit", path);
        printf("%u %i", modelWidth * modelHeight * modelDepth, imageWidth * imageHeight);
        assert(0);
    }

    uint32_t index = 0;
    float one = 1.0 / 255.0;
    for (uint32_t z = 0; z < modelDepth; ++z) {
        for (uint32_t y = 0; y < modelHeight; ++y) {
            for (uint32_t x = 0; x < modelWidth; ++x) {
                surmap->materials.data[i].positionInstances[(index * 3) + 0] = x;
                surmap->materials.data[i].positionInstances[(index * 3) + 1] = y;
                surmap->materials.data[i].positionInstances[(index * 3) + 2] = z;

                uint32_t iZ = z * (modelHeight * modelWidth);
                uint32_t iY = y * modelWidth;

                surmap->materials.data[i].colorInstances[(index * 3) + 0] = one * imageData[(index * 3) + 0];
                surmap->materials.data[i].colorInstances[(index * 3) + 1] = one * imageData[(index * 3) + 1];
                surmap->materials.data[i].colorInstances[(index * 3) + 2] = one * imageData[(index * 3) + 2];
                ++index;
            }
        }
    }
    surmap->materials.data[i].voxel_size = index;

    stbi_image_free(imageData);
};

void surmapSetRenderDistance(struct Surmap* surmap, uint32_t width, uint32_t depth)
{
    surmap->renderDistance.depth = depth;
    surmap->renderDistance.width = width;
}

void surmapDelete(struct Surmap* surmap, struct VoxelInstanceManager* voxelInstance)
{
    for (uint32_t i = 0; i < surmap->materials.capacity; ++i)
    {
        voxelInstance->delete(&(surmap->materials.data[i]));
    }

    free(surmap->materials.data);
    surmap->materials.capacity = 0;

    stbi_image_free(surmap->heightMap.data);
}
