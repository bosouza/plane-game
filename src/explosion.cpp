#include <explosion.h>

#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

explosion::explosion(vector2d position, sprite *s, unsigned int VAO, float period)
    : t(period)
{
    this->position = position;
    this->s = s;
    this->action = 0;
    this->VAO = VAO;
}

bool explosion::updateAndBind()
{
    this->s->BindAction(this->action);
    glBindVertexArray(this->VAO);
    if (t.isElapsed())
    {
        t.start();
        this->action++;
        return this->action == (this->s->wQtd * this->s->hQtd);
    }
    return false;
}