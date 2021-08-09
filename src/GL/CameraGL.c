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

static void cameraSetVelocity(struct CameraGL* camera, float x, float y, float z)
{
    camera->xVelocity = x;
    camera->yVelocity = y;
    camera->zVelocity = z;
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
    cameraSetVelocity(camera, 0.0f, 0.0f, 0.0f);
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

static void cameraMove(struct CameraGL* camera, CameraGLMove move, bool x, bool y, bool z)
{
    if (x) {
        if (move == cameraMoveForward) camera->xVelocity = sin(camera->xAngle) * camera->speed;
        if (move == cameraMoveBackward) camera->xVelocity = -sin(camera->xAngle) * camera->speed;
        if (move == cameraMoveRight) camera->xVelocity = sin(camera->xAngle + radians_s(-90)) * camera->speed;
        if (move == cameraMoveLeft) camera->xVelocity = sin(camera->xAngle + radians_s(90)) * camera->speed;
    }

    if (y) {
        if (move == cameraMoveForward) camera->yVelocity = tan(camera->yAngle) * camera->speed;
        if (move == cameraMoveBackward) camera->yVelocity = -tan(camera->yAngle) * camera->speed;
    }

    if (z) {
        if (move == cameraMoveForward) camera->zVelocity = cos(camera->xAngle) * camera->speed;
        if (move == cameraMoveBackward) camera->zVelocity = -cos(camera->xAngle) * camera->speed;
        if (move == cameraMoveRight) camera->zVelocity = cos(camera->xAngle + radians_s(-90)) * camera->speed;
        if (move == cameraMoveLeft) camera->zVelocity = cos(camera->xAngle + radians_s(90)) * camera->speed;
    }
}

static void cameraUpdate(struct CameraGL* camera, double time)
{
    camera->x += camera->xVelocity * time;
    camera->y += camera->yVelocity * time;
    camera->z += camera->zVelocity * time;

    cameraSetVelocity(camera, 0.0f, 0.0f, 0.0f);
}

struct CameraGLManager cameraGlManagerInit()
{
    struct CameraGLManager manager;

    manager.apply = &cameraApply;
    manager.init = &cameraGlInit;
    manager.move = &cameraMove;
    manager.update = &cameraUpdate;
    manager.rotate = &cameraRotate;
    manager.setAngle = &cameraSetAngle;
    manager.setVelocity = &cameraSetVelocity;
    manager.setPosition = &cameraSetPosition;

    return manager;
}


