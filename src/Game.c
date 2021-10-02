#include "Game.h"
#include <clist/list.h>
#include <cbitset/bitset.h>
#include <inttypes.h>
#include <cglm/cglm.h>
#include "Game/Units/Inventory.h"
#include "Game/Units/Item.h"

void gameInit(struct Game* const game, struct GlobalManager* const manager)
{
    manager->time.init(&(game->ltime));

    manager->program.init(&(game->shader), "res\\glsl\\first.vert", "res\\glsl\\first.frag");
    manager->program.link(&(game->shader));
    manager->program.use(&(game->shader));

    manager->vao.init(&(game->voxel));

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

    manager->vao.addVertexBufferObject(&(game->voxel), 3, 3 * 8, vertex);
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

    manager->vao.addVertexBufferObject(&(game->voxel), 2, 2 * 8, uv);

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

    manager->vao.addIndices(&(game->voxel), 6 * 6, indices);
}

void gameLoop(struct Game* const game, struct WindowGL* const win, struct GlobalManager* const manager)
{
    mat4 model;
    memset(model, 0, sizeof(mat4));
    model[0][0] = 1.0f; model[1][1] = 1.0f; model[2][2] = 1.0f; model[3][3] = 1.0f;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    void renderAsciiVoxelInstanced(VoxelInstance* const mesh, float x, float y, float z) {
        model[3][0] = x; model[3][1] = y; model[3][2] = z;
        manager->program.setMat4fv(&(game->shader), "u_Model", 1, GL_FALSE, model);
        manager->program.setVec3fArray(&(game->shader), "positionInstances", mesh->voxelSize, mesh->buffer.data[0].data);
        manager->program.setVec2fArray(&(game->shader), "uvInstances", mesh->voxelSize, mesh->buffer.data[1].data);
        manager->vao.drawElementsInstanced(&(game->voxel), GL_TRIANGLES, mesh->voxelSize);
    }

    void renderVoxelSym(float x, float y, float z, char sym) {
        manager->program.use(&(game->shader));
        if (sym == 0) return;
        model[3][0] = x; model[3][1] = y; model[3][2] = z;
        manager->program.setMat4fv(&(game->shader), "u_Model", 1, GL_FALSE, model);
        const static float charSize = 1.0f / 16.0f;
        int iX = sym & 0b1111;
        int iY = sym >> 4;
        manager->program.setVec2fArray(&(game->shader), "uvInstances", 1, (float[]){ charSize * iX, charSize * iY });
        manager->program.setVec3fArray(&(game->shader), "positionInstances", 1, (float[]){ 0.0f, 0.0f, 0.0f });
        manager->vao.drawElementsInstanced(&(game->voxel), GL_TRIANGLES, 1);
    }

    void objectDrawAscii(void* const value) {
        Object* o = value;
        for (int z = o->position[2]; z < (o->size[2] + o->position[2]); ++z)
            for (int y = o->position[1]; y < (o->size[1] + o->position[1]); ++y)
                for (int x = o->position[0]; x < (o->size[0] + o->position[0]); ++x) {
                    renderVoxelSym(x, y, z, '@');
                }
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
    manager->mainPlayer.perspective(&(game->player), glm_rad(70.0f), 5.0f / 4.0f, 0.1f, 200.0f);

    const GLfloat GRAVITY = 0.05f;

    Texture tex;
    manager->texture.init(&tex, GL_NEAREST, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    {
        Image img;
            manager->image.init(&img, "res\\img\\font.bmp");
            manager->texture.setImage(&tex, img.width, img.height, img.channels, (uint8_t*)img.data);
        manager->image.delete(&img);
    }

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

    bag.data[0][0].iD = ItemSwordEnum;

    Screen iscreen;
    manager->screen.init(&iscreen, 640, 480, "res\\glsl\\second.vert", "res\\glsl\\second.frag");

    InventoryIcons itemIcon;
    manager->inventoryIcon.init(&itemIcon);
    manager->inventoryIcon.addIcon(&itemIcon, "res\\img\\slot.png");
    manager->inventoryIcon.addIcon(&itemIcon, "res\\img\\sword.png");

    void slotDraw(int x, int y, Slot sl) {
        switch (sl.iD)
        {
        case (ItemSwordEnum):
            manager->screen.image(&iscreen, x * 16, y * 16, &(itemIcon.icons.data[1]));
            break;
        default:
            manager->screen.image(&iscreen, x * 16, y * 16, &(itemIcon.icons.data[0]));
            break;
        }
    }

    SDL_WarpMouseGlobal(1366 / 2, 768 / 2);
    bool run = true;
    while (run) {
        manager->time.update(&(game->ltime));

        static bool x = 1;
        static bool y = 1;
        static bool z = 1;

        const uint8_t* const key = SDL_GetKeyboardState(NULL);
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                run = false;

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

            if (key[SDL_SCANCODE_UP])
                manager->cameraGL.rotate(&(game->player.camera), 0.0f, 3.0f);
            if (key[SDL_SCANCODE_DOWN])
                manager->cameraGL.rotate(&(game->player.camera), 0.0f, -3.0f);
            if (key[SDL_SCANCODE_LEFT])
                manager->cameraGL.rotate(&(game->player.camera), 3.0f, 0.0f);
            if (key[SDL_SCANCODE_RIGHT])
                manager->cameraGL.rotate(&(game->player.camera), -3.0f, 0.0f);
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

        manager->screen.clear(&iscreen, 255, 255, 255, 0);

        manager->screen.set(&iscreen, 0, 0, 255, 0, 0, 200);
        manager->program.use(&(game->shader));
        manager->mainPlayer.apply(&(game->player), &(game->shader), "u_ViewProjection", &(manager->program));

        // draw
        {
            manager->program.use(&(game->shader));
            manager->texture.bind(&tex);
            manager->objects.draw(&objs, &objectDrawAscii);
            manager->objects.draw(&items, &itemDrawAscii);
            manager->asciiWorld.draw(&world, &renderAsciiVoxelInstanced);

            if (key[SDL_SCANCODE_I])
                while (true)
                {
                    SDL_Event event;
                    SDL_PollEvent(&event);
                    inventoryDraw(&bag, 10, 10, 8, 8, slotDraw);
                    if (event.key.keysym.sym == SDLK_c)
                        break;
                }
        }
        {
            manager->mainPlayer.control(&(game->player), win, x, y, z);
            if (key[SDL_SCANCODE_SPACE] && game->player.isOnGround) {
                manager->mainPlayer.vertMove(&(game->player), GRAVITY * 100);
            }
        }
        {
            manager->mainPlayer.update(&(game->player), game->ltime.deltaTime * 0.001f, &world, &(manager->asciiWorld));
            manager->objects.update(&objs, game->ltime.deltaTime * 0.001f, &world, &(manager->asciiWorld));
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

        manager->screen.swap(&iscreen);
        manager->windowGL.swap(win);
    }
    manager->inventoryIcon.delete(&itemIcon);
    manager->screen.delete(&iscreen);
    manager->objects.delete(&objs);
    manager->objects.delete(&items);
    manager->asciiWorld.delete(&world);
}

void gameDelete(struct Game* const game, struct GlobalManager* const manager)
{
    manager->vao.delete(&(game->voxel));
    manager->program.delete(&(game->shader));
}

