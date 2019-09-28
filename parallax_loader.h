#ifndef PARALLAX_LOADER_H_DEFINED
#define PARALLAX_LOADER_H_DEFINED
#include <string>
#include <iostream>
typedef struct
{
    std::string path;
    float parallaxRatex;
    float parallaxRatey;
    float stretch;
    unsigned int VAO;

    //attributes overwritten by LoadImages function

    unsigned int textureId;
    unsigned int width;
    unsigned int height;
    //mapping is two floats (x,y) for each corner (top-right, bottom-right, bottom-left, top-left)
    float maping[8];
} parallaxImage;

bool LoadImages(parallaxImage images[], unsigned int size);

#endif