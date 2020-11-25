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

#define IN1  D2
#define IN2  D3
#define IN3  D4
#define IN4  D5
#define IN5  D6
#define IN6  D7
#define IN7  D8
#define IN8  D9

class MotorPin
{
    public:
    char x;//引脚1
    char y;//引脚2
}Motor1,Motor2,Motor3,Motor4;
MotorPin Motor1;
MotorPin Motor2;
MotorPin Motor3;
MotorPin Motor4;
Motor1.x=D2;Motor1.y=D3;
Motor2.x=D4;Motor2.y=D5;
Motor3.x=D6;Motor3.y=D7;
Motor4.x=D8;Motor4.y=D9;//定义电机引脚

class CarSpeed
{
    double Vx, Vy, W;//Vx，Vy小车实际速度，Vx向右为正，Vy向前为正，AngularVelocity小车绕中心旋转速度，逆时针为正
    double Vw1,Vw2,Vw3,Vw4;//各个轮子速度
    double Vtx,Vty;//小车速度分量
    void GetWheelSpeed(int OffsetAngle, int RotationAngle, int Speed);
}WheelSpeed;


int dataIn = 0;//储存小车接受到的控制信号
const char* ssid = "abc";
const char* password = "12345678";
#define MAX_SRV_CLIENTS 3  //最大客户端连接数
WiFiServer server(23);    //创建server 端口号是23

WiFiClient serverClients[MAX_SRV_CLIENTS];   //定义最多多少个client可以连接本server(一般不要超过4个)
 

void setup() {
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(IN5, OUTPUT);
    pinMode(IN6, OUTPUT);
    pinMode(IN7, OUTPUT);
    pinMode(IN8, OUTPUT);
    Wifi_start();
}
void loop() 
{
    Wifi_loop();
}

void Wifi_start()
{
    Serial.begin(115200);
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
          while (serverClients[i].available()) {
            dataIn = serverClients[i].read();
            if (dataIn==49) {
              Go();   
            }
            else if (dataIn==50) {
              Back();
            }
            else if (dataIn==51) {
              Right();
            }
            else if (dataIn==52) {
              Left();
            }
            else {
              Stop();
            }
        }
            Serial.print(dataIn);
            Serial.println();
            Serial.print("HIGH = ");
            Serial.println(HIGH);

          }
        }
      }
}

void SingleMotorRun(struct Motor, int Speed)//Direction:0为正转，1为反转,单个电机的控制函数
{
    if(Speed >= 0)
    {
        analogWrite(struct.x,Speed);
        analogWrite(struct.y,LOW);
    }//正转
    else
    {
        analogWrite(struct.y,Speed);
        analogWrite(struct.x,LOW);
    }
    
    
}
void MotorRun(int OffsetAngle, int RotationAngle, int Speed)//
{
    Car = GetWheelSpeed(OffsetAngle, RotationAngle, Speed);
    SingleMotorRun(Motor1,Direction,Car.Vw1);
    SingleMotorRun(Motor2,Direction,Car.Vw2);
    SingleMotorRun(Motor3,Direction,Car.Vw3);
    SingleMotorRun(Motor4,Direction,Car.Vw4);
}


void WheelSpeed::GetWheelSpeed(int OffsetAngle, int RotationAngle, int Speed)//Direction:0为正转，1为反转,OffsetAngle为小车平移方向角度，RotationAngle为小车绕中心旋转角度
{
    WheelSpeed Car;
    Car.W = RotationAngle;
    Car.Vtx = Speed * cos(OffsetAngle);
    Car.Vty = Speed * sin(OffsetAngle);
    Car.Vw1 = Vty - Vtx + W*(a + b);
    Car.Vw2 = Vty + Vtx _ W*(a + b);
    Car.Vw3 = Vty - Vtx _ W*(a + b);
    Car.Vw4 = Vty + Vtx + W*(a + b);
    Car.Vx = Vtx + W*b;
    Car.Vy = Vty - W*a;
    return Car;
}
