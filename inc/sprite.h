#ifndef SPRITE_H_DEFINED
#define SPRITE_H_DEFINED
#include <string>
class sprite
{
private:
    unsigned int textureID;
    int offsetxLocation;
    int offsetyLocation;

public:
    unsigned int wQtd;
    unsigned int hQtd;
    sprite(std::string path, unsigned int wQtd, unsigned int hQtd, int offsetxLocation, int offsetyLocation);
    ~sprite() {}
    void BindAction(unsigned int action);
    void FillTextureBuffer(float buffer[]);
};
#endif