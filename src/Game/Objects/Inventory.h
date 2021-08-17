#ifndef INVENTORY_HEADER_FILE
#define INVENTORY_HEADER_FILE

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <SDL2/SDL.h>

struct Inventory
{
    unsigned short* data;
    uint32_t* elementSize;
    uint32_t length;
};

struct InventoryManager
{
    void (*init)(struct Inventory* bag, uint32_t inventorySlotSize);
    int (*addItemInEmptySlot)(struct Inventory* bag, unsigned short itemID);
    void (*draw)(struct Inventory* bag);
    void (*delete)(struct Inventory* bag);
};

struct InventoryManager inventoryManagerInit();

#endif // INVENTORY_HEADER_FILE
