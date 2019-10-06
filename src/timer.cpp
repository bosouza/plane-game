#include <timer.h>
#include <chrono>
#include <thread>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"

timer::timer(float FPSmax)
{
    if (FPSmax != 0)
        this->minPeriod = 1 / FPSmax;
    else
        this->minPeriod = 0;
    this->update();
}

void timer::update()
{
    double currentTime = glfwGetTime();
    while (currentTime - this->previousTime < this->minPeriod)
    {
        std::this_thread::sleep_for(std::chrono::nanoseconds(0));
        currentTime = glfwGetTime();
    }
    this->elapsedTime = currentTime - this->previousTime;
    this->previousTime = currentTime;
}

double timer::getElapsedTime()
{
    return this->elapsedTime;
}
