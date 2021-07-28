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

#include "SDL/Window.h"

#include "GL/Render.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"

#include "Util/matrix.h"

#include "Game/World/Chunk.h"
#include "Game/World/Chunks.h"
#include "Game/World/VoxelInstance.h"
#include "Game/World/VoxelWorld.h"

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
    gameRenderInit(&game, "position", 3, NULL, NULL, cube_position, &globalManager);
    gameVoxelWorldInit(&game, 3, 3, 3, &globalManager);
    gameCameraInit(&game, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, &globalManager);

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

        globalManager.cameraGL.apply(&(game.shader),
                                     &(globalManager.program),
                                     &(game.camera),
                                     "camera_position",
                                     "camera_direction");

        gameDraw(&game, cube_index, 6, "positionInstances", "colorInstances", "u_translatef", &globalManager);
        /*uint32_t index = 0;
        for (uint32_t z = 0; z < game.world.world.chunks.countZ; ++z) {
            for (uint32_t y = 0; y < game.world.world.chunks.countY; ++y) {
                for (uint32_t x = 0; x <  game.world.world.chunks.countX; ++x) {

                    globalManager.program.setVec3f(&(game.shader), "u_translatef", x * 8, y * 8, z * 8);
                    globalManager.program.setVec3fArray(&(game.shader), "positionInstances",  game.world.mesh[index].voxel_size, game.world.mesh[index].positionInstances);
                    globalManager.program.setVec3fArray(&(game.shader), "colorInstances", game.world.mesh[index].voxel_size, game.world.mesh[index].colorInstances);

                    globalManager.render.drawInstanced(&(game.render), GL_TRIANGLES, 6, GL_UNSIGNED_INT, cube_index,game.world.mesh[index].voxel_size);
                    // globalManager.render.draw(&render, GL_TRIANGLES, 6, GL_UNSIGNED_INT, cube_index);
                    ++index;
                }
            }
        }
*/
        globalManager.windowGL.swap(window);
    }

    gameDelete(&game, &globalManager);
    globalManager.windowGL.delete(&window);

    SDL_Quit();
    return 0;
}
