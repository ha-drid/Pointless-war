#include "Inventory.h"

void inventoryClear(void* bag, size_t width, size_t height) {
    memset(bag, 0, sizeof(Slot) * width * height);
}
/*
void inventoryDraw(void* bag, size_t width, size_t height,
                   void (*renderSlot)(float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b)) {
    Inventory(width, height)* const inv = bag;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            glScalef(2.0f / 100.0f, 2.0f / 100.0f, 1.0f);
            renderSlot(x * 2.0f, y * 2.0f, 2.0f, 2.0f, 0.0f, 0.0f, 0.0f);
        }
        printf("\n");
    }
}
*/
void inventoryPrintf(void* bag, size_t width, size_t height) {
    Slot* ptr = bag;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            printf("iD:%hu|count:%hu  ", ptr->iD, ptr->count);
            ++ptr;
        }
        printf("\n");
    }
}

void inventoryPush(void* bag, size_t width, size_t height, uint16_t iD, uint16_t maxSize)
{
    Slot* ptr = bag;
    for (size_t y = 0; y < height; ++y) {
        for (size_t x = 0; x < width; ++x) {
            if (ptr->count == 0) {
                ptr->iD = iD;
                ptr->count = 1;
                return;
            }
            else if (ptr->iD == iD && ptr->count < maxSize) {
                ++ptr->count;
                return;
            }
            ++ptr;
        }
    }
}

void inventoryDraw(void* bag, size_t width, size_t height, int x, int y, void (*const slotDraw)(int x, int y, Slot sl))
{
    Slot* ptr = bag;
    for (size_t y1 = 0; y1 < height; ++y1) {
        for (size_t x1 = 0; x1 < width; ++x1) {
            slotDraw(x + x1, y + y1, (*ptr));
            ++ptr;
        }
    }
}

