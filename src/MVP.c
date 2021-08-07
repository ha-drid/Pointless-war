#include "MVP.h"

static void mvpInit(struct MVP* mvp,
             struct Program* shader,
             const char* projection_name,
             const char* view_name,
             const char* model_name,
             struct ProgramManager* program,
             struct MatrixManager* matrix)
{
    matrix->init(&(mvp->model), 4, 4);
    matrix->init(&(mvp->projection), 4, 4);
    matrix->init(&(mvp->view), 4, 4);

    mvp->projectionID = program->getUniformLocation(shader, projection_name);
    mvp->viewID = program->getUniformLocation(shader, view_name);
    mvp->modelID = program->getUniformLocation(shader, model_name);
}

static void mvpSetView(struct MVP* mvp, struct CameraGL* camera, struct Vector3fManager* vector, struct MatrixManager* matrix)
{
    matrix->lookAt(&(mvp->view),
                camera->x, camera->y, camera->z,
                camera->x + sin(camera->xAngle), camera->y + tan(camera->yAngle), camera->z + cos(camera->xAngle),
                0.0f, 1.0f, 0.0f,
                vector);

    glUniformMatrix4fv(mvp->viewID, 1, GL_FALSE, mvp->view.data);
}

static void mvpSetProjection(struct MVP* mvp, float angle_radians, float ratio, float near, float far, struct MatrixManager* matrix)
{
    matrix->perpective(&(mvp->projection), angle_radians, ratio, near, far);
    glUniformMatrix4fv(mvp->projectionID, 1, GL_FALSE, mvp->projection.data);
}

void mvpModelShaderPush(struct MVP* mvp)
{
    glUniformMatrix4fv(mvp->modelID, 1, GL_FALSE, mvp->model.data);
}

static void mvpDelete(struct MVP* mvp, struct MatrixManager* matrix)
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
    manager.modelShaderPush = &mvpModelShaderPush;
    manager.setProjection = &mvpSetProjection;
    manager.setView = &mvpSetView;

    return manager;
};
