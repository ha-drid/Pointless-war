#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <windows.h>
#include <glad/glad.h>
#include <GL/glu.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "main.h"
#include "Utility.h"
#include "Game.h"
#include "World.h"

#include "MVP.h"

#include "SDL/Window.h"

#include "GL/Render.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"

#include "Util/matrix.h"

#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/Chunks.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Game/VoxelWorld.h"
#include "Game/Surmap.h"
#include "Game/Entity.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);

    struct GlobalManager globalManager = globalManagerInit();

    struct Chunk grass_chunk;
    globalManager.chunk.init(&grass_chunk, 8, 8, 8);
    globalManager.chunk.fill(&grass_chunk, 8, 8, 8, (struct Voxel){ Dirt });
    for (uint32_t z = 0; z < 8; ++z)
        for (uint32_t x = 0; x < 8; ++x)
        {
            globalManager.chunk.setVoxel(&grass_chunk, x, 7, z, 8, 8, 8, (struct Voxel){ Grass });
            globalManager.chunk.setVoxel(&grass_chunk, x, 6, z, 8, 8, 8, (struct Voxel){ Grass });
        }


    struct WindowGL window;
    globalManager.windowGL.init(&window, "Game", 640, 480,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

    struct Program shader;
    globalManager.program.init(&shader, "res\\glsl\\first.vert", "res\\glsl\\first.frag");
    globalManager.program.link(&shader);
    globalManager.program.use(&shader);

    struct Render render;
    globalManager.render.init(&render);
    globalManager.render.addAttribute(&render, &shader, &(globalManager.program), "position", 3, NULL, NULL, VOXEL_POSITION);

    struct CameraGL camera;
    globalManager.cameraGL.init(&camera, 0, 0, -5, 0, 0, 1.0f);

    struct MVP mvp;
    globalManager.mvp.init(&mvp, &(globalManager.matrix));
    globalManager.mvp.setProjection(&mvp, radians_s(90.0f), 5.0f / 3.0f, 0.1, 200.0f, &(globalManager.matrix));
    globalManager.program.setMat4fv(&(shader), "u_projection", 1, GL_FALSE, mvp.projection.data);

    struct Surmap world;
    surmapInit(&world, "res\\img\\heightmap.png");

    //surmapAddMaterial(&world, &grass_chunk, 8, 8, 8, voxelGetColor, &(globalManager.voxelInstance));
    surmapAddLoadMaterial(&world, "res\\img\\grass.bmp", 8, 8, 8, &(globalManager.voxelInstance));
    surmapAddLoadMaterial(&world, "res\\img\\Stone.bmp", 8, 8, 8, &(globalManager.voxelInstance));
    surmapSetRenderDistance(&world, 30, 30);

    void renderVoxels(struct VoxelInstance* mesh, float x, float y, float z)
    {
        globalManager.matrix.translate3f(&(mvp.model), x, y, z);

        globalManager.program.setMat4fv(&shader, "u_model", 1, GL_FALSE, mvp.model.data);

        globalManager.program.setVec3fArray(&shader, "positionInstances", mesh->voxel_size, mesh->positionInstances);
        globalManager.program.setVec3fArray(&shader, "colorInstances", mesh->voxel_size, mesh->colorInstances);

        globalManager.render.drawInstanced(&render, GL_TRIANGLES, 6, GL_UNSIGNED_INT, VOXEL_INDEX, mesh->voxel_size);
    }

    bool run = true;
    while (run)
    {
        system("cls");

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                run = false;

            if (event.key.keysym.sym == SDLK_ESCAPE)
                run = false;

            if (SDL_GetGrabbedWindow() != window.window)
            {
                  int mouseX, mouseY;
                    static float mouseSpeed = 0.2f;

                    SDL_GetGlobalMouseState(&mouseX, &mouseY);
                    SDL_WarpMouseGlobal(1366 / 2, 768 / 2);
                    float x = mouseSpeed * (float)(1366 / 2 - mouseX);
                    float y = mouseSpeed * (float)( 768 / 2 - mouseY);

                    globalManager.cameraGL.rotate(&(camera), x, y);
            }

            if (event.key.keysym.sym == SDLK_w)
                globalManager.cameraGL.move(&(camera), CAMERA_FORWARD);
            if (event.key.keysym.sym == SDLK_s)
                globalManager.cameraGL.move(&(camera), CAMERA_BACKWARD);
            if (event.key.keysym.sym == SDLK_a)
                globalManager.cameraGL.move(&(camera), CAMERA_LEFT);
            if (event.key.keysym.sym == SDLK_d)
                globalManager.cameraGL.move(&(camera), CAMERA_RIGHT);

        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);

        globalManager.mvp.setView(&mvp, &(camera), &(globalManager.vector3f), &(globalManager.matrix));
        globalManager.program.setMat4fv(&(shader), "u_view", 1, GL_FALSE, mvp.view.data);

        float iX = ((int)round(camera.x) - ((int)round(camera.x) % 8)) / 8;
        float iZ = ((int)round(camera.z) - ((int)round(camera.z) % 8)) / 8;

        surmapDraw(&world, iX, iZ, renderVoxels);

        globalManager.windowGL.swap(window);
    }
    surmapDelete(&world, &(globalManager.voxelInstance));
    globalManager.render.delete(&render);
    globalManager.mvp.delete(&mvp, &(globalManager.matrix));
    globalManager.program.delete(&shader);
    globalManager.windowGL.delete(&window);

    SDL_Quit();
    return 0;
}
