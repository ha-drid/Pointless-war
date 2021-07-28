#include "Game.h"

void gameProgramInit(struct Game* game, const char* vert, const char* frag, struct GlobalManager* manager)
{
    manager->program.init(&(game->shader), vert, frag);
    manager->program.link(&(game->shader));
    manager->program.use(&(game->shader));
}

void gameRenderInit(struct Game* game,
                    const char* attrib_name,
                    GLint size,
                    GLboolean normalized,
                    GLsizei stride,
                    const float* array,
                    struct GlobalManager* manager)
{

    glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);


    manager->render.init(&(game->render));
    manager->render.addAttribute(&(game->render),
                                 &(game->shader),
                                 &(manager->program),
                                 attrib_name,
                                 size,
                                 normalized,
                                 stride, array);

};

void gameVoxelWorldInit(struct Game* game, uint32_t w, uint32_t h, uint32_t d, struct GlobalManager* manager)
{
    manager->voxelWorld.init(&(game->world), w, h, d, &(manager->chunk), &(manager->chunks), &(manager->voxelInstance));
    manager->voxelWorld.update((&game->world), voxelGetColor, &(manager->chunk), &(manager->chunks), &(manager->voxelInstance));
}

void gameCameraInit(struct Game* game,
                    float x,
                    float y,
                    float z,
                    float xAngle,
                    float yAngle,
                    float speed,
                    struct GlobalManager* manager)
{
    manager->cameraGL.init(&(game->camera), x, y, z, xAngle, yAngle, speed);
};

void gameDraw(struct Game* game,
              unsigned* indeces,
              uint32_t indeces_size,
              const char* positionInstance,
              const char* colorInstance,
              const char* translate,
              struct GlobalManager* manager)
{
    uint32_t index = 0;
    for (uint32_t z = 0; z < game->world.world.chunks.countZ; ++z) {
        for (uint32_t y = 0; y < game->world.world.chunks.countY; ++y) {
            for (uint32_t x = 0; x < game->world.world.chunks.countX; ++x) {

                manager->program.setVec3f(&(game->shader),
                                               translate,
                                               x * 8,
                                               y * 8,
                                               z * 8);

                manager->program.setVec3fArray(&(game->shader),
                                                    positionInstance,
                                                    game->world.mesh[index].voxel_size,
                                                    game->world.mesh[index].positionInstances);

                manager->program.setVec3fArray(&(game->shader),
                                                    colorInstance,
                                                    game->world.mesh[index].voxel_size,
                                                    game->world.mesh[index].colorInstances);

                manager->render.drawInstanced(&(game->render),
                                                   GL_TRIANGLES,
                                                   indeces_size,
                                                   GL_UNSIGNED_INT,
                                                   indeces,
                                                   game->world.mesh[index].voxel_size);
                // globalManager.render.draw(&render, GL_TRIANGLES, 6, GL_UNSIGNED_INT, cube_index);
                ++index;
            }
        }
    }
};

void gameDelete(struct Game* game, struct GlobalManager* manager)
{
    manager->program.delete(&(game->shader));
    manager->render.delete(&(game->render));
    manager->voxelWorld.delete(&(game->world), &(manager->chunk), &(manager->chunks), &(manager->voxelInstance));
}
