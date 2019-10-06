#ifndef GAME_ENTITY_H_DEFINED
#define GAME_ENTITY_H_DEFINED

typedef struct
{
    float x;
    float y;
} vector2d;

class game_entity
{
protected:
    float travelledDistance;

public:
    float speed;
    float angle;
    float angularVelocity;
    bool visible;
    vector2d position;
    game_entity(float positionx, float positiony, float speed, float angle, float angularVelocity);
    ~game_entity() {}
    void step(float time);
};

#endif