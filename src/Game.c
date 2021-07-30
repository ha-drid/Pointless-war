#include "Game.h"
#include "Util/matrix.h"

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

void gameChunkDraw(struct Game* game,
                   uint32_t chunkID,
                   unsigned* indeces,
                   uint32_t indeces_size,
                   const char* positionInstance,
                   const char* colorInstance,
                   struct ProgramManager* program,
                   struct RenderManager* render)
{
    program->setVec3fArray(&(game->shader),
                            positionInstance,
                            game->world.mesh[chunkID].voxel_size,
                            game->world.mesh[chunkID].positionInstances);

    program->setVec3fArray(&(game->shader),
                            colorInstance,
                            game->world.mesh[chunkID].voxel_size,
                            game->world.mesh[chunkID].colorInstances);

    render->drawInstanced(&(game->render),
                         GL_TRIANGLES,
                         indeces_size,
                         GL_UNSIGNED_INT,
                         indeces,
                         game->world.mesh[chunkID].voxel_size);
}

void gameDraw(struct Game* game,
              unsigned* indeces,
              uint32_t indeces_size,
              struct Matrix* model,
              const char* positionInstance,
              const char* colorInstance,
              const char* model_name,
              struct GlobalManager* manager)
{
    int X = ((int)round(game->camera.x) - ((int)round(game->camera.x) % game->world.world.chunk_width)) / game->world.world.chunk_width;
    int Y = ((int)round(game->camera.y) - ((int)round(game->camera.y) % game->world.world.chunk_height)) / game->world.world.chunk_height;
    int Z = ((int)round(game->camera.z) - ((int)round(game->camera.z) % game->world.world.chunk_depth)) / game->world.world.chunk_depth;

    for (int z = Z - game->drawingDistance.z; z < Z + game->drawingDistance.z; ++z) {
    for (int y = Y - game->drawingDistance.y; y < Y + game->drawingDistance.y; ++y) {
    for (int x = X - game->drawingDistance.x; x < X + game->drawingDistance.x; ++x) {
                if (manager->chunks.isThereAChunk(&(game->world.world), x, y, z))
                {
                    matrixTranslate3f(model, x * 8, y * 8, z * 8);

                    manager->program.setMat4fv(&(game->shader), model_name, 1, GL_FALSE, model->data);

                    int index = x +
                                (y * game->world.world.chunks.countX) +
                                (z * (game->world.world.chunks.countX * game->world.world.chunks.countY));

                    gameChunkDraw(game,
                                index,
                                indeces,
                                indeces_size,
                                positionInstance,
                                colorInstance,
                                &(manager->program),
                                &(manager->render));

                }
    }
    }
    }
};

void gameDrawingDistance(struct Game* game, float x_size, float y_size, float z_size)
{
    game->drawingDistance.x = x_size;
    game->drawingDistance.y = y_size;
    game->drawingDistance.z = z_size;
}

void gameDelete(struct Game* game, struct GlobalManager* manager)
{
    manager->program.delete(&(game->shader));
    manager->render.delete(&(game->render));
    manager->voxelWorld.delete(&(game->world), &(manager->chunk), &(manager->chunks), &(manager->voxelInstance));
}
