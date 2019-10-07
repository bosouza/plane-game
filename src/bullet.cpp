#include <bullet.h>

bullet::bullet(float positionx, float positiony, float speed, float angle, float angularVelocity, int player, float distanceToLive)
    : game_entity(positionx, positiony, speed, angle, angularVelocity)
{
    this->player = player;
    this->distanceToLive = distanceToLive;
    this->alive = true;
}

void bullet::step(float time)
{
    this->game_entity::step(time);
    if (this->travelledDistance > this->distanceToLive)
        this->alive = false;
}

bool bullet::isAlive()
{
    return this->alive;
}