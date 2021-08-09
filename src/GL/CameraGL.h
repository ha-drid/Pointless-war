#ifndef GLCAMERA_HEADER_INCLUDE_FILE
#define GLCAMERA_HEADER_INCLUDE_FILE

#include <stdint-gcc.h>

#include "Program.h"

typedef enum {
    cameraMoveForward = 0,
    cameraMoveBackward = 1,
    cameraMoveRight = 2,
    cameraMoveLeft = 3,
} CameraGLMove;

struct CameraGL
{
    float x, y, z;
    float xVelocity, yVelocity, zVelocity;
    float yAngle, xAngle;
    float speed;
};

struct CameraGLManager
{
    void (*init)(struct CameraGL* camera, float x, float y, float z, float xAngle, float yAngle, float speed);
    void (*rotate)(struct CameraGL* camera, float xAngleGradus, float yAngleGradus);
    void (*move)(struct CameraGL* camera, CameraGLMove move, bool x, bool y, bool z);
    void (*setAngle)(struct CameraGL* camera, float xAngle, float yAngle);
    void (*setVelocity)(struct CameraGL* camera, float x, float y, float z);
    void (*apply)(struct Program* program, struct ProgramManager* manager, struct CameraGL* camera,
                    const char* cameraPositionUniform, const char* cameraDirectionUniform);
    void (*update)(struct CameraGL* camera, double time);
    void (*setPosition)(struct CameraGL* camera, float x, float y, float z);
};

struct CameraGLManager cameraGlManagerInit();

#endif // GLCAMERA_HEADER_INCLUDE_FILE
