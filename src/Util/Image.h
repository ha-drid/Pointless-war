#ifndef IMAGE_HEADER_FILE
#define IMAGE_HEADER_FILE

#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

// каналы картинки
typedef enum {
    R = 1,
    RG = 2,
    RGB = 3,
    RGBA = 4,
} Format;

typedef struct Image {
    Format channels;
    int width, height;
    uint8_t* data;
} Image;

typedef struct ImageManager {
    void (*init)(Image* const img, const char* path);
    void (*resize)(Image* const img, int w, int h);
    void (*delete)(Image* img);
} ImageManager;

ImageManager imageManagerInit();

#endif // IMAGE_HEADER_FILE
