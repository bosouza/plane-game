#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED
class timer
{
private:
    double previousTime;
    double elapsedTime;

public:
    timer();
    ~timer() {}
    void update();
    double getElapsedTime();
};

#endif