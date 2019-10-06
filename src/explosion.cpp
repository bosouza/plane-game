#include <explosion.h>
explosion::explosion(vector2d position, sprite *s, float period)
    : t(period)
{
    this->position = position;
    this->s = s;
    this->action = 0;
}

bool explosion::updateAndBind()
{
    this->s->BindAction(this->action);
    if (t.isElapsed())
    {
        t.start();
        this->action++;
        return this->action == (this->s->wQtd * this->s->hQtd);
    }
    return false;
}