#include <mbed.h>

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

double now_current = 0.0;    //現在でんりゅう
double target_current = 0.0; //目標電流
#define CURRENT_OFFSET 0x10  //current setting id offset
#define POSITION_OFFSET 0x20 //position setting id offset
#define READ_LEN 1
#define SEND_LEN 8
int id; //0~7
double idsumval = 0.0;
char read_data[READ_LEN];
char send_data[SEND_LEN];

int set_id()
{
  double id_sum = 0.0;
  const int loop_size = 100;
  for (int i = 0; i < loop_size; i++)
    id_sum += id_setting;
  idsumval = id_sum;
  return (int)(id_sum / loop_size * 7.9);
}
void init()
{
  id = set_id();
  led_r = (id >> 0) & 1;
  led_g = (id >> 1) & 1;
  led_b = (id >> 2) & 1;
}
void send()
{
  if (can.write(CANMessage(id + POSITION_OFFSET, send_data, SEND_LEN)))
  {
    // led_r = !led_r; //blink red
  }
}

int main()
{
  init();
  ticker.attach(&send, 0.01);

  CANMessage msg;
  while (1)
  {
    init();
    if (can.read(msg))
    {
      if ((int)msg.id == (id + CURRENT_OFFSET))
      {
        // led_g = !led_g; //blink red
        target_current = (msg.data[1] << 8 | msg.data[0]);
      }
    }
  }
}
