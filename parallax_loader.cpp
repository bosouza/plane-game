#include <parallax_loader.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool LoadImages(parallaxImage images[], unsigned int size)
{
    stbi_set_flip_vertically_on_load(true);
    for (int i = 0; i < size; i++)
    {
        glGenTextures(1, &images[i].textureId);
        glBindTexture(GL_TEXTURE_2D, images[i].textureId);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        int width, height, nrChannels;
        unsigned char *data = stbi_load(images[i].path.c_str(), &width, &height, &nrChannels, 0);
        if (data == NULL)
        {
            std::cout << "Failed to load texture " << images[i].path << std::endl;
            return false;
        }
        // std::cout << "width: " << width << ", height: " << height << ", nrChannels: " << nrChannels << std::endl;
        unsigned int format;
        switch (nrChannels)
        {
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            std::cout << "received image with unexpected number of channels: " << nrChannels << ", " << images[i].path << std::endl;
            break;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

        float pWidth, pHeight;
        if (width > height)
        {
            pWidth = 1.0f / images[i].stretch;
            pHeight = ((float)width / height) / images[i].stretch;
        }
        else
        {
            pHeight = 1.0f / images[i].stretch;
            pWidth = ((float)height / width) / images[i].stretch;
        }

        std::cout << "image: " << images[i].path << " pWidth: " << pWidth << " pHeight: " << pHeight << std::endl;

        images[i].maping[0] = pWidth;
        images[i].maping[1] = pHeight;
        images[i].maping[2] = pWidth;
        images[i].maping[3] = 0.0f;
        images[i].maping[4] = 0.0f;
        images[i].maping[5] = 0.0f;
        images[i].maping[6] = 0.0f;
        images[i].maping[7] = pHeight;
        // stbi_image_free(data);
    }
    return true;
}

sprite::sprite(std::string path, unsigned int wQtd, unsigned int hQtd, int offsetxLocation, int offsetyLocation)
{
    this->wQtd = wQtd;
    this->hQtd = hQtd;
    this->offsetxLocation = offsetxLocation;
    this->offsetyLocation = offsetyLocation;
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture " << path << std::endl;
        return;
    }
    // std::cout << "width: " << width << ", height: " << height << ", nrChannels: " << nrChannels << std::endl;
    unsigned int format;
    switch (nrChannels)
    {
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        std::cout << "received image with unexpected number of channels: " << nrChannels << ", " << path << std::endl;
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void sprite::BindAction(unsigned int action)
{
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1f(this->offsetxLocation, (float)(action % wQtd) / (float)wQtd);
    glUniform1f(this->offsetyLocation, ((float)hQtd - 1.0f - (float)(action / wQtd)) / (float)hQtd);
}

void sprite::FillTextureBuffer(float buffer[])
{
    buffer[0] = 1.0f / wQtd;
    buffer[1] = 1.0f / hQtd;
    buffer[2] = 1.0f / wQtd;
    buffer[3] = 0.0f;
    buffer[4] = 0.0f;
    buffer[5] = 0.0f;
    buffer[6] = 0.0f;
    buffer[7] = 1.0f / hQtd;
}