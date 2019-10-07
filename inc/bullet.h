#ifndef BULLET_H_DEFINED
#define BULLET_H_DEFINED
#include <game_entity.h>
class bullet : public game_entity
{
private:
    float distanceToLive;
    bool alive;

public:
    int player;
    bullet(float positionx, float positiony, float speed, float angle, float angularVelocity, int player, float distanceToLive);
    ~bullet() {}
    void step(float time);
    bool isAlive();
};

#endif