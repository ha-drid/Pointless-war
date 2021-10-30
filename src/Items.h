#ifndef ITEMS_H_INCLUDED
#define ITEMS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cvector/vector.h>

#include "Game/Units/Item.h"
#include "Util/Image.h"
#include "GL/Texture.h"

typedef enum {
    ItemEmptyEnum = 0,
    ItemSwordEnum = 1,
    ItemPickEnum = 2,
    ItemTest1Enum = 3,
} ItemEnum;

const static vec3 ItemsSize[] = {
    { 0.0f, 0.0f, 0.0f }, // Empty
    { 1.0f, 4.0f, 1.0f }, // Sword
    { 1.0f, 3.0f, 1.0f }, // Pick
    { 5.0f, 5.0f, 5.0f }, // Test1
};

const static float ItemsSpeed[] = {
        0.0f, // Empty
        2.0f, // Sword
        2.0f, // Pick
        0.5f, // Test1
};

static inline Item* createItem(float x, float y, float z, int64_t iD)
{
    Item* new_o = calloc(sizeof(Item), 1);

    glm_vec3_copy((vec3){ x, y, z }, new_o->position);
    new_o->iD = iD;

    return new_o;
}

static inline void itemGetPosition(void* const value, float** const position) {
    Item* i = value;
    (*position) = i->position;
}

static inline void itemGetVelocity(void* const value, float** const velocity) {
    Item* i = value;
    (*velocity) = i->velocity;
}

static inline void itemGetIsOnGround(void* const value, bool** const isOnGround) {
    Item* i = value;
    (*isOnGround) = &(i->isOnGround);
}

static inline void itemGetSize(void* const value, float** const size) {
    Item* i = value;
    (*size) = ItemsSize[i->iD];
}
static inline void itemGetSpeed(void* const value, float** const speed) {
    Item* i = value;
    (*speed) = &(ItemsSpeed[i->iD]);
}


#endif // ITEMS_H_INCLUDED
