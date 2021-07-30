#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <windows.h>
#include <glad/glad.h>
#include <GL/glu.h>

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

#include "Game/Entity.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);

    struct GlobalManager globalManager = globalManagerInit();

    struct WindowGL window;
    globalManager.windowGL.init(&window, "Game", 640, 480,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    gladLoadGL();

    struct Game game;
    gameProgramInit(&game, "res\\glsl\\first.vert", "res\\glsl\\first.frag", &globalManager);
    gameRenderInit(&game, "position", 3, NULL, NULL, VOXEL_POSITION, &globalManager);
    gameVoxelWorldInit(&game, 50, 10, 50, &globalManager);
    gameCameraInit(&game, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, &globalManager);
    gameDrawingDistance(&game, 20.0f, 10.0f, 20.0f);

    struct MVP mvp;
    mvpInit(&mvp);
    mvpSetProjection(&mvp, radians_s(45.0f), 4.0f / 3.0f, 0.1, 150.0f);
    globalManager.program.setMat4fv(&(game.shader), "u_projection", 1, GL_FALSE, mvp.projection.data);

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

            if (SDL_GetGrabbedWindow() != window.window)
            {
                  int mouseX, mouseY;
                    static float mouseSpeed = 0.2f;

                    SDL_GetGlobalMouseState(&mouseX, &mouseY);
                    SDL_WarpMouseGlobal(1366 / 2, 768 / 2);
                    float x = mouseSpeed * (float)(1366 / 2 - mouseX);
                    float y = mouseSpeed * (float)( 768 / 2 - mouseY);

                    globalManager.cameraGL.rotate(&(game.camera), x, y);
            }

            if (event.key.keysym.sym == SDLK_w)
                globalManager.cameraGL.move(&(game.camera), CAMERA_FORWARD);
            if (event.key.keysym.sym == SDLK_s)
                globalManager.cameraGL.move(&(game.camera), CAMERA_BACKWARD);
            if (event.key.keysym.sym == SDLK_a)
                globalManager.cameraGL.move(&(game.camera), CAMERA_LEFT);
            if (event.key.keysym.sym == SDLK_d)
                globalManager.cameraGL.move(&(game.camera), CAMERA_RIGHT);

        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
/*
        globalManager.cameraGL.apply(&(game.shader),
                                     &(globalManager.program),
                                     &(game.camera),
                                     "u_view",
                                     "camera_direction");
*/
        mvpSetView(&mvp, &(game.camera), &(globalManager.vector3f));
        globalManager.program.setMat4fv(&(game.shader), "u_view", 1, GL_FALSE, mvp.view.data);

        gameDraw(&game, VOXEL_INDEX, 6, &(mvp.model), "positionInstances", "colorInstances", "u_model", &globalManager);

        globalManager.windowGL.swap(window);
    }
    mvpDelete(&mvp);
    gameDelete(&game, &globalManager);
    globalManager.windowGL.delete(&window);

    SDL_Quit();
    return 0;
}
