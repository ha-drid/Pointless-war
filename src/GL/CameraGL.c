#include "CameraGL.h"

#define M_PI    3.14159265358979323846264338327950288

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

static void cameraSetAngle(struct CameraGL* const camera, float xAngle, float yAngle)
{
    camera->xAngle = xAngle;
    camera->yAngle = yAngle;
}

static void cameraSetPosition(struct CameraGL* const camera, vec3 pos)
{
    glm_vec3_copy(pos, camera->position);
}

static void cameraGlInit(struct CameraGL* const camera, vec3 pos, float xAngle, float yAngle, float speed)
{
    cameraSetPosition(camera, pos);
    cameraSetAngle(camera, xAngle, yAngle);
//    cameraSetVelocity(camera, (float[]){ 0.0f, 0.0f, 0.0f });
    camera->speed = speed;

 //   cameraOrtho(camera, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}

static void cameraRotate(struct CameraGL* const camera, float xAngleGradus, float yAngleGradus)
{
    camera->xAngle += radians_s(xAngleGradus);
    camera->yAngle += radians_s(yAngleGradus);

    if (camera->xAngle > radians_s(360))
        camera->xAngle -= radians_s(360);
    if (camera->xAngle < radians_s(0))
        camera->xAngle += radians_s(360);

    if (camera->yAngle > radians_s(89.0f))
        camera->yAngle = radians_s(89.0f);

    if (camera->yAngle < radians_s(-89.0f))
        camera->yAngle = radians_s(-89.0f);
}
/*
static void cameraGLApply(struct CameraGL* const camera,
                  struct Program* const program,
                  const char* u_ProjectionView,
                  struct ProgramManager* const  manager)
{
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

	vec3 up;
	glm_vec3_cross(right, direction, up);

    glm_lookat( camera->position,
               (vec3){ camera->position[0] + direction[0],
                       camera->position[1] + direction[1],
                       camera->position[2] + direction[2] },
               up,
               camera->view);

    mat4 mul;
    glm_mat4_mul(camera->projection, camera->view, mul);

    manager->setMat4fv(program, u_ProjectionView, 1, GL_FALSE, mul);
}*/

static void cameraMove(struct CameraGL* camera, bool x, bool y, bool z,
                         uint8_t forwardKey, uint8_t backKey, uint8_t leftKey, uint8_t rightKey)
{
    const uint8_t const *state = SDL_GetKeyboardState(NULL);
    vec3 right = {
		sin(camera->xAngle - glm_rad(90.0f)),
		0,
		cos(camera->xAngle - glm_rad(90.0f))
	};

    vec3 direction = { 0.0f, 0.0f, 0.0f };
    if (x) direction[0] = cos(camera->yAngle) * sin(camera->xAngle);
    if (y) direction[1] = sin(camera->yAngle);
    if (z) direction[2] = cos(camera->yAngle) * cos(camera->xAngle);

    if (state[forwardKey])
        glm_vec3_add(camera->velocity, direction, camera->velocity);

    if (state[backKey])
        glm_vec3_sub(camera->velocity, direction, camera->velocity);

    if (state[leftKey])
        glm_vec3_sub(camera->velocity, right, camera->velocity);

    if (state[rightKey])
        glm_vec3_add(camera->velocity, right, camera->velocity);
}

static void cameraGetDirection(struct CameraGL* const camera, vec3 dest)
{
    dest[0] = cos(camera->yAngle) * sin(camera->xAngle);
    dest[1] = sin(camera->yAngle);
    dest[2] = cos(camera->yAngle) * cos(camera->xAngle);
}

static void cameraUpdate(struct CameraGL* const camera, float time)
{
    camera->position[0] += camera->velocity[0] * time * camera->speed;
    camera->position[1] += camera->velocity[1] * time * camera->speed;
    camera->position[2] += camera->velocity[2] * time * camera->speed;

    //glm_vec3_add(camera->position, camera->velocity, camera->position);

    memset(camera->velocity, 0, sizeof(vec3));
}

struct CameraGLManager cameraGlManagerInit()
{
    struct CameraGLManager manager;

//    manager.apply = &cameraGLApply;
   // manager.perspective = &cameraPerspective;
    //manager.ortho = &cameraOrtho;
    manager.init = &cameraGlInit;
    manager.rotate = &cameraRotate;
    manager.setAngle = &cameraSetAngle;
 //   manager.apply = &cameraGLApply;
    manager.move = &cameraMove;
    manager.setPosition = &cameraSetPosition;
    manager.update = &cameraUpdate;
    manager.getDirection = &cameraGetDirection;

    return manager;
}


