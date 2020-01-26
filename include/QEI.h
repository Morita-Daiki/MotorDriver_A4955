//https://os.mbed.com/users/kikoaac/code/QEI2/　をほぼコピペ

#ifndef QEI_H
#define QEI_H

#include "mbed.h"

#define PREV_MASK 0x1 //Mask for the previous state in determining direction of rotation.
#define CURR_MASK 0x2 //Mask for the current state in determining direction of rotation.
#define INVALID 0x3   //XORing two states where both bits have changed.

class QEI
{
protected:
    PinName Pin[2];
    QEI(const QEI &q);
    QEI &operator=(const QEI &q)
    {
        return *this;
    }

public:
    typedef enum Encoding
    {
        X2_ENCODING = 2,
        X4_ENCODING = 4
    } Encoding;

    QEI(PinName channelA, PinName channelB, int pulsesPerRev, Timer *T, Encoding encoding = X2_ENCODING);

    void qei_reset(void);

    int getCurrentState(void);

    void set(int pul, int rev);

    int getPulses(void);

    int getRevolutions(void);

    int getAng_rev();

    double getAngle();
    double getSumangle();
    double getRPM();
    double getRPS();
    double getRPMS();
    double getRPUS();
    int pulsesPerRev_;
    void state(int i);

private:
    Timer *timer;
    //Ticker Tick;
    double RPM, RPS, RPMS, RPUS;
    float gettime()
    {
        timer->start();
        static float prev_time;
        float a = timer->read() - prev_time;
        prev_time = timer->read();
        return a;
    }

    void encode(void);

    Encoding encoding_;

    InterruptIn channelA_;
    InterruptIn channelB_;
    int round_rev;

    int prevState_;
    int currState_;
    double angle_, sumangle;
    int angle_pulses;
    volatile int pulses_;
    volatile int revolutions_;
};

#endif