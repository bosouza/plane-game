#ifndef TIMER_H_DEFINED
#define TIMER_H_DEFINED
class timer
{
private:
    double minPeriod;
    double previousTime;
    double elapsedTime;

public:
    timer(float FPSmax);
    ~timer() {}
    void update();
    double getElapsedTime();
};

#endif