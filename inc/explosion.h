#ifndef EXPLOSION_H_INCLUDED
#define EXPLOSION_H_INCLUDED
#include <game_entity.h>
#include <timer.h>
#include <sprite.h>

class explosion
{
private:
    event_timer t;
    sprite *s;
    unsigned int action;
    unsigned int VAO;

public:
    vector2d position;
    explosion(vector2d position, sprite *s, unsigned int VAO, float period);
    ~explosion() {}
    bool updateAndBind();
};
#endif