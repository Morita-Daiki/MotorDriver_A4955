#ifndef MOTORCONTROL4955_H
#define MOTORCONTROL4955_H

#include <mbed.h>

class MotorControl4955
{
public:
    MotorControl4955(PinName in1, PinName in2, PinName sleepn, PinName ocln, PinName vref, PinName voltage, PinName aiout);
    setCurrent(float amp);

private:
    DigitalOut _in1, _in2, _sleepn;
    DigitalIn _ocln;
    AnalogOut _vref;
    AnalogIn _voltage, _aiout;
}

#endif