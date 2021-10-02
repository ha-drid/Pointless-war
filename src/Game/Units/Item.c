#include "Item.h"

void itemInit(Item* item, float x, float y, float z, int64_t iD)
{
    memset(item, 0, sizeof(Item));
    item->position[0] = x;
    item->position[1] = y;
    item->position[2] = z;

    item->iD = iD;
}
