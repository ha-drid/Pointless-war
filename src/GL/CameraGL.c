#include "CameraGL.h"

#define M_PI    3.14159265358979323846264338327950288

#define radians_s(x) \
    ((float)((M_PI / 180.0f) * x))

#define gradus_s(x) \
    ((float)((180.0f / M_PI) * x))

void cameraGlInit(struct CameraGL* camera, float x, float y, float z, float xAngle, float yAngle, float speed)
{
    camera->x = x;
    camera->y = y;
    camera->z = z;
    camera->xAngle = xAngle;
    camera->yAngle = yAngle;
    camera->speed = speed;
}

void cameraRotate(struct CameraGL* camera, float xAngleGradus, float yAngleGradus)
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

void cameraApply(struct Program* program, struct ProgramManager* manager, struct CameraGL* camera,
                  const char* cameraPositionUniform, const char* cameraDirectionUniform)
{
    manager->setVec3f(program, cameraPositionUniform, camera->x, camera->y, camera->z);

    manager->setVec3f(program, cameraDirectionUniform,
                            camera->x + sin(camera->xAngle),
                            camera->y + tan(camera->yAngle),
                            camera->z + cos(camera->xAngle)
                            );
}

void cameraMove(struct CameraGL* camera, uint32_t move)
{
    if (move == CAMERA_FORWARD)
    {
        camera->x += sin(camera->xAngle) * camera->speed;
        camera->y += tan(camera->yAngle) * camera->speed;
        camera->z += cos(camera->xAngle) * camera->speed;
    }
    if (move == CAMERA_BACKWARD)
    {
        camera->x -= sin(camera->xAngle) * camera->speed;
        camera->y -= tan(camera->yAngle) * camera->speed;
        camera->z -= cos(camera->xAngle) * camera->speed;
    }
    if (move == CAMERA_LEFT)
    {
        camera->x += sin(radians_s(gradus_s(camera->xAngle) + 90)) * camera->speed;
        camera->z += cos(radians_s(gradus_s(camera->xAngle) + 90)) * camera->speed;
    }
    if (move == CAMERA_RIGHT)
    {
        camera->x += sin(radians_s(gradus_s(camera->xAngle) - 90)) * camera->speed;
        camera->z += cos(radians_s(gradus_s(camera->xAngle) - 90)) * camera->speed;
    }
}

void cameraSetPosition(struct CameraGL* camera, float x, float y, float z)
{
    camera->x = x;
    camera->y = y;
    camera->z = z;
}

struct CameraGLManager cameraGlManagerInit()
{
    struct CameraGLManager manager;

    manager.apply = &cameraApply;
    manager.init = &cameraGlInit;
    manager.move = &cameraMove;
    manager.rotate = &cameraRotate;
    manager.setPosition = &cameraSetPosition;

    return manager;
}


