#include <parallax_image.h>
#include <iostream>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "stb_image.h"

parallax_image::parallax_image(std::string path, float parallaxRatex, float parallaxRatey, float stretch, bool allignCenter, int offsetxLocation, int offsetyLocation)
{
    this->path = path;
    this->parallaxRatex = parallaxRatex;
    this->parallaxRatey = parallaxRatey;
    this->stretch = stretch;
    this->allignCenter = allignCenter;
    this->offsetxLocation = offsetxLocation;
    this->offsetyLocation = offsetyLocation;

    glGenVertexArrays(1, &this->VAO);
    stbi_set_flip_vertically_on_load(true);
    glGenTextures(1, &this->textureID);
    glBindTexture(GL_TEXTURE_2D, this->textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int nrChannels;
    unsigned char *data = stbi_load(this->path.c_str(), &this->width, &this->height, &nrChannels, 0);
    if (data == NULL)
    {
        std::cout << "Failed to load texture " << this->path << std::endl;
        throw "Failed to load texture";
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
        std::cout << "received image with unexpected number of channels: " << nrChannels << ", " << this->path << std::endl;
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
}

void parallax_image::bind()
{
    glBindVertexArray(this->VAO);
    glBindTexture(GL_TEXTURE_2D, this->textureID);
}

void parallax_image::fillTextureBuffer(float buffer[], int screenWidth, int screenHeight)
{
    float pWidth, pHeight;
    if (screenWidth / screenHeight > pWidth / pHeight)
    {
        pWidth = 1.0f / this->stretch;
        pHeight = ((float)screenHeight / screenWidth) * ((float)this->width / this->height) / this->stretch;
    }
    else
    {
        pHeight = 1.0f / this->stretch;
        pWidth = ((float)screenWidth / screenHeight) * ((float)this->height / this->width) / this->stretch;
    }
    std::cout << "image: " << this->path << " pWidth: " << pWidth << " pHeight: " << pHeight << std::endl;
    if (this->allignCenter)
    {
        buffer[0] = 1 - ((1 - pWidth) / 2);
        buffer[1] = 1 - ((1 - pHeight) / 2);
        buffer[2] = 1 - ((1 - pWidth) / 2);
        buffer[3] = (1 - pHeight) / 2;
        buffer[4] = (1 - pWidth) / 2;
        buffer[5] = (1 - pHeight) / 2;
        buffer[6] = (1 - pWidth) / 2;
        buffer[7] = 1 - ((1 - pHeight) / 2);
    }
    else
    {
        buffer[0] = pWidth;
        buffer[1] = pHeight;
        buffer[2] = pWidth;
        buffer[3] = 0.0f;
        buffer[4] = 0.0f;
        buffer[5] = 0.0f;
        buffer[6] = 0.0f;
        buffer[7] = pHeight;
    }
}

void parallax_image::scrollTo(vector2d position)
{
    glUniform1f(this->offsetxLocation, position.x * this->parallaxRatex / this->stretch);
    glUniform1f(this->offsetyLocation, position.y * this->parallaxRatey / this->stretch);
}