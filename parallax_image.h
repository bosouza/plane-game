#ifndef PARALLAX_IMAGE_H_DEFINED
#define PARALLAX_IMAGE_H_DEFINED
#include <string>
#include <game_entity.h>
class parallax_image
{
private:
    std::string path;
    float parallaxRatex;
    float parallaxRatey;
    float stretch;
    int offsetxLocation;
    int offsetyLocation;
    unsigned int VAO;
    unsigned int textureID;
    int width;
    int height;

public:
    parallax_image(std::string path, float parallaxRatex, float parallaxRatey, float stretch, int offsetxLocation, int offsetyLocation);
    ~parallax_image() {}
    void bind();
    void scrollTo(vector2d position);
    void fillTextureBuffer(float buffer[]);
};

#endif