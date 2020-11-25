//例子介绍：8266作为WiFiServer端，打开TCP调试助手，模拟TCP Client的请求IP地址为192.168.43.151
//TCP通讯传输的是十进制ACSII码，需要转码
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

#define IN1  D2
#define IN2  D3
#define IN3  D4
#define IN4  D5
#define IN5  D6
#define IN6  D7
#define IN7  D8
#define IN8  D9

char pin[8][3]={IN1,IN2,IN3,IN4,IN5,IN6,IN7,IN8};
int dataIn = 0;
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
    print(pin[0]);
}
void loop() 
{
    Wifi_loop();
}


void Stop()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,LOW);
  digitalWrite(IN7,LOW);
  digitalWrite(IN8,LOW);
}
void Go()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  digitalWrite(IN5,HIGH);
  digitalWrite(IN6,LOW);
  digitalWrite(IN7,HIGH);
  digitalWrite(IN8,LOW);
}
void Back()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,HIGH);
  digitalWrite(IN7,LOW);
  digitalWrite(IN8,HIGH);
}
void Left()
{
  
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  digitalWrite(IN5,HIGH);
  digitalWrite(IN6,LOW);
  digitalWrite(IN7,LOW);
  digitalWrite(IN8,HIGH);
}
void Right()
{
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  digitalWrite(IN5,LOW);
  digitalWrite(IN6,HIGH);
  digitalWrite(IN7,HIGH);
  digitalWrite(IN8,LOW);
}

void MotorRun()
{
    //analogWrite();
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
