#ifndef MODELVIEWPROJECTION_HEADER_FILE
#define MODELVIEWPROJECTION_HEADER_FILE

#include "GL/CameraGL.h"
#include "Util/matrix.h"

struct MVP
{
    struct Matrix projection;
    struct Matrix view;
    struct Matrix model;
};

struct MVPManager
{
    void (*init)(struct MVP* mvp, struct MatrixManager* matrix);
    void (*setView)(struct MVP* mvp, struct CameraGL* camera, struct Vector3fManager* vector, struct MatrixManager* matrix);
    void (*setProjection)(struct MVP* mvp, float angle_radians, float ratio, float near, float far, struct MatrixManager* matrix);
    void (*delete)(struct MVP* mvp, struct MatrixManager* matrix);
};

struct MVPManager mvpManagerInit();

#endif // MODELVIEWPROJECTION_HEADER_FILE
