#include "Menu.h"

static void menuInit(Menu* const menu, const char* vertShader, const char* fragShader) {
    vectorInit(menu->buttons, 0);
    menu->programManager = programManagerInit();

    menu->programManager.init(&(menu->program), vertShader, fragShader);
    menu->programManager.bindAttribute(&(menu->program), 0, "position");
    menu->programManager.bindAttribute(&(menu->program), 1, "uv");
    menu->programManager.link(&(menu->program));
    glm_ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f, menu->projection);
}

static void menuSetWindowSize(Menu* const menu, int width, int height)
{
    glm_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, menu->projection);
}

static void menuAddButton(Menu* const menu, float x, float y, float w, float h,
                             int textureX, int textureY, int textureW, int textureH,
                             const char* image_path)
{
    Button button;
    ButtonManager manager = buttonManagerInit();
    manager.init(&button, x, y, w, h, textureX, textureY, textureW, textureH, image_path);
    vectorPushBack(menu->buttons, button);
}

static void menuDraw(Menu* const menu)
{
    mat4 view;

    glm_lookat((vec3){ 0.0f, 0.0f, 0.0f},
               (vec3){ 0.0f, 0.0f, -1.0f},
               (vec3){ 0.0f, 1.0f, 0.0f}, view);

    mat4 result;
    glm_mat4_mul(menu->projection, view, result);
    menu->programManager.use(&(menu->program));
    menu->programManager.setMat4fv(&(menu->program), "u_ViewProjection", 1, GL_FALSE, result);

    ButtonManager manager = buttonManagerInit();

    for (uint32_t i = 0; i < menu->buttons.size; ++i)
        manager.draw(menu->buttons.data + i);

}

static void menuDelete(Menu* const menu) {
    ButtonManager manager = buttonManagerInit();
    for (uint32_t i = 0; i < menu->buttons.size; ++i)
        manager.delete(menu->buttons.data + i);

    vectorDelete(menu->buttons);
    menu->programManager.delete(&(menu->program));
}

MenuManager menuManagerInit()
{
    MenuManager manager;
    manager.init = &menuInit;
    manager.addButton = &menuAddButton;
    manager.draw = &menuDraw;
    manager.delete = &menuDelete;
    manager.setWindowSize = &menuSetWindowSize;
    return manager;
}
