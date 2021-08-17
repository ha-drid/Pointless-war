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
#include "Time.h"
#include "ModelViewProjectionMatrix.h"
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
#include "Game.h"

void setBlockInWorld(unsigned char* image_data,
                     int channels,
                     struct Chunk* worldData,
                     uint32_t width_world,
                     uint32_t height_world,
                     uint32_t depth_world,
                     struct ChunkManager* manage) {
    uint32_t index = 0;
    for(int z = 0; z < depth_world; ++z)
        for(int x = 0; x < width_world; ++x) {
            int c = image_data[index * channels] / 15;

            for(int y = 0; y < c; ++y) {
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

    struct Game game;
    gameInit(&game, &globalManager);
    gameLoop(&game, &window, &globalManager);
    gameDelete(&game, &globalManager);

    globalManager.windowGL.delete(&window);

    SDL_Quit();
    return 0;
}
