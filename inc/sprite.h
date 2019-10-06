#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED
#include <string>
class sprite
{
private:
    unsigned int textureID;
    unsigned int wQtd;
    unsigned int hQtd;
    int offsetxLocation;
    int offsetyLocation;

public:
    sprite(std::string path, unsigned int wQtd, unsigned int hQtd, int offsetxLocation, int offsetyLocation);
    ~sprite() {}
    void BindAction(unsigned int action);
    void FillTextureBuffer(float buffer[]);
};
#endif