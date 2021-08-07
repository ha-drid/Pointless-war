#include "Surmap.h"
#include <stb_image.h>

static void surmapInit(struct Surmap* surmap,
                const char* path,
                void (*setBlockInWorld)(unsigned char* image_data,
                     int channels,
                     struct Chunk* worldData,
                     uint32_t width_world,
                     uint32_t height_world,
                     uint32_t depth_world,
                     struct ChunkManager* manage),
                struct ChunkManager* manager)
{
    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);

    manager->init(&(surmap->world.data), width, 256, height);
    surmap->world.depth = height;
    surmap->world.height = 256;
    surmap->world.width = width;
    manager->fill(&(surmap->world.data), width, 256, height, (struct Voxel){ 0 });

    setBlockInWorld(data, channels,
                    &(surmap->world.data),
                    surmap->world.width,
                    surmap->world.height,
                    surmap->world.depth,
                    manager);

    stbi_image_free(data);
    surmap->materials.capacity = 0;
    surmap->materials.data = NULL;
}

static void surmapDraw(struct Surmap* surmap, float x_pos, float y_pos, float z_pos, void (*renderVoxels)(struct VoxelInstance* mesh, float x, float y, float z))
{
    for (int z = z_pos - surmap->renderDistance.depth; z < z_pos + surmap->renderDistance.depth; ++z) {
        for (int y = y_pos - surmap->renderDistance.height; y < y_pos + surmap->renderDistance.height; ++y) {
            for (int x = x_pos - surmap->renderDistance.width; x < x_pos + surmap->renderDistance.width; ++x) {
                if ((x < 0) || (x >= surmap->world.width) ||
                    (y < 0) || (y >= surmap->world.height) ||
                    (z < 0) || (z >= surmap->world.depth))
                    continue;

                uint32_t iZ = z * (surmap->world.width * surmap->world.height);
                uint32_t iY = y * surmap->world.width;
                unsigned iD = surmap->world.data.voxels[iZ + iY + x].iD;

                if (iD == 0)
                    continue;
                else if (iD >= surmap->materials.capacity)
                    renderVoxels(&(surmap->materials.data[0]), x * 8, y * 8, z * 8);
                else
                    renderVoxels(&(surmap->materials.data[iD]), x * 8, y * 8, z * 8);
            }
        }
    }

}

static void surmapAddLoadMaterial(struct Surmap* surmap,
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

static void surmapSetRenderDistance(struct Surmap* surmap, uint32_t width, uint32_t height, uint32_t depth)
{
    surmap->renderDistance.depth = depth;
    surmap->renderDistance.height = height;
    surmap->renderDistance.width = width;
}

static void surmapDelete(struct Surmap* surmap, struct VoxelInstanceManager* voxelInstance, struct ChunkManager* chunk)
{
    for (uint32_t i = 0; i < surmap->materials.capacity; ++i)
    {
        voxelInstance->delete(&(surmap->materials.data[i]));
    }
    chunk->delete(&(surmap->world.data));

    free(surmap->materials.data);
    surmap->materials.capacity = 0;
}

struct SurmapManager surmapManagerInit()
{
    struct SurmapManager manager;
    manager.addLoadMaterial = &surmapAddLoadMaterial;
    manager.delete = &surmapDelete;
    manager.draw = &surmapDraw;
    manager.init = &surmapInit;;
    manager.setRenderDistance = &surmapSetRenderDistance;

    return manager;
};
