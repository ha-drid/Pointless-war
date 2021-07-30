#include "MVP.h"

void mvpInit(struct MVP* mvp)
{
    matrixInit(&(mvp->model), 4, 4);
    matrixInit(&(mvp->projection), 4, 4);
    matrixInit(&(mvp->view), 4, 4);
}

void mvpSetView(struct MVP* mvp, struct CameraGL* camera, struct Vector3fManager* manager)
{
    matrixLookAt(&(mvp->view),
                camera->x, camera->y, camera->z,
                camera->x + sin(camera->xAngle), camera->y + tan(camera->yAngle), camera->z + cos(camera->xAngle),
                0.0f, 1.0f, 0.0f,
                manager);
}

void mvpSetProjection(struct MVP* mvp, float angle_radians, float ratio, float near, float far)
{
    matrixPerpective(&(mvp->projection), angle_radians, ratio, near, far);
}

void mvpDelete(struct MVP* mvp)
{
    matrixDelete(&(mvp->model));
    matrixDelete(&(mvp->projection));
    matrixDelete(&(mvp->view));
}
