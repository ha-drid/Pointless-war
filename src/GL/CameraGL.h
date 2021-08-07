#ifndef GLCAMERA_HEADER_INCLUDE_FILE
#define GLCAMERA_HEADER_INCLUDE_FILE

#include <stdint-gcc.h>

#include "Program.h"

#define CAMERA_FORWARD 0
#define CAMERA_BACKWARD 1
#define CAMERA_LEFT 2
#define CAMERA_RIGHT 3

struct CameraGL
{
    float x, y, z;
    float yAngle, xAngle;
    float speed;
};

struct CameraGLManager
{
    void (*init)(struct CameraGL* camera, float x, float y, float z, float xAngle, float yAngle, float speed);
    void (*rotate)(struct CameraGL* camera, float xAngleGradus, float yAngleGradus);
    void (*move)(struct CameraGL* camera, uint32_t move);
    void (*apply)(struct Program* program, struct ProgramManager* manager, struct CameraGL* camera,
                    const char* cameraPositionUniform, const char* cameraDirectionUniform);
    void (*setPosition)(struct CameraGL* camera, float x, float y, float z);
};

struct CameraGLManager cameraGlManagerInit();

#endif // GLCAMERA_HEADER_INCLUDE_FILE
