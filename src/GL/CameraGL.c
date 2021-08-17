#include "CameraGL.h"

#define M_PI    3.14159265358979323846264338327950288

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

static void cameraSetAngle(struct CameraGL* camera, float xAngle, float yAngle)
{
    camera->xAngle = xAngle;
    camera->yAngle = yAngle;
}

static void cameraSetPosition(struct CameraGL* camera, float x, float y, float z)
{
    camera->x = x;
    camera->y = y;
    camera->z = z;
}

static void cameraGlInit(struct CameraGL* camera, float x, float y, float z, float xAngle, float yAngle, float speed)
{
    cameraSetPosition(camera, x, y, z);
    cameraSetAngle(camera, xAngle, yAngle);
    camera->speed = speed;
}

static void cameraRotate(struct CameraGL* camera, float xAngleGradus, float yAngleGradus)
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

static void cameraApply(struct Program* program, struct ProgramManager* manager, struct CameraGL* camera,
                  const char* cameraPositionUniform, const char* cameraDirectionUniform)
{
    manager->setVec3f(program, cameraPositionUniform, camera->x, camera->y, camera->z);

    manager->setVec3f(program, cameraDirectionUniform,
                            camera->x + sin(camera->xAngle),
                            camera->y + tan(camera->yAngle),
                            camera->z + cos(camera->xAngle)
                            );
}

static void cameraMove(struct CameraGL* camera, CameraGLMove move, float time, bool x, bool y, bool z)
{
    if (x) {
        if (move == cameraMoveForward) camera->x += sin(camera->xAngle) * camera->speed * time;
        if (move == cameraMoveBackward) camera->x += -sin(camera->xAngle) * camera->speed * time;
        if (move == cameraMoveRight) camera->x += sin(camera->xAngle + radians_s(-90)) * camera->speed * time;
        if (move == cameraMoveLeft) camera->x += sin(camera->xAngle + radians_s(90)) * camera->speed * time;
    }

    if (y) {
        if (move == cameraMoveForward) camera->y += tan(camera->yAngle) * camera->speed * time;
        if (move == cameraMoveBackward) camera->y += -tan(camera->yAngle) * camera->speed * time;
    }

    if (z) {
        if (move == cameraMoveForward) camera->z += cos(camera->xAngle) * camera->speed * time;
        if (move == cameraMoveBackward) camera->z += -cos(camera->xAngle) * camera->speed * time;
        if (move == cameraMoveRight) camera->z += cos(camera->xAngle + radians_s(-90)) * camera->speed * time;
        if (move == cameraMoveLeft) camera->z += cos(camera->xAngle + radians_s(90)) * camera->speed * time;
    }
}

struct CameraGLManager cameraGlManagerInit()
{
    struct CameraGLManager manager;

    manager.apply = &cameraApply;
    manager.init = &cameraGlInit;
    manager.move = &cameraMove;
    manager.rotate = &cameraRotate;
    manager.setAngle = &cameraSetAngle;
    manager.setPosition = &cameraSetPosition;

    return manager;
}


