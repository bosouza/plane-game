#include <plane.h>
#include <math.h>

plane::plane(float positionx, float positiony, float speed, float angle, float angularVelocity, int lives, float fallingSpinVelocity)
    : game_entity(positionx, positiony, speed, angle, angularVelocity)
{
    this->exploded = false;
    this->damage = 0;
    this->lives = lives;
    this->fallingSpinVelocity = fallingSpinVelocity;
}

void plane::step(float time)
{
    if (this->isFalling())
        this->angularVelocity = cos(this->angle) > 0 ? -fallingSpinVelocity : fallingSpinVelocity;
    this->game_entity::step(time);
}

void plane::hit()
{
    this->damage++;
}

bool plane::isFalling()
{
    return this->damage >= this->lives;
}