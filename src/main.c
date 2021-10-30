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

#include "main.h"
#include "Utility.h"
#include "Time.h"
#include "SDL/Window.h"

#include "GL/Program.h"
#include "GL/CameraGL.h"


#include "Game/Units/Inventory.h"
#include "Game/Voxels/Chunk.h"
#include "Game/Voxels/VoxelInstance.h"
#include "Game.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    srand(time(NULL));
    SDL_Init(SDL_INIT_EVERYTHING);

    const struct GlobalManager globalManager = globalManagerInit();
    struct WindowGL window;
    globalManager.windowGL.init(&window, "Game", 640, 480, SDL_WINDOW_RESIZABLE);

    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    struct Game game;

    gameInit(&game, &globalManager);
    gameLoop(&game, &window, &globalManager);
    gameDelete(&game, &globalManager);

    globalManager.windowGL.delete(&window);

    SDL_Quit();
    return 0;
}
