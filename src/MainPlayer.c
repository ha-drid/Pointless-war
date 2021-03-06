#include "MainPlayer.h"
#include "Game/Voxels/Voxel.h"

static void mainPlayerInit(MainPlayer* const player, vec3 position, vec3 size, float speed)
{
    player->manager = cameraGlManagerInit();
    player->isOnGround = false;
    glm_vec3_copy(size, player->size);

    const CameraGL* const self = &(player->camera);
    player->manager.init(self, position, 0.0f, 0.0f, speed);
    memset(player->camera.velocity, 0, sizeof(vec3));
}

static void mainPlayerPerspective(MainPlayer* const player, float radian, float aspect, float nearZ, float farZ)
{
    const CameraGL* const self = &(player->camera);
    player->manager.perspective(self, radian, aspect, nearZ, farZ);
}

static void mainPlayerVertMove(MainPlayer* const player, float gravity)
{
    player->camera.velocity[1] += gravity;
}

static void mainPlayerControl(MainPlayer* const player, WindowGL* const window, bool x, bool y, bool z)
{
    player->manager.move(&(player->camera), x, y, z, SDL_SCANCODE_W, SDL_SCANCODE_S, SDL_SCANCODE_A, SDL_SCANCODE_D);
}

static void mainPlayerUpdate(MainPlayer* player, float time, AsciiWorld* const world, AsciiWorldManager* const manager)
{
    bool objectWorldIsColision() {
        MainPlayer* p = player;
        for (int z = (p->camera.position[2] - p->size[2]); z < (p->camera.position[2] + p->size[2]); ++z)
        for (int y = (p->camera.position[1] - p->size[1]); y < (p->camera.position[1] + p->size[1]); ++y)
        for (int x = (p->camera.position[0] - p->size[0]); x < (p->camera.position[0] + p->size[0]); ++x) {
            if (manager->getVoxel(world, x, y, z).iD != Empty)
            {
                if (isColision(p->camera.position[0], p->camera.position[1], p->camera.position[2],
                            p->size[0], p->size[1], p->size[2],
                            x, y, z,
                            1, 1, 1))
                {
                    return true;
                }
            }
        }

        return false;
    }
    float * const position = player->camera.position;
    float * const velocity = player->camera.velocity;
    float * const size = player->size;
    float speed = player->camera.speed;
    player->isOnGround = false;

    position[0] += velocity[0] * time * speed;
    if (objectWorldIsColision()) {
        position[0] -= velocity[0] * time * speed;
    }

    position[1] += velocity[1] * time * speed;
     if (objectWorldIsColision()) {
        position[1] -= velocity[1] * time * speed;
        velocity[1] = 0.0f;
        player->isOnGround = true;
    }

    position[2] += velocity[2] * time * speed;
    if (objectWorldIsColision()) {
        position[2] -= velocity[2] * time * speed;
    }

    velocity[0] = velocity[2] = 0.0f;
}

void mainPlayerGetDirection(MainPlayer* player, vec3 dir)
{
    dir[0] = cos(player->camera.yAngle) * sin(player->camera.xAngle);
    dir[1] = sin(player->camera.yAngle);
    dir[2] = cos(player->camera.yAngle) * cos(player->camera.xAngle);
}

void mainPlayerGetView(MainPlayer* player, vec3 pos, vec3 dir, vec3 up)
{
    CameraGL* const camera = &(player->camera);
    vec3 direction = {
		cos(camera->yAngle) * sin(camera->xAngle),
		sin(camera->yAngle),
		cos(camera->yAngle) * cos(camera->xAngle)
    };

    vec3 right = {
		sin(camera->xAngle - glm_rad(90.0f)),
		0,
		cos(camera->xAngle - glm_rad(90.0f))
	};


	glm_vec3_cross(right, direction, up);

	pos[0] = camera->position[0] + (player->size[0] / 2.0f);
	pos[1] = camera->position[1] + player->size[1];
	pos[2] = camera->position[2] + (player->size[2] / 2.0f);
	glm_vec3_add(pos, direction, dir);

};

MainPlayerManager mainPlayerManagerInit()
{
    MainPlayerManager manager;
    manager.init = &mainPlayerInit;
    manager.control = &mainPlayerControl;
    manager.update = &mainPlayerUpdate;
    manager.vertMove = &mainPlayerVertMove;
    manager.getDirection = &mainPlayerGetDirection;
    manager.getView = &mainPlayerGetView;

    return manager;
}
