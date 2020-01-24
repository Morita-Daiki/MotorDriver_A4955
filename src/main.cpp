#include <mbed.h>
#if defined(DEVICE_CAN) || defined(DOXYGEN_ONLY)

DigitalOut led_r(PB_4); //Red
DigitalOut led_g(PB_5); //Green
DigitalOut led_b(PB_0); //Blue

DigitalOut in1(PA_2);    //CW
DigitalOut in2(PA_3);    //CCW
DigitalOut sleepn(PA_6); //HIGHで駆動可能，LOWで空転
DigitalIn ocln(PA_5);    //目標電流 < 実電流
AnalogOut vref(PA_4);    //目標電流
AnalogIn volage(PA_1);   //現在電圧
AnalogIn aiout(PA_7);    //現在電流

InterruptIn enc_a(PA_8); //エンコーダA
InterruptIn enc_b(PA_9); //エンコーダB

AnalogIn id_setting(PA_0);      //setting
CAN can(PA_11, PA_12, 1000000); //CAN
DigitalOut can_stby(PA_10);     //CAN_EN

Ticker ticker;

const double VREF_LIMIT = 2.5 / 3.3; //A4955 VREF_LIMIT
#define SET_INDEX 0x10
#define SEND_INDEX 0x20
#define DATA_LEN 8
int id;
char read_data[DATA_LEN];
char send_data[DATA_LEN];
void init()
{
  id = set_id();
}
int set_id()
{
  double id = 0.0;
  const int loop_size = 100;
  for (int i = 0; i < loop_size; i++)
  {
    id += id_setting * 10.0;
  }
  return (int)(id / loop_size + 0.5); //四捨五入で0~10をreturn
}

void send()
{
  if (can.write(CANMessage(id + SEND_INDEX, send_data, DATA_LEN)))
  {
    led_r = !led_r;
  }
}

int main()
{
  init();

  CANMessage msg;
  while (1)
  {
    if (can.read(msg))
    {
      if (msg.id == id + SET_INDEX)
      {
      }
    }
  }
}

#else
#error CAN NOT SUPPORTED
#endif