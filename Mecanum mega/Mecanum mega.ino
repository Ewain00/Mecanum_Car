//例子介绍：8266作为WiFiServer端，打开TCP调试助手，模拟TCP Client的请求IP地址为192.168.43.151
//麦克纳姆轮为O-长方形型
//TCP通讯传输的是十进制ACSII码，需要转码
/*                   车头
            Motor1          Motor2
            Motor3          Motor4
*/
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <math.h>
#include <WString.h>

using namespace std;
#define a 10
#define b 20


class MotorPin
{
    public:
    char x;//引脚1
    char y;//引脚2
}Motor1,Motor2,Motor3,Motor4;


class CarSpeed
{
    public:
    double Vx, Vy, W;//Vx，Vy小车实际速度，Vx向右为正，Vy向前为正，AngularVelocity小车绕中心旋转速度，逆时针为正
    double Vw1,Vw2,Vw3,Vw4;//各个轮子速度
    double Vtx,Vty;//小车速度分量
    friend CarSpeed GetWheelSpeed(CarSpeed Car, int OffsetAngle, int RotationAngle, int Speed);
}WheelSpeed;


void setup() 
{
    Motor1.x=D2;Motor1.y=D3;
    Motor2.x=D4;Motor2.y=D5;
    Motor3.x=D6;Motor3.y=D7;
    Motor4.x=D8;Motor4.y=D9;//定义电机引脚,类的变量不能在全局定义
    Serial.begin(115200);
    pinMode(Motor1.x, OUTPUT);
    pinMode(Motor1.y, OUTPUT);
    pinMode(Motor2.x, OUTPUT);
    pinMode(Motor2.y, OUTPUT);
    pinMode(Motor3.x, OUTPUT);
    pinMode(Motor3.y, OUTPUT);
    pinMode(Motor4.x, OUTPUT);
    pinMode(Motor4.y, OUTPUT);
}
void loop() 
{
    MotorRun(200,100,0,150);
    delay(1000);
    MotorRun(200,-100,0,150);
    delay(1000);
    MotorRun(0,0,0,0);
    delay(1000);
    MotorRun(-200,-50,0,150);
    delay(1000);
    MotorRun(-200,50,0,150);
    delay(1000);
}

void SingleMotorRun(MotorPin Motor, int Speed)//Direction:0为正转，1为反转,单个电机的控制函数
{
    if(Speed >= 0)
    {
        analogWrite(Motor.x,Speed);
        analogWrite(Motor.y,LOW);
        Serial.print("Single Speed:");
        Serial.println(Speed);
    }//正转
    else
    {
        analogWrite(Motor.y,abs(Speed));
        analogWrite(Motor.x,LOW);
    }

    /*if(Speed >= 0)
    {
        digitalWrite(Motor.x,HIGH);
        digitalWrite(Motor.y,LOW);
        Serial.print("Single Speed:");
        Serial.println(Speed);
    }//正转
    else
    {
        digitalWrite(Motor.y,abs(HIGH));
        digitalWrite(Motor.x,LOW);
    }*/
    
}
void MotorRun(double OffsetAngle, double RotationAngle, double Speed)//
{
    WheelSpeed = GetWheelSpeed(WheelSpeed,OffsetAngle, RotationAngle, Speed);
    SingleMotorRun(Motor1,int(WheelSpeed.Vw1));
    SingleMotorRun(Motor2,int(WheelSpeed.Vw2));
    SingleMotorRun(Motor3,int(WheelSpeed.Vw3));
    SingleMotorRun(Motor4,int(WheelSpeed.Vw4));
    Serial.print("W1=");
    Serial.println(WheelSpeed.Vw1);
    Serial.print("W2=");
    Serial.println(WheelSpeed.Vw2);
    Serial.print("W3=");
    Serial.println(WheelSpeed.Vw3);
    Serial.print("W4=");
    Serial.println(WheelSpeed.Vw4);
}

CarSpeed GetWheelSpeed(CarSpeed Car, double OffsetAngle, double RotationAngle, double Speed)//Direction:0为正转，1为反转,OffsetAngle为小车平移方向角度，RotationAngle为小车绕中心旋转角度
{
    Car.W = RotationAngle;
    Car.Vtx = Vx;
    Car.Vty = Vy;
    Car.Vw1 = Car.Vty - Car.Vtx + Car.W*(a + b);
    Car.Vw2 = Car.Vty + Car.Vtx - Car.W*(a + b);
    Car.Vw3 = Car.Vty - Car.Vtx - Car.W*(a + b);
    Car.Vw4 = Car.Vty + Car.Vtx + Car.W*(a + b);
    Car.Vx = Car.Vtx + Car.W*b;
    Car.Vy = Car.Vty - Car.W*a;
    if(Car.Vw1>=255)
    {
        Car.Vw1 = 255;
    }
    if(Car.Vw2>=255)
    {
        Car.Vw2 = 255;
    }
    if(Car.Vw3>=255)
    {
        Car.Vw3 = 255;
    }
    if(Car.Vw4>=255)
    {
        Car.Vw4 = 255;
    }
    ///
    ///
    if(Car.Vw1<=-255)
    {
        Car.Vw1 = -255;
    }
    if(Car.Vw2<=-255)
    {
        Car.Vw2 = -255;
    }
    if(Car.Vw3<=-255)
    {
        Car.Vw3 = -255;
    }
    if(Car.Vw4<=-255)
    {
        Car.Vw4 = -255;
    }
    return Car;
}
