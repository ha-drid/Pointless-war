#ifndef MODELVIEWPROJECTION_HEADER_FILE
#define MODELVIEWPROJECTION_HEADER_FILE

#include "GL/CameraGL.h"
#include "Util/matrix.h"

struct ModelViewProjectionMatrix
{
    struct Matrix projection;
    struct Matrix view;
    struct Matrix model;
    int modelID;
    int viewID;
    int projectionID;
};

struct ModelViewProjectionMatrixManager
{
    void (*init)(struct ModelViewProjectionMatrix* mvp,
             struct Program* shader,
             const char* model_name,
             const char* view_name,
             const char* projection_name,
             struct ProgramManager* program,
             struct MatrixManager* matrix);
    void (*setView)(struct ModelViewProjectionMatrix* mvp, struct CameraGL* camera, struct Vector3fManager* vector, struct MatrixManager* matrix);
    void (*setProjection)(struct ModelViewProjectionMatrix* mvp, float angle_radians, float ratio, float near, float far, struct MatrixManager* matrix);
    void (*delete)(struct ModelViewProjectionMatrix* mvp, struct MatrixManager* matrix);
};

struct ModelViewProjectionMatrixManager modelViewProjectionMatrixManagerInit();

#endif // MODELVIEWPROJECTION_HEADER_FILE
