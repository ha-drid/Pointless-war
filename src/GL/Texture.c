#include "Texture.h"

static void textureBind(Texture* t)
{
    glBindTexture(GL_TEXTURE_2D, t->handle);
}

static void textureInit(Texture* t, int mag_filter_value, int min_filter_value, int wrap_s_value, int wrap_t_value)
{
    glGenTextures(1, &(t->handle));
	textureBind(t);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s_value);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t_value);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter_value);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter_value);

}


static void textureSetImage(Texture* t, int width, int height, int channels, uint8_t* const data) {
    textureBind(t);

    GLenum glFormat;

    switch (channels)
    {
    case 1:
        glFormat = GL_R;
        break;
    case 2:
        glFormat = GL_RG;
        break;
    case 3:
        glFormat = GL_RGB;
        break;
    case 4:
        glFormat = GL_RGBA;
        break;
    default:
        break;
    }


    glTexImage2D(GL_TEXTURE_2D,
				 0,
		         glFormat,
		         width,
		         height,
		         0,
		         glFormat,
		         GL_UNSIGNED_BYTE,
		         data);
}

static void textureDelete(Texture* t)
{
    glDeleteTextures(1, &(t->handle));
}

TextureManager textureManagerInit()
{
    TextureManager manager;
    manager.init = &textureInit;
    manager.bind = &textureBind;
    manager.delete = &textureDelete;
    manager.setImage = &textureSetImage;
    return manager;
}
