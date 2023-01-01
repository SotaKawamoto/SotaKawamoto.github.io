//お試し
#include "mbed.h"

DigitalOut gate(D13);
AnalogIn ain(A5);
float kenti(void);
void driveMotor(float,float);
PwmOut pinAFin(D5);
PwmOut pinARin(D6);
PwmOut pinBFin(D9);
PwmOut pinBRin(D10);
DigitalOut STBY(D4);

float kenti(void)
{
DigitalOut trig_pin(D1);
DigitalIn echo_pin(D0);
Timer t;
float duration;
float dist;
        trig_pin = 0;
        wait(0.00002);
        trig_pin = 1;
        wait(0.00002);
        trig_pin = 0;
        while( echo_pin==0 );
        t.start();//high状態カウント開始
        while( echo_pin==1 );
        t.stop();//high状態カウント終了
        duration = t.read_us();//echoからのhigh状態の時間をマイクロ秒で読み込んでいる
        if(duration > 0)
        {
            dist = duration / 2; //単位はマイクロ秒 片道の時間を示す
            dist = dist * 0.34;　//音速（mm/us)×時間 = 距離（mm)を示す
            printf("距離は%.2fmm\n",dist);
        }
        t.reset();
        return dist;
}

void driveMotor(float speedA , float speedB)
{
    float outputA = abs(speedA);
    float outputB = abs(speedB);
    if(speedA>0)
      {
        pinAFin=outputA;
        pinARin=0;
      }
    else if (speedA<0)
     {
        pinAFin=0;
        pinARin=outputA;
     }
    else
     {
        pinAFin=0;
        pinARin=0;
    }
     if(speedB>0)
    {
        pinBFin=outputB;
        pinBRin=0;
    }
    else if (speedB<0)
    {
        pinBFin=0;
        pinBRin=outputB;
    }
   else
   {
        pinBFin=0;
        pinBRin=0;
    }
}

int main()
{
    while(1)//着地判定・切断
　　{
        wait_us(1);
        if(ain>0.1){
            printf("Water Detection");
　　      　 wait(1);//切断までの時間をいれる。
            printf("開始");
            gate = 1;
            wait(10);//切断中
            gate = 0;
            printf("終了");
           break;
           }
        else{
            printf("No Data");
        wait_us(1);
        }
  　}
    while(ain > 0.1)//水上でのみ移動
    {
          STBY = 1;
          if(kenti()> 400)//400mm手前を閾値とした場合
　　      {
          driveMotor(0,0);
          driveMotor(0.5,0.5);
          }
          else if(kenti() <400)
          {
          driveMotor(0,0);
          driveMotor(-0.5,0.5);
          }
          else
          {
          driveMotor(0,0);
          driveMotor(0.5,0.5);
          }
    }
　}
