#include <timer.h>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

timer::timer()
{
    this->update();
}

void timer::update()
{
    double currentTime = glfwGetTime();
    this->elapsedTime = currentTime - this->previousTime;
    this->previousTime = currentTime;
}

double timer::getElapsedTime()
{
    return this->elapsedTime;
}
