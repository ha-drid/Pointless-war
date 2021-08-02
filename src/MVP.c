#include "MVP.h"

void mvpInit(struct MVP* mvp, struct MatrixManager* matrix)
{
    matrix->init(&(mvp->model), 4, 4);
    matrix->init(&(mvp->projection), 4, 4);
    matrix->init(&(mvp->view), 4, 4);
}

void mvpSetView(struct MVP* mvp, struct CameraGL* camera, struct Vector3fManager* vector, struct MatrixManager* matrix)
{
    matrix->lookAt(&(mvp->view),
                camera->x, camera->y, camera->z,
                camera->x + sin(camera->xAngle), camera->y + tan(camera->yAngle), camera->z + cos(camera->xAngle),
                0.0f, 1.0f, 0.0f,
                vector);
}

void mvpSetProjection(struct MVP* mvp, float angle_radians, float ratio, float near, float far, struct MatrixManager* matrix)
{
    matrix->perpective(&(mvp->projection), angle_radians, ratio, near, far);
}

void mvpDelete(struct MVP* mvp, struct MatrixManager* matrix)
{
    matrix->delete(&(mvp->model));
    matrix->delete(&(mvp->projection));
    matrix->delete(&(mvp->view));
}

struct MVPManager mvpManagerInit()
{
    struct MVPManager manager;

    manager.init = &mvpInit;
    manager.delete = &mvpDelete;
    manager.setProjection = &mvpSetProjection;
    manager.setView = &mvpSetView;

    return manager;
};
