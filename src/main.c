/**<
      ���� ��� ���������� ������ � ����� ��� �� ����������� ���. ���� ��� ������������ �������� ���� � ������� ���� �����.

      ��� �� �������� �������� ��� ���, � ���� ������� ������ ��� ����� ���������� ��������� ����� ������� ��������� �������
    ����� �������� ����������� ����� ����.
    ������:
    struct MyStruct{
        int i;
    };

    struct MyStructManager {
        void (*method)(struct MyStruct* value);
    };

    struct MyStructManager manager;

    ... ��������� manager �����������������

    struct MyStruct a, b, c;
    manager.method(&a);
    manager.method(&b);
    manager.method(&c);

    ��� ��������� ��� ���� ����� ������ ������� �� ��� ���� ����.
    ���� �� ������ ������ ����� � IDE ���� ���������� ��� ������.
    � ����� Utility.h ���� �������� ������� ������� ��� ��� ����������� ���������.
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
