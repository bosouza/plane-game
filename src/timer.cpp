#include <timer.h>
#include <chrono>
#include <thread>
#define GLEW_STATIC
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#define VERY_NEGATIVE -100000

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

event_timer::event_timer(double period)
{
    this->period = period;
    this->previousTime = VERY_NEGATIVE;
}

void event_timer::start()
{
    this->previousTime = glfwGetTime();
}

bool event_timer::isElapsed()
{
    return glfwGetTime() - this->previousTime > this->period;
}