#ifndef ITEM_HEADER_FILE
#define ITEM_HEADER_FILE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <cglm/cglm.h>

typedef struct Item {
    int64_t iD;
    vec3 position;
    vec3 velocity;
    float xAngle, yAngle;
    bool isOnGround;
} Item;
/*
typedef struct ItemManager {

} ItemManager;
*/
void itemInit(Item* item, float x, float y, float z, int64_t iD);

#endif // ITEM_HEADER_FILE
