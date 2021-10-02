#ifndef TEXTURE_GL_HEADER_FILE
#define TEXTURE_GL_HEADER_FILE

#include <stb_image.h>
#include <glad/glad.h>


typedef struct Texture {
    GLuint handle;
} Texture;

typedef struct TextureManager {
    void (*init)(Texture* t, int mag_filter_value, int min_filter_value, int wrap_s_value, int wrap_t_value);
    void (*bind)(Texture* t);
    void (*setImage)(Texture* t, int width, int height, int channels, uint8_t* const data) ;
    void (*delete)(Texture* t);
} TextureManager;

TextureManager textureManagerInit();

#endif // TEXTURE_GL_HEADER_FILE
