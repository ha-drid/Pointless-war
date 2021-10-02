#include "Items.h"

static void inventoryIconsInit(InventoryIcons* const dest) {
    vectorInit(dest->icons, 0);
    dest->imageManager = imageManagerInit();
}

static void inventoryIconsAddIcon(InventoryIcons* const dest, const char* path) {
    Image img;
    dest->imageManager.init(&img, path);

    vectorPushBack(dest->icons, img);
}

static void inventoryIconsDelete(InventoryIcons* const dest) {
    for (size_t i = 0; i < dest->icons.size; ++i)
        dest->imageManager.delete(&(dest->icons.data[i]));

    vectorDelete(dest->icons);
}

InventoryIconsManager inventoryIconsManagerInit()
{
    InventoryIconsManager manager;

    manager.delete = &inventoryIconsDelete;
    manager.addIcon = &inventoryIconsAddIcon;
    manager.init = &inventoryIconsInit;

    return manager;
}
