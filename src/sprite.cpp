#include <sprite.h>
#include <iostream>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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