#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

static void imageInit(Image* const img, const char* path) {
    int channels;
    img->data = stbi_load(path, &(img->width), &(img->height), &channels, 0);
    if (img->data == NULL)
	{
		// не удалось загрузить
		printf("could not load image: %s", path);
		assert(0);
	}

    switch (channels)
    {
    case (1):
        img->channels = R;
        break;
    case (2):
        img->channels = RG;
        break;
    case (3):
        img->channels = RGB;
        break;
    case (4):
        img->channels = RGBA;
        break;
    default:
        assert(0);
        break;
    }


}
static void imageDelete(Image* img) {
    stbi_image_free(img->data);
    img->height = 0;
    img->width = 0;
    img->channels = 0;
}

static void imageResize(Image* const img, int w, int h) {
    Format channels = img->channels;
    uint8_t* data = calloc(sizeof(uint8_t) * channels, w * h);

    stbir_resize_uint8(img->data, img->width, img->height, 0,
                       data, w, h, 0, channels);

    imageDelete(img);

    img->data = data;
    img->height = h;
    img->width = w;
    img->channels = channels;
}


ImageManager imageManagerInit()
{
    ImageManager manager;

    manager.delete = &imageDelete;
    manager.init = &imageInit;
    manager.resize = &imageResize;

    return manager;
}
