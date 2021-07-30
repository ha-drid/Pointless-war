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

void mvpInit(struct MVP* mvp);
void mvpSetView(struct MVP* mvp, struct CameraGL* camera, struct Vector3fManager* manager);
void mvpSetProjection(struct MVP* mvp, float angle_radians, float ratio, float near, float far);
void mvpDelete(struct MVP* mvp);


#endif // MODELVIEWPROJECTION_HEADER_FILE
