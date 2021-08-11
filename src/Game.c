#include "Game.h"

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
    manager->cameraGL.init(&(game->camera), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 2.5f);

    manager->surmap.init(&(game->surmap), "res\\img\\heightmap.png", 8, 8, 8, setBlockInWorld, &(manager->chunk));

    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\undefined.bmp", &(manager->voxelInstance));
    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\grass.bmp", &(manager->voxelInstance));
    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\Stone.bmp", &(manager->voxelInstance));
    manager->surmap.addLoadMaterial(&(game->surmap), "res\\img\\dirt.bmp", &(manager->voxelInstance));

    manager->surmap.setRenderDistance(&(game->surmap), 30, 30, 30);
}

void gameLoop(struct Game* game, struct WindowGL* win, struct GlobalManager* manager)
{
    struct Program shader;
    manager->program.init(&shader, "res\\glsl\\first.vert", "res\\glsl\\first.frag");
    manager->program.link(&shader);
    manager->program.use(&shader);
    struct Render render;
    manager->render.init(&render);
    manager->render.addAttribute(&render, &shader, &(manager->program),
                                 "position", 3, 0, 0, CUBE_POSITION);
    struct MVP mvp;
    manager->mvp.init(&mvp,&shader,"u_projection","u_view","u_model",&(manager->program),&(manager->matrix));
    manager->mvp.setProjection(&mvp, radians_s(90.0f), 5.0f / 3.0f, 0.1, 200.0f, &(manager->matrix));

    void renderVoxels(struct VoxelInstance* mesh, float x, float y, float z) {
        manager->matrix.translate3f(&(mvp.model), x, y, z);
        manager->mvp.modelShaderPush(&mvp);
        manager->program.setVec3fArray(&shader, "positionInstances", mesh->voxel_size, mesh->positionInstances);
        manager->program.setVec3fArray(&shader, "colorInstances", mesh->voxel_size, mesh->colorInstances);
        manager->render.drawInstanced(&render, GL_TRIANGLES, 36, GL_UNSIGNED_INT, CUBE_INDEX, mesh->voxel_size);
    }

    void renderVoxel(float x, float y, float z) {
        manager->matrix.translate3f(&(mvp.model), x, y, z);
        manager->mvp.modelShaderPush(&mvp);
        manager->program.setVec3fArray(&shader, "colorInstances", 1, VoxelColor[Stone]);
        manager->render.draw(&render, GL_TRIANGLES, 36, GL_UNSIGNED_INT, CUBE_INDEX);
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

    struct Object obj;
    manager->object.init(&obj, 0.0f, 60.0f, 0.0f, 5.0f, 5.0f, 5.0f, 0.0f, 0.0f, 2.50f);

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
                manager->cameraGL.move(&(game->camera), cameraMoveForward, 1, 1, 1);
            if (event.key.keysym.sym == SDLK_s)
                manager->cameraGL.move(&(game->camera), cameraMoveBackward, 1, 1, 1);
            if (event.key.keysym.sym == SDLK_a)
                manager->cameraGL.move(&(game->camera), cameraMoveLeft, 1, 1, 1);
            if (event.key.keysym.sym == SDLK_d)
                manager->cameraGL.move(&(game->camera), cameraMoveRight, 1, 1, 1);

            if (event.key.keysym.sym == SDLK_UP) {
                manager->object.move(&obj, objectMoveForward, 1, 0, 1);
                manager->cameraGL.move(&(game->camera), cameraMoveForward, 1, 0, 1);
            }
            if (event.key.keysym.sym == SDLK_DOWN) {
                manager->object.move(&obj, objectMoveBackward, 1, 0, 1);
                manager->cameraGL.move(&(game->camera), cameraMoveBackward, 1, 0, 1);
            }
            if (event.key.keysym.sym == SDLK_RIGHT) {
                manager->object.move(&obj, objectMoveRight, 1, 0, 1);
                manager->cameraGL.move(&(game->camera), cameraMoveRight, 1, 0, 1);
            }
            if (event.key.keysym.sym == SDLK_LEFT) {
                manager->object.move(&obj, objectMoveLeft, 1, 0, 1);
                manager->cameraGL.move(&(game->camera), cameraMoveLeft, 1, 0, 1);
            }
            if ((event.key.keysym.sym == SDLK_SPACE) && (obj.isOnGround))
                manager->object.vertMove(&obj, 2.5f, game->ltime.deltaTime);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

        manager->mvp.setView(&mvp, &(game->camera), &(manager->vector3f), &(manager->matrix));

        float iX = ((int)round(game->camera.x) - ((int)round(game->camera.x) % 8)) / 8;
        float iY = ((int)round(game->camera.y) - ((int)round(game->camera.y) % 8)) / 8;
        float iZ = ((int)round(game->camera.z) - ((int)round(game->camera.z) % 8)) / 8;

        manager->surmap.draw(&(game->surmap), iX, iY, iZ, renderVoxels);
        manager->object.draw(&obj, renderVoxel);
        manager->object.vertMove(&obj, -0.05f, game->ltime.deltaTime);
        manager->cameraGL.update(&(game->camera), game->ltime.deltaTime);

        manager->object.setAngle(&obj, game->camera.xAngle, game->camera.yAngle);


        manager->windowGL.swap(*win);
        manager->object.update(&obj, game->ltime.deltaTime, objectWorldIsColision);

        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){0,0});
        manager->time.update(&(game->ltime));
        printf("%i\n%f\n", (int)obj.isOnGround, obj.yVelocity);
    }
    manager->mvp.delete(&mvp, &(manager->matrix));
    manager->render.delete(&render);
    manager->program.delete(&shader);
}

void gameDelete(struct Game* game, struct GlobalManager* manager)
{
    manager->surmap.delete(&(game->surmap), &(manager->voxelInstance), &(manager->chunk));
    //manager->mvp.delete(&(game->mvp), &(manager->matrix));
}
