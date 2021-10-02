#ifndef INVENTORY_HEADER_FILE
#define INVENTORY_HEADER_FILE

#include <stdio.h>
#include <stdint.h>
#include <malloc.h>
#include <inttypes.h>
#include <SDL2/SDL.h>

typedef struct Slot
{
    unsigned short iD;
    unsigned short count;
} Slot;

#define Inventory(width, height) \
    struct { \
        struct Slot data[height][width];\
    }

void inventoryClear(void* bag, size_t width, size_t height);
void inventoryPrintf(void* bag, size_t width, size_t height);
void inventoryPush(void* bag, size_t width, size_t height, uint16_t iD, uint16_t maxSize);
void inventoryDraw(void* bag, size_t width, size_t height, int x, int y, void (*const slotDraw)(int x, int y, Slot sl));

#endif // INVENTORY_HEADER_FILE
