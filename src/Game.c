#include "Game.h"
#include <clist/list.h>


void setBlockInWorld(unsigned char* image_data,
                     int channels,
                     struct Chunk* worldData,
                     uint32_t width_world,
                     uint32_t height_world,
                     uint32_t depth_world,
                     struct ChunkManager* manage);

void gameInit(struct Game* game, struct GlobalManager* manager)
{
    manager->time.init(&(game->ltime));
    manager->cameraGL.init(&(game->camera), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 3.0f);

    manager->surmap.init(&(game->surmap), "res\\img\\heightmap.png", 8, 8, 8, setBlockInWorld, &(manager->chunk));

    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\undefined.bmp", &(manager->voxelInstance));
    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\grass.bmp", &(manager->voxelInstance));
    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\Stone.bmp", &(manager->voxelInstance));
    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\dirt.bmp", &(manager->voxelInstance));

    manager->surmap.setRenderDistance(&(game->surmap), 30, 30, 30);

    manager->program.init(&(game->shader), "res\\glsl\\first.vert", "res\\glsl\\first.frag");
    manager->program.link(&(game->shader));
    manager->program.use(&(game->shader));

    manager->vao.init(&(game->voxel));
    manager->vao.addVertexBufferObject(&(game->voxel), 3, 24, CUBE_POSITION);
    manager->vao.addIndices(&(game->voxel), 36, CUBE_INDEX);

    manager->mvp.init(&(game->mvp), &(game->shader), "u_Model", "u_View", "u_Projection", &(manager->program),&(manager->matrix));
    manager->mvp.setProjection(&(game->mvp), radians_s(90.0f), 5.0f / 3.0f, 0.1, 200.0f, &(manager->matrix));
}

void gameLoop(struct Game* game, struct WindowGL* win, struct GlobalManager* manager)
{
    void renderVoxels(struct VoxelInstance* mesh, float x, float y, float z) {
        manager->matrix.translate3f(&(game->mvp.model), x, y, z);
        manager->program.setMat4fv(&(game->shader), "u_Model", 1, GL_FALSE, game->mvp.model.data);
        manager->program.setVec3fArray(&(game->shader), "positionInstances", mesh->voxel_size, mesh->positionInstances);
        manager->program.setVec3fArray(&(game->shader), "colorInstances", mesh->voxel_size, mesh->colorInstances);
        manager->vao.drawElementsInstanced(&(game->voxel), GL_TRIANGLES, mesh->voxel_size);
    }

    void renderVoxel(float x, float y, float z) {
        manager->matrix.translate3f(&(game->mvp.model), x, y, z);
        manager->program.setMat4fv(&(game->shader), "u_Model", 1, GL_FALSE, game->mvp.model.data);
        manager->program.setVec3fArray(&(game->shader), "colorInstances", 1, VoxelColor[Stone]);
        manager->vao.drawElements(&(game->voxel), GL_TRIANGLES);
    }

    bool objectWorldIsColision(struct Object* obj) {
        int blockH = game->surmap.materials.blockH;
        int blockW = game->surmap.materials.blockW;
        int blockD = game->surmap.materials.blockD;
        for (int z = (obj->z - obj->depth) / blockD; z < (obj->z + obj->depth) / blockD; ++z)
        for (int y = (obj->y - obj->height) / blockH; y < (obj->y + obj->height) / blockH; ++y)
        for (int x = (obj->x - obj->width) / blockW; x < (obj->x + obj->width) / blockW; ++x) {
            if (manager->chunk.getVoxel(&(game->surmap.world.data),
                                        x, y, z,
                                        game->surmap.world.width,
                                        game->surmap.world.height,
                                        game->surmap.world.depth).iD == Grass)
            {
                if (manager->object.isColision(obj, x*blockW, y*blockH, z*blockD, blockW, blockH, blockD)) {
                    return true;
                }
            }
        }
        return false;
    }

    bool run = true;
    while (run)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                run = false;
            if (event.key.keysym.sym == SDLK_ESCAPE)
                run = false;
            if (manager->windowGL.isFocused(win))
            {
                int mouseX, mouseY;
                static float mouseSpeed = 0.2f;

                SDL_GetGlobalMouseState(&mouseX, &mouseY);
                SDL_WarpMouseGlobal(1366 / 2, 768 / 2);
                float x = mouseSpeed * (float)(1366 / 2 - mouseX);
                float y = mouseSpeed * (float)( 768 / 2 - mouseY);

                manager->cameraGL.rotate(&(game->camera), x, y);
            }

            if (event.key.keysym.sym == SDLK_w)
                manager->cameraGL.move(&(game->camera), cameraMoveForward, game->ltime.deltaTime, 1, 1, 1);
            if (event.key.keysym.sym == SDLK_s)
                manager->cameraGL.move(&(game->camera), cameraMoveBackward, game->ltime.deltaTime, 1, 1, 1);
            if (event.key.keysym.sym == SDLK_a)
                manager->cameraGL.move(&(game->camera), cameraMoveLeft, game->ltime.deltaTime, 1, 1, 1);
            if (event.key.keysym.sym == SDLK_d)
                manager->cameraGL.move(&(game->camera), cameraMoveRight, game->ltime.deltaTime, 1, 1, 1);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
        manager->program.use(&(game->shader));

        manager->mvp.setView(&game->mvp, &(game->camera), &(manager->vector3f), &(manager->matrix));
        // draw
        {
            float iX = ((int)round(game->camera.x) - ((int)round(game->camera.x) % 8)) / 8;
            float iY = ((int)round(game->camera.y) - ((int)round(game->camera.y) % 8)) / 8;
            float iZ = ((int)round(game->camera.z) - ((int)round(game->camera.z) % 8)) / 8;

            manager->surmap.draw(&(game->surmap), iX, iY, iZ, renderVoxels);
        }
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){0,0});
        int fps = manager->time.update(&(game->ltime));
        if (fps != 0) {system("cls"); printf("FPS : %i", fps); }
        manager->windowGL.swap(*win);
    }
}

void gameDelete(struct Game* game, struct GlobalManager* manager)
{
    manager->mvp.delete(&(game->mvp), &(manager->matrix));
    manager->vao.delete(&(game->voxel));
    manager->program.delete(&(game->shader));
    manager->surmap.delete(&(game->surmap), &(manager->voxelInstance), &(manager->chunk));
}
