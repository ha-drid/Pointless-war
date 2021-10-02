#ifndef GLCAMERA_HEADER_INCLUDE_FILE
#define GLCAMERA_HEADER_INCLUDE_FILE

#include <SDL2/SDL.h>
#include <stdint-gcc.h>
#include <cglm/cglm.h>
#include "Program.h"

typedef enum {
    cameraMoveForward = 0,
    cameraMoveBackward = 1,
    cameraMoveRight = 2,
    cameraMoveLeft = 3,
} CameraGLMove;

typedef struct CameraGL
{
    vec3 position;
    vec3 velocity;
    float yAngle, xAngle;
    float speed;
    mat4 projection, view;
} CameraGL;

typedef struct CameraGLManager
{
    void (*init)(struct CameraGL* const camera, vec3 pos, float xAngle, float yAngle, float speed);
    void (*perspective)(struct CameraGL* const camera, float radian, float aspect, float nearZ, float farZ);
    void (*ortho)(struct CameraGL* const camera,
                  float left,    float right,
                  float bottom,  float top,
                  float nearZ,   float farZ);
    void (*rotate)(struct CameraGL* const camera, float xAngleGradus, float yAngleGradus);
    void (*setAngle)(struct CameraGL* const camera, float xAngle, float yAngle);
    void (*apply)(struct CameraGL* const camera,
                  struct Program* const program,
                  const char* u_ProjectionView,
                  struct ProgramManager* const manager);
    void (*move)(struct CameraGL* camera, bool x, bool y, bool z,
                         uint8_t forwardKey, uint8_t backKey, uint8_t leftKey, uint8_t rightKey);
    void (*setPosition)(struct CameraGL* const camera, vec3 pos);
    void (*update)(struct CameraGL* const cam, float time);
    void (*getDirection)(struct CameraGL* const cam, vec3 dest);
} CameraGLManager;

CameraGLManager cameraGlManagerInit();

#endif // GLCAMERA_HEADER_INCLUDE_FILE
