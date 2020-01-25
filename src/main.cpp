#include <mbed.h>
#if defined(DEVICE_CAN) || defined(DOXYGEN_ONLY)

DigitalOut led_r(PB_4); //Red
DigitalOut led_g(PB_5); //Green
DigitalOut led_b(PB_0); //Blue

PwmOut in1(PA_2);
PwmOut in2(PA_3);
// DigitalOut in1(PA_2);    //CW
// DigitalOut in2(PA_3);    //CCW
DigitalOut sleepn(PA_6); //HIGHで駆動可能，LOWで空転
DigitalIn ocln(PA_5);    //目標電流 < 実電流
AnalogOut vref(PA_4);    //目標電流
AnalogIn volage(PA_1);   //現在電圧(Vpower*10k/110k)
AnalogIn aiout(PA_7);    //現在電流

InterruptIn enc_a(PA_8); //エンコーダA
DigitalIn enc_b(PA_9);
// InterruptIn enc_b(PA_9); //エンコーダB

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
long encoder_val = 0;
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
  wait(0.5);

  sleepn = 1;
  in1 = in2 = 1;

  can_stby = 0;

  // wait(1.0);
  // led_r = 1;
  // wait(1.0);
}
void set_current(double current) //if torqu=0 then break mode
{
  vref = min(abs(current), VREF_LIMIT); //vref=電流*Rsense*10
  in1 = current >= 0;                   //cw
  in2 = current <= 0;                   //ccw
}

void send()
{
  led_r = !led_r;
  if (can.write(CANMessage(id + POSITION_OFFSET, send_data, SEND_LEN)))
  {
    led_g = !led_g; //blink red
  }
}
void encoder_rise()
{
  encoder_val += !enc_b - enc_b;
}
void encoder_fall()
{
  encoder_val += enc_b - !enc_b;
}
int main()
{
  init();
  enc_a.rise(&encoder_rise);
  enc_a.fall(&encoder_fall);
  ticker.attach(&send, 0.1); //can send 1 [data/ms]

  CANMessage msg;
  while (1)
  {
    set_current(0.01);
    led_b = !led_b;
    wait(5);
    set_current(-0.01);
    led_b = !led_b;
    wait(5);
    // init();
    if (can.read(msg))
    {
      if ((int)msg.id == (id + CURRENT_OFFSET))
      {
        led_g = !led_g; //blink red
        target_current = (msg.data[1] << 8 | msg.data[0]);
      }
    }
  }
}
#else
#error CAN NOT SUPPORTED
#endif