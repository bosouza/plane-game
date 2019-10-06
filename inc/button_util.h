#ifndef BUTTON_UTIL_H_DEFINED
#define BUTTON_UTIL_H_DEFINED
class press_event
{
private:
    bool state;
    unsigned int eventCnt;

public:
    press_event();
    void press();
    void release();
    void setState(bool state);
    bool pressEvent();
};
#endif