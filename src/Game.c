#include "Game.h"
#include <clist/list.h>
#include <cbitset/bitset.h>
#include <inttypes.h>
#include <cglm/cglm.h>
#include "Game/Units/Inventory.h"
#include "Game/Units/Item.h"
#include "Gui/Menu.h"

#define GUI_WINDOW_WIDTH 30
#define GUI_WINDOW_HEIGHT 30

void gameInit(struct Game* game, struct GlobalManager* manager)
{
    manager->time.init(&(game->ltime));

    vec3 vertex[8] = {
        { 0.0f, 0.0f, 0.0f },
        { 0.0f, 1.0f, 0.0f },
        { 1.0f, 1.0f, 0.0f },
        { 1.0f, 0.0f, 0.0f },

        { 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 1.0f },
        { 1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f },
    };
    const static float charSize = 1.0f / 16.0f;

    vec2 uv[8] = {
        { charSize, charSize },
        { charSize, 0.0f },
        { 0.0f, 0.0f },
        { 0.0f, charSize },

        { 0.0f, charSize },
        { 0.0f, 0.0f },
        { charSize, 0.0f },
        { charSize, charSize },

    };

    GLuint indices[6 * 6] = {
        0, 1, 2,
        0, 3, 2,

        4, 5, 6,
        4, 7, 6,

        1, 2, 6,
        1, 5, 6,

        3, 2, 7,
        2, 7, 6,

        0, 1, 4,
        5, 4, 1,

        0, 3, 7,
        4, 7, 0,
    };

    manager->canvas.init(&(game->gameCanvas), 8, vertex, uv, 36, indices, "res\\img\\font.bmp", "res\\glsl\\first.vert", "res\\glsl\\first.frag");
    stbi_set_flip_vertically_on_load(1);

    manager->canvas.lookAt(&(game->gameCanvas), 0.0f, 0.0f, 1.0f,
                                     0.0f, 0.0f, 0.0f,
                                     0.0f, 1.0f, 0.0f);

    manager->canvas.perspective(&(game->gameCanvas), glm_rad(120.0f), 5.0f / 3.0f, 0.1f, 100);
}

void gameLoop(struct Game* game, struct WindowGL* const win, struct GlobalManager* manager)
{
    void renderAsciiVoxelInstanced(VoxelInstance* const mesh, float x, float y, float z) {
        manager->canvas.drawInstanced(&(game->gameCanvas), x, y, z, mesh);
    }

    const static float charSize = 1.0f / 16.0f;
    void renderVoxelSym(float x, float y, float z, char sym) {
        int iX = sym & 0b1111;
        int iY = sym >> 4;
        manager->canvas.draw(&(game->gameCanvas), x, y, z, iX * charSize, iY * charSize);
    }

    void objectDrawAscii(void* const value) {
        Object* o = value;
        for (int z = o->position[2]; z < (o->size[2] + o->position[2]); ++z)
            for (int y = o->position[1]; y < (o->size[1] + o->position[1]); ++y)
                for (int x = o->position[0]; x < (o->size[0] + o->position[0]); ++x) {
                    renderVoxelSym(x, y, z, '@');
                }
    }

    void slotDraw(int x, int y, Slot sl) {
        int iX = sl.iD & 0b1111;
        int iY = sl.iD >> 4;
    }

    void itemDrawAscii(void* const value) {
        Item* o = value;
        float* size;
        itemGetSize(value, &size);
        for (int z = o->position[2]; z < (size[2] + o->position[2]); ++z)
            for (int y = o->position[1]; y < (size[1] + o->position[1]); ++y)
                for (int x = o->position[0]; x < (size[0] + o->position[0]); ++x) {
                    renderVoxelSym(x, y, z, 'I');
                }
    }


    manager->mainPlayer.init(&(game->player), (vec3){ 0.0f, 0.0f, -5.0f }, (vec3){ 0.5f, 1.7f, 0.5f }, 4.0f);

    const GLfloat GRAVITY = 0.05f;

    AsciiWorld world;
    manager->asciiWorld.init(&world, 10, 1, 10);

    for (uint32_t z = 0; z < world.depth; ++z)
        for (uint32_t y = 0; y < world.height; ++y)
            for (uint32_t x = 0; x < world.width; ++x)
                manager->chunk.fill(&(world.data[z][y][x]), (Voxel){ rand() });

    manager->asciiWorld.update(&world);

    Objects objs;
    manager->objects.init(&objs, &objectGetPosition, &objectGetVelocity, &objectGetSize, &objectGetSpeed, &objectGetIsOnGround);

    manager->objects.newObject(&objs, createObject(0.0f, 30.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f));
    manager->objects.newObject(&objs, createObject(30.0f, 30.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f));
    manager->objects.newObject(&objs, createObject(60.0f, 30.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f));

    Objects items;
    manager->objects.init(&items, &itemGetPosition, &itemGetVelocity, &itemGetSize, &itemGetSpeed, &itemGetIsOnGround);

    manager->objects.newObject(&items, createItem(10.0f, 100.0f, 10.0f, ItemSwordEnum));
    manager->objects.newObject(&items, createItem(30.0f, 100.0f, 10.0f, ItemPickEnum));
    manager->objects.newObject(&items, createItem(50.0f, 100.0f, 10.0f, ItemTest1Enum));

    Inventory(10, 10) bag;
    inventoryClear(&bag, 10, 10);
    bool inventoryOpen = false;

    bag.data[0][0].iD = 0;
    bag.data[0][1].iD = 1;
    bag.data[0][2].iD = 2;
    bag.data[0][3].iD = 3;

    Menu menu;
    manager->menu.init(&menu, "res\\glsl\\second.vert", "res\\glsl\\second.frag");
    manager->menu.setWindowSize(&menu, 30, 30);
    manager->menu.addButton(&menu, 0.0f, 0.0f, 1.0f, 1.0f, 0, 0, 16, 16, "res\\img\\sword.png");

    SDL_WarpMouseGlobal(1366 / 2, 768 / 2);
    bool run = true;
    while (run) {
        manager->time.update(&(game->ltime));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

        static bool x = 1;
        static bool y = 1;
        static bool z = 1;

        const uint8_t* const key = SDL_GetKeyboardState(NULL);
        SDL_Event event;
        SDL_PumpEvents();
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                run = false;

            if (inventoryOpen)
            {
                if (key[SDL_SCANCODE_ESCAPE])
                    inventoryOpen = false;
                int window_width, window_height;
                manager->windowGL.getSize(win, &window_width, &window_height);
                float ButtonSizeY = (float)window_height / GUI_WINDOW_HEIGHT;
                float ButtonSizeX = (float)window_width / GUI_WINDOW_WIDTH;

                int mouseX, mouseY;

                SDL_GetMouseState(&mouseX, &mouseY);

                system("cls");
                if (event.button.button == SDL_BUTTON_LEFT) {
                    for (int y = 0; y < GUI_WINDOW_HEIGHT; ++y)
                        for (int x = 0; x < GUI_WINDOW_WIDTH; ++x) {
                            float iY = y * ButtonSizeY;
                            float iX = x * ButtonSizeX;
                            if (isColision(iX, iY, 0.0f, ButtonSizeX, ButtonSizeY, 1.0f,
                                       mouseX, mouseY, 0.0f, 0.1f, 0.1f, 0.1f))
                            {
                                printf("%i %i\n", x, y);
                            }
                        }
                }
            }
            else
            {
                if (manager->windowGL.isFocused(win))
                {
                    int mouseX, mouseY;
                    static float mouseSpeed = 0.1f;

                    SDL_GetGlobalMouseState(&mouseX, &mouseY);
                    SDL_WarpMouseGlobal(1366 / 2, 768 / 2);
                    float x = mouseSpeed * (float)(1366 / 2 - mouseX);
                    float y = mouseSpeed * (float)( 768 / 2 - mouseY);

                    manager->cameraGL.rotate(&(game->player.camera), x, y);
                }

                if (event.button.button == SDL_BUTTON_LEFT) {
                    vec3 direction;
                    manager->cameraGL.getDirection(&(game->player.camera), direction);
                    glm_vec3_divs(direction, 10.0f, direction);

                    vec3 bulletPos;
                    bulletPos[0] = game->player.camera.position[0] + (game->player.size[0] / 2.0f);
                    bulletPos[1] = game->player.camera.position[1] + (game->player.size[1]);
                    bulletPos[2] = game->player.camera.position[2] + (game->player.size[2] / 2.0f);

                    int distance = 0;
                    while (distance < 40) {
                        int iX = (int)(bulletPos[0]);
                        int iY = (int)(bulletPos[1]);
                        int iZ = (int)(bulletPos[2]);
                        if (manager->asciiWorld.getVoxel(&world, iX, iY, iZ).iD != 0) {
                            if (isColision(iX, iY, iZ, 1.0f, 1.0f, 1.0f,
                                        bulletPos[0], bulletPos[1], bulletPos[2], 0.5f, 0.5f, 0.5f)) {

                                manager->asciiWorld.setVoxel(&world, iX, iY, iZ, (Voxel){ NULL });
                                manager->asciiWorld.update(&world);
                                break;
                            }
                        }

                        glm_vec3_add(direction, bulletPos, bulletPos);

                        ++distance;
                    }
                }
                if (key[SDL_SCANCODE_ESCAPE])
                    run = false;
                if (key[SDL_SCANCODE_1])
                    y = true;
                if (key[SDL_SCANCODE_0])
                    y = false;
            }

            if (key[SDL_SCANCODE_I]) {
                inventoryOpen = !inventoryOpen;
                SDL_Delay(300);
            }
        }


        vec3 direction, position, up;
        manager->mainPlayer.getView(&(game->player), position, direction, up);
        manager->canvas.lookAt(&(game->gameCanvas), position[0], position[1], position[2],
                                          direction[0],direction[1], direction[2],
                                          up[0], up[1], up[2]);
        // draw
        {
            manager->objects.draw(&objs, &objectDrawAscii);
            manager->objects.draw(&items, &itemDrawAscii);
            manager->asciiWorld.draw(&world, &renderAsciiVoxelInstanced);
        }

        if (inventoryOpen)
        {
            manager->menu.draw(&menu);
        }
        else {
            manager->mainPlayer.control(&(game->player), win, x, y, z);

            if (key[SDL_SCANCODE_SPACE] && game->player.isOnGround)
                manager->mainPlayer.vertMove(&(game->player), GRAVITY * 100);
        }

        {
            manager->mainPlayer.update(&(game->player), game->ltime.deltaTime * 0.001f, &world, &(manager->asciiWorld));
            manager->objects.update(&objs, game->ltime.deltaTime / 1000.0f, &world, &(manager->asciiWorld));
            manager->objects.update(&items, game->ltime.deltaTime * 0.001f, &world, &(manager->asciiWorld));

        }
        {
            manager->objects.vertMove(&objs, -GRAVITY);
            manager->objects.vertMove(&items, -GRAVITY);

            if (y == 0)
                manager->mainPlayer.vertMove(&(game->player), -GRAVITY);
            else
                game->player.camera.velocity[1] = 0.0f;
        }

        manager->time.frameEnded(&(game->ltime));

        manager->windowGL.swap(win);
    }
    manager->menu.delete(&menu);
    manager->objects.delete(&objs);
    manager->objects.delete(&items);
    manager->asciiWorld.delete(&world);
}

void gameDelete(struct Game* game, struct GlobalManager* manager)
{
    manager->canvas.delete(&(game->gameCanvas));
}

