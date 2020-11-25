//例子介绍：8266作为WiFiServer端，打开TCP调试助手，模拟TCP Client的请求IP地址为192.168.43.151
//麦克纳姆轮为O-长方形型
//TCP通讯传输的是十进制ACSII码，需要转码
/*                   车头               ↑X
            Motor1          Motor2       →Y
            Motor3          Motor4
*/
#include <SoftwareSerial.h>
//#include <ESP8266WiFi.h>
#include <math.h>
#include <WString.h>

using namespace std;
#define a 10
#define b 20
#define IN1  2
#define IN2  3
#define IN3  4
#define IN4  6
#define IN5  7
#define IN6  8
#define IN7  9
#define IN8  10
int commaPosition; //分割符位置

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

const char* ssid = "Ewain2";//cfk
const char* password = "27188266";//18318501236
#define MAX_SRV_CLIENTS 3  //最大客户端连接数
//WiFiServer server(23);    //创建server 端口号是23

//WiFiClient serverClients[MAX_SRV_CLIENTS];   //定义最多多少个client可以连接本server(一般不要超过4个)
 

void setup() {
    Motor1.x=2;Motor1.y=3;
    Motor2.x=4;Motor2.y=6;
    Motor3.x=7;Motor3.y=8;
    Motor4.x=9;Motor4.y=10;//定义电机引脚,类的变量不能在全局定义
    Serial.begin(115200);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);
    pinMode(IN8, OUTPUT);
    //Wifi_start();
}
void loop() 
{
    //MotorRun(0,0,150);
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

/*void Wifi_start()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("\n连接到:"); 
    Serial.println(ssid);
    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED && i++ < 20) {
      delay(500);
    }
    if (i == 21) {
      Serial.print("没能连接到:"); 
      Serial.println(ssid);
      return ;
    }
    
    server.begin();  //启动server
    server.setNoDelay(true);//关闭小包合并包功能，不会延时发送数据
    
    Serial.print("准备好了！使用网络吧 IP是: ");
    Serial.print(WiFi.localIP());
    Serial.println("      端口是： 23");
}
void Wifi_loop()
{
    uint8_t i;
    if (server.hasClient()) {  //判断是否有新的client请求进来
      for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      
        //释放旧无效或者断开的client
        if (!serverClients[i] || !serverClients[i].connected()) {
          if (!serverClients[i]) {
            //serverClients[i]    判断指定序号的客户端是否有效
            serverClients[i].stop();  //停止指定客户端的连接
          }

          serverClients[i] = server.available();//分配最新的client
          Serial.print("1个新的客户端: "); 
          Serial.println(i+1);
          break; //跳出一层for循环
        }
      }
    
      //当达到最大连接数 无法释放无效的client，需要拒绝连接
      if (i == MAX_SRV_CLIENTS) {
        WiFiClient client = server.available();
        client.stop();
        Serial.println("连接被拒绝 ");
      }
    }
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
        if (serverClients[i] && serverClients[i].connected()) {
            if (serverClients[i].available()) {
                Serial.print("接受的数据:");
                //serverClients[i].available()   判断指定客户端是否有可读数据
                while (serverClients[i].available())
                {
                    String Angle;
                    String Speed;
                    String line = serverClients[i].readStringUntil('\n');
                    Serial.println(line);
                    commaPosition = line.indexOf(',');  
                    if (commaPosition != -1)
                    {
                        Angle = line.substring(0,commaPosition);//打印取第一个字符
                        Speed = line.substring(commaPosition+1, line.length());
                        //Double.parseDouble();
                    }
                    //Angle = line.substring(0,commaPosition);//打印取第一个字符
                    //Speed = line.substring(commaPosition+1, line.length());
                    //Double.parseDouble();
                    Serial.print("Angle=");
                    Serial.println(Angle);
                    Serial.print("Speed=");
                    Serial.println(Speed);
                    delay(100);
                    MotorRun(Angle.toDouble(),0,Speed.toDouble());
                    //MotorRun(0,0,0);
                }  
                    //dataIn = serverClients[i].read();//read返回的是int类型
                    /*if (dataIn==49) {

                    }
                    else if (dataIn==50) {

                    }
                    else if (dataIn==51) {

                    }
                    else if (dataIn==52) {
                    }
                    else {
                    }
            }
        }
    }   
}*/
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
void MotorRun(double Vx, double Vy, double RotationAngle, double Speed)//
{
    WheelSpeed = GetWheelSpeed(WheelSpeed,Vx, Vy, RotationAngle, Speed);
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

CarSpeed GetWheelSpeed(CarSpeed Car, double Vx, double Vy, double RotationAngle, double Speed)//Direction:0为正转，1为反转,OffsetAngle为小车平移方向角度，RotationAngle为小车绕中心旋转角度
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
    if(Car.Vw1>=240)
    {
        Car.Vw1 = 240;
    }
    if(Car.Vw2>=240)
    {
        Car.Vw2 = 240;
    }
    if(Car.Vw3>=240)
    {
        Car.Vw3 = 240;
    }
    if(Car.Vw4>=240)
    {
        Car.Vw4 = 240;
    }
    ///
    ///
    if(Car.Vw1<=-240)
    {
        Car.Vw1 = -240;
    }
    if(Car.Vw2<=-240)
    {
        Car.Vw2 = -240;
    }
    if(Car.Vw3<=-240)
    {
        Car.Vw3 = -240;
    }
    if(Car.Vw4<=-240)
    {
        Car.Vw4 = -240;
    }
    return Car;
}
