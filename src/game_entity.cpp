#include <game_entity.h>
#include <math.h>

game_entity::game_entity(float positionx, float positiony, float speed, float angle, float angularVelocity)
{
    this->position.x = positionx;
    this->position.y = positiony;
    this->speed = speed;
    this->angle = angle;
    this->angularVelocity = angularVelocity;
    this->travelledDistance = 0;
}

void game_entity::step(float time)
{
    this->angle += angularVelocity * time;
    this->travelledDistance += fabs(this->speed) * time;
    this->position.x += this->speed * cos(angle) * time;
    this->position.y += this->speed * sin(angle) * time;
}