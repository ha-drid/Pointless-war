/**<
      Пока что создаеться движок и здесь что то интересного нет. Пока что тестируеться создание мира с помошью карт высот.

      Как вы наверное заметили или нет, в этом проекте каждый раз когда создаеться структура нужно создать структуру которая
    будет управлят структурами этого типа.
    Пример:
    struct MyStruct{
        int i;
    };

    struct MyStructManager {
        void (*method)(struct MyStruct* value);
    };

    struct MyStructManager manager;

    ... структура manager инициализируеться

    struct MyStruct a, b, c;
    manager.method(&a);
    manager.method(&b);
    manager.method(&c);

    Это делаеться для того чтобы быстро находит то что тебе надо.
    Ведь ты просто ставиш точку и IDE сама показывает все методы.
    В файле Utility.h есть стуктура которая хранить эти все управляюшие структуры.
*/

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
#include "World.h"
#include "Time.h"
#include "MVP.h"
#include "SDL/Window.h"

#include "GL/Render.h"
#include "GL/Program.h"
#include "GL/CameraGL.h"

#include "Util/matrix.h"

#include "Game/Objects/Object.h"
#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/Chunks.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Game/VoxelWorld.h"
#include "Game/Surmap.h"

void setBlockInWorld(unsigned char* image_data,
                     int channels,
                     struct Chunk* worldData,
                     uint32_t width_world,
                     uint32_t height_world,
                     uint32_t depth_world,
                     struct ChunkManager* manage) {
    uint32_t index = 0;
    for(int z = 0; z < depth_world; ++z)
        for(int x = 0; x < width_world; ++x)
        {
            int c = image_data[index * channels] / 15;

            for(int y = 0; y < c; ++y)
            {
                int iZ = z * (width_world * height_world);
                int iY = y * width_world;

                if (y + 1 == c)
                    worldData->voxels[iZ + iY + x].iD = Grass;
               /* else if (y + 2 == c)
                    worldData->voxels[iZ + iY + x].iD = Dirt;
                else
                    worldData->voxels[iZ + iY + x].iD = Stone;
                    */
            }
            ++index;
        }
}


int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);

    struct GlobalManager globalManager = globalManagerInit();

    struct WindowGL window;
    globalManager.windowGL.init(&window, "Game", 640, 480, SDL_WINDOW_RESIZABLE);
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
    globalManager.mvp.init(&mvp,&shader, "u_projection", "u_view", "u_model", &(globalManager.program), &(globalManager.matrix));
    globalManager.mvp.setProjection(&mvp, radians_s(90.0f), 5.0f / 3.0f, 0.1, 200.0f, &(globalManager.matrix));

    struct Surmap world;
    globalManager.surmap.init(&world, "res\\img\\heightmap.png", setBlockInWorld, &(globalManager.chunk));
    globalManager.surmap.addLoadMaterial(&world, "res\\img\\undefined.bmp", 8, 8, 8, &(globalManager.voxelInstance));
    globalManager.surmap.addLoadMaterial(&world, "res\\img\\grass.bmp", 8, 8, 8, &(globalManager.voxelInstance));
    globalManager.surmap.addLoadMaterial(&world, "res\\img\\Stone.bmp", 8, 8, 8, &(globalManager.voxelInstance));
    globalManager.surmap.addLoadMaterial(&world, "res\\img\\dirt.bmp", 8, 8, 8, &(globalManager.voxelInstance));

    globalManager.surmap.setRenderDistance(&world, 30, 30, 30);

    struct Time ltime;
    globalManager.time.init(&ltime);

    void renderVoxels(struct VoxelInstance* mesh, float x, float y, float z) {
        globalManager.matrix.translate3f(&(mvp.model), x, y, z);
        globalManager.mvp.modelShaderPush(&mvp);
        globalManager.program.setVec3fArray(&shader, "positionInstances", mesh->voxel_size, mesh->positionInstances);
        globalManager.program.setVec3fArray(&shader, "colorInstances", mesh->voxel_size, mesh->colorInstances);
        globalManager.render.drawInstanced(&render, GL_TRIANGLES, 6, GL_UNSIGNED_INT, VOXEL_INDEX, mesh->voxel_size);
    }

    void renderVoxel(float x, float y, float z) {
        globalManager.matrix.translate3f(&(mvp.model), x, y, z);
        globalManager.mvp.modelShaderPush(&mvp);
        globalManager.program.setVec3fArray(&shader, "colorInstances", 1, VoxelColor[Stone]);
        globalManager.render.draw(&render, GL_TRIANGLES, 6, GL_UNSIGNED_INT, VOXEL_INDEX);
    }

    struct Object obj;
    globalManager.object.init(&obj, 0.0f, 60.0f, 0.0f, 5.0f, 5.0f, 5.0f);

    bool run = true;
    while (run)
    {
        system("cls");
        globalManager.time.update(&ltime);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                run = false;

            if (event.key.keysym.sym == SDLK_ESCAPE)
                run = false;

            if (globalManager.windowGL.isFocused(&window))
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

        float iX = ((int)round(camera.x) - ((int)round(camera.x) % 8)) / 8;
        float iY = ((int)round(camera.y) - ((int)round(camera.y) % 8)) / 8;
        float iZ = ((int)round(camera.z) - ((int)round(camera.z) % 8)) / 8;

        globalManager.surmap.draw(&world, iX, iY, iZ, renderVoxels);
        globalManager.object.draw(&obj, renderVoxel);

        globalManager.windowGL.swap(window);
    }

    globalManager.surmap.delete(&world, &(globalManager.voxelInstance), &(globalManager.chunk));
    globalManager.render.delete(&render);
    globalManager.mvp.delete(&mvp, &(globalManager.matrix));
    globalManager.program.delete(&shader);
    globalManager.windowGL.delete(&window);

    SDL_Quit();
    return 0;
}
