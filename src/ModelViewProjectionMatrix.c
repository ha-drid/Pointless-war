#include "ModelViewProjectionMatrix.h"


static void mvpInit(struct ModelViewProjectionMatrix* mvp,
             struct Program* shader,
             const char* model_name,
             const char* view_name,
             const char* projection_name,
             struct ProgramManager* program,
             struct MatrixManager* matrix)
{
    matrix->init(&(mvp->model), 4, 4);
    matrix->init(&(mvp->projection), 4, 4);
    matrix->init(&(mvp->view), 4, 4);

    mvp->modelID = program->getUniformLocation(shader, model_name);
    mvp->viewID = program->getUniformLocation(shader, view_name);
    mvp->projectionID = program->getUniformLocation(shader, projection_name);
}

static void mvpSetView(struct ModelViewProjectionMatrix* mvp, struct CameraGL* camera, struct Vector3fManager* vector, struct MatrixManager* matrix)
{
    matrix->lookAt(&(mvp->view),
                camera->x, camera->y, camera->z,
                camera->x + sin(camera->xAngle), camera->y + tan(camera->yAngle), camera->z + cos(camera->xAngle),
                0.0f, 1.0f, 0.0f,
                vector);
    glUniformMatrix4fv(mvp->viewID, 1, GL_FALSE, mvp->view.data);
}

static void mvpSetProjection(struct ModelViewProjectionMatrix* mvp, float angle_radians, float ratio, float near, float far, struct MatrixManager* matrix)
{
    matrix->perpective(&(mvp->projection), angle_radians, ratio, near, far);
    glUniformMatrix4fv(mvp->projectionID, 1, GL_FALSE, mvp->projection.data);
}

static void mvpDelete(struct ModelViewProjectionMatrix* mvp, struct MatrixManager* matrix)
{
    matrix->delete(&(mvp->model));
    matrix->delete(&(mvp->projection));
    matrix->delete(&(mvp->view));
}

struct ModelViewProjectionMatrixManager modelViewProjectionMatrixManagerInit()
{
    struct ModelViewProjectionMatrixManager manager;

    manager.init = &mvpInit;
    manager.delete = &mvpDelete;
    manager.setProjection = &mvpSetProjection;
    manager.setView = &mvpSetView;

    return manager;
};
