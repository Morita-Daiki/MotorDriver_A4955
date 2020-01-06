#include <mbed.h>
// #if defined(DEVICE_CAN) || defined(DOXYGEN_ONLY)

DigitalOut led_r(PB_4); //Red
DigitalOut led_g(PB_5); //Green
DigitalOut led_b(PB_0); //Blue
DigitalOut led_tx(PA_12);
DigitalOut led_rx(PA_11);
int main()
{

  while (true)
  {
    led_r = rand() % 2;
    led_g = rand() % 2;
    led_b = rand() % 2;
    led_rx = rand() % 2;
    led_tx = rand() % 2;
    wait(0.1);
  }
}
// DigitalOut in1(PA_2);    //CW
// DigitalOut in2(PA_3);    //CCW
// DigitalOut sleepn(PA_6); //HIGHで駆動可能，LOWで空転
// DigitalIn ocln(PA_5);    //目標電流 < 実電流
// AnalogOut vref(PA_4);    //目標電流
// AnalogIn volage(PA_1);   //現在電圧
// AnalogIn aiout(PA_7);    //現在電流

// InterruptIn enc_a(PA_8); //エンコーダA
// InterruptIn enc_b(PA_9); //エンコーダB

// AnalogIn id_setting(PA_0);      //setting
// CAN can(PA_11, PA_12, 1000000); //CAN
// DigitalOut can_stby(PA_10);     //CAN_EN

// Ticker ticker;

// char counter = 0;
// const double VREF_LIMIT = 2.5 / 3.3; //A4955 VREF_LIMIT

// void motor_init()
// {
//   sleepn = 1;    //wake up
//   in1 = in2 = 1; //break
//   led_r = 1;     //state=break
// }

// void set_current(double current) //if torqu=0 then break mode
// {
//   vref = min(abs(current), VREF_LIMIT); //vref=電流*Rsense*10
//   in1 = current >= 0;                   //cw
//   in2 = current <= 0;                   //ccw
// }

// void send()
// {
//   if (can.write(CANMessage(1337, &counter, 1)))
//   {
//     led_g = !led_g;
//   }
// }

// int main()
// {
//   ticker.attach(&send, 1);
//   CANMessage msg;

//   while (1)
//   {
//     printf("%f", sin(19));
//     if (can.read(msg))
//     {
//       led_r = !led_r;
//     }
//     wait_us(999);
//   }
// }

// #else
// #error CAN NOT SUPPORTED
// #endif