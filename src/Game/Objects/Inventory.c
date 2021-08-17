#include "Inventory.h"

static void inventoryInit(struct Inventory* bag, uint32_t inventorySlotSize)
{
    bag->data = calloc(sizeof(unsigned short), inventorySlotSize);
    bag->elementSize = calloc(sizeof(uint32_t), inventorySlotSize);

    bag->length = inventorySlotSize;
}

static int inventoryAddItemInEmptySlot(struct Inventory* bag, unsigned short itemID)
{
    for (uint32_t i = 0; i < bag->length; ++i)
    {
        if (bag->data[i] == 0) {
            bag->data[i] = itemID;
            bag->elementSize[i] = 1;
        }
    }
}

static void inventoryDraw(struct Inventory* bag)
{
    for (uint32_t i = 0; i < bag->length; ++i)
    {
        printf("%hu : %u#", bag->data[i], bag->elementSize[i]);
    }
}

static void inventoryDelete(struct Inventory* bag) {
    free(bag->data);
    free(bag->elementSize);
    bag->length = 0;
}

struct InventoryManager inventoryManagerInit()
{
    struct InventoryManager manager;
    manager.addItemInEmptySlot = &inventoryAddItemInEmptySlot;
    manager.delete = &inventoryDelete;
    manager.init = &inventoryInit;
    manager.draw = &inventoryDraw;

    return manager;
};
