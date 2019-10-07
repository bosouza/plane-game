#ifndef PLANE_H_DEFINED
#define PLANE_H_DEFINED
#include <game_entity.h>

class plane : public game_entity
{
private:
    int damage;
    int lives;
    float fallingSpinVelocity;

public:
    bool exploded;
    plane(float positionx, float positiony, float speed, float angle, float angularVelocity, int lives, float fallingSpinVelocity);
    ~plane() {}
    bool isFalling();
    void hit();
    void step(float time);
};

#endif