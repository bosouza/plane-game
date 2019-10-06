#include <button_util.h>

press_event::press_event()
{
    this->state = false;
    this->eventCnt = 0;
}

void press_event::press()
{
    if (this->state == false)
    {
        this->state = true;
        this->eventCnt++;
    }
}

void press_event::release()
{
    this->state = false;
}

void press_event::setState(bool state)
{
    state ? this->press() : release();
}

bool press_event::pressEvent()
{
    if (this->eventCnt > 0)
    {
        this->eventCnt--;
        return true;
    }
    return false;
}