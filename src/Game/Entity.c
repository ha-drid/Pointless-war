#include "Entity.h"

#define M_PI    3.14159265358979323846264338327950288

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

void entityInit(struct Entity* entity,
                float x,
                float y,
                float z,
                float width,
                float height,
                float depth)
{
    entitySetPosition(entity, x, y, z);

    entity->width = width;
    entity->height = height;
    entity->depth = depth;

    entity->vertSpeed = 0.0f;

    entity->isOnGround = false;
}
/*
void entityRotate(struct Entity* entity, float xAngleGradus, float yAngleGradus)
{
    entity->xAngle += radians_s(xAngleGradus);
    entity->yAngle += radians_s(yAngleGradus);

    if (entity->xAngle > radians_s(360))
        entity->xAngle -= radians_s(360);
    if (entity->xAngle < radians_s(0))
        entity->xAngle += radians_s(360);

    if (entity->yAngle > radians_s(90))
        entity->yAngle = radians_s(90);
    if (entity->yAngle < radians_s(-90))
        entity->yAngle = radians_s(-90);

    entity->xRotate = sin(entity->xAngle);
    entity->yRotate = (tan(gradus_s(entity->yAngle) / 180 * M_PI));
    entity->zRotate = cos(entity->xAngle);
}
*/
void entitySetPosition(struct Entity* entity, float x, float y, float z)
{
    entity->x = x;
    entity->y = y;
    entity->z = z;
}

void entityVertMove(struct Entity* entity, struct VoxelWorld* world, struct VoxelWorldManager worldManager)
{
    entity->vertSpeed -= 0.04f;
    entity->y += entity->vertSpeed;

    struct Entity* b = entity;
    struct VoxelWorld* w = world;

    for (int Z = (b->z - b->depth) / w->world.chunk_depth; Z < (b->z + b->depth) / w->world.chunk_depth; ++Z)
        for (int Y = (b->y - b->height) / w->world.chunk_height; Y < (b->y + b->height) / w->world.chunk_height; ++Y)
            for (int X = (b->x - b->width) / w->world.chunk_width; X < (b->x + b->width) / w->world.chunk_width; ++X)

                if (worldManager.isThereAChunk(w, X, Y, Z))
                    if ((isColision(b->x, b->y, b->z, b->width, b->height, b->depth,
                                   X*w->world.chunk_width,
                                   Y*w->world.chunk_height,
                                   Z*w->world.chunk_depth,
                                   w->world.chunk_width, w->world.chunk_width, w->world.chunk_width)) &&
                        (w->world.chunks.data[Z][Y][X].voxels->iD != 0)) {
                        entity->y -= entity->vertSpeed;
                        entity->vertSpeed = 0;
                        entity->isOnGround = true;
                        return ;
                    }

}

bool entityColision(struct Entity* entity, float x, float y, float z, float width, float height, float depth)
{
    // collision x-axis
    bool collisionX = ((entity->x + entity->width) >= x) && ((x + width) >= entity->x);
    // collision y-axis
    bool collisionY = ((entity->y + entity->height) >= y) && ((y + height) >= entity->y);
    // collision z-axis
    bool collisionZ = ((entity->z + entity->depth) >= z) && ((z + depth) >= entity->z);

    // collision only if on both axes
    return collisionX && collisionY && collisionZ;
}

void entityHorizontMove(struct Entity* entity, struct VoxelWorld* world, float dx, float dz)
{
    entity->x += dx;
    entity->z += dz;

    for (uint32_t z = 0; z < world->world.chunks.countZ; ++z)
    {
        for (uint32_t y = 0; y < world->world.chunks.countY; ++y)
        {
            for (uint32_t x = 0; x < world->world.chunks.countX; ++x)
            {

            }
        }
    }
}

void entityMove(struct Entity* entity, struct VoxelWorld* world, uint32_t move, float time, float speed)
{
    float dx = 0.0f;
    float dz = 0.0f;

    if (move == ENTITY_FORWARD)
    {
        dx += sin(entity->xAngle) * speed;
        dz += cos(entity->xAngle) * speed;
    }
    if (move == ENTITY_BACKWARD)
    {
        dx -= sin(entity->xAngle) * speed;
        dz -= cos(entity->xAngle) * speed;
    }
    if (move == ENTITY_LEFT)
    {
        dx += sin(radians_s(gradus_s(entity->xAngle) + 90)) * speed;
        dz += cos(radians_s(gradus_s(entity->xAngle) + 90)) * speed;
    }
    if (move == ENTITY_RIGHT)
    {
        dx += sin( radians_s(gradus_s(entity->xAngle) - 90) ) * speed;
        dz += cos( radians_s(gradus_s(entity->xAngle) - 90) ) * speed;
    }

    entityHorizontMove(entity, world, dx, dz);
}

/*
void entityMove(struct Entity* entity, bool fly, uint32_t move)
{
    if (move == ENTITY_FORWARD)
    {
        entity->xVelocity -= sin(entity->xAngle);
        entity->zVelocity -= cos(entity->xAngle);
    }
    if (move == ENTITY_BACKWARD)
    {
        entity->xVelocity += sin(entity->xAngle);
        entity->zVelocity += cos(entity->xAngle);
    }
    if (move == ENTITY_LEFT)
    {
        entity->xVelocity += sin(radians_s(gradus_s(entity->xAngle) - 90));
        entity->zVelocity += cos(radians_s(gradus_s(entity->xAngle) - 90));
    }
    if (move == ENTITY_RIGHT)
    {
        entity->xVelocity += sin(radians_s(gradus_s(entity->xAngle) + 90));
        entity->zVelocity += cos(radians_s(gradus_s(entity->xAngle) + 90));
    }
}

void entityColisionInVoxelWorld(struct Entity* entity, struct VoxelWorld* world, float dx, float dy, float dz)
{
    const float x = entity->x, w = entity->width;
    const float y = entity->y, h = entity->height;
    const float z = entity->z, d = entity->depth;

    const float size_x = world->world.chunk_width;
    const float size_y = world->world.chunk_height;
    const float size_z = world->world.chunk_depth;

    for (int Z = (z - d) / size_z; Z < (z + d) / size_z; ++Z)
        for (int Y = (y - h) / size_y; Y < (y + h) / size_y; ++Y)
            for (int X = (x - w) / size_x; X < (x + w) / size_x; ++X)
                if (voxelWorldIsThereAChunk(world, X, Y, Z))
                {
                    if (isColision(x, y, z, w, h, d, X*size_x, Y*size_y, Z*size_z, size_x, size_y, size_z)) {
                        if (dx > 0)  entity->x = X * size_x - entity->width;
                        if (dx < 0)  entity->x = X * size_x + size_x + entity->width;
                        if (dy > 0)  entity->y -= entity->y;
                        if (dy < 0)  { entity->y += 1.0f; entity->isOnGround = true; entity->yVelocity = 0;};
                        //if (dz > 0)  entity->z = Z * size_z - d;
                        //if (dz < 0)  entity->z = Z * size_z + size + d;
                    }
                }

}*/

void entityDraw(struct Entity* entity, void (*drawVoxel)(float x, float y, float z))
{
    for (int z = entity->z; z < (entity->z + entity->depth); ++z)
        for (int y = entity->y; y < (entity->y + entity->height); ++y)
            for (int x = entity->x; x < (entity->x + entity->width); ++x) {
                drawVoxel(x, y, z);
            }

}


