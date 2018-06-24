#include <SoftwareSerial.h>
#include <DFRobot_SHT20.h>
#include <ArduinoJson.h>
SoftwareSerial softSerial(7, 8);
String sendMsg = "";//发送的数据
String receiveMsg = "";//接收的指令
DFRobot_SHT20 sht20;
float humidity;
float temperature;
int ASignal = A0;
int soil;
int Relay = 3;
int seconds;
int clientID = 2;
int detectInterval;


void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.listen();
  sht20.initSHT20();
  delay(100);
  sht20.checkSHT20();
  pinMode(ASignal, INPUT);
  pinMode(Relay, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly
  DetectAirCondition(humidity, temperature);
  soil = DetectSoilMoisture();
  sendMsg = GetData(temperature, humidity, soil);
  //Serial.println(sendMsg);
  SendData(sendMsg);
  receiveMsg = ReceiveInstruction();
  if(receiveMsg.length() != 0){
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(receiveMsg);
    if(!root.success()){
      //Serial.println("parseObject failed!");
    }
    else{
      int id = root["clientID"];
      int WF = root["WF"];
      int water = root["water"];
      detectInterval = root["detectInterval"];
      //seconds = (water * 10) / 500;
      seconds = water;
      if( WF== 1){
        PumpWork(seconds);
        delay(seconds * 1000);
      }
    }
  }
  sendMsg = "";
  delay(detectInterval * 1000);
}


void SendData(String someData){//从WIFI模块发送数据
 softSerial.print(someData);
 delay(2000);
}


String ReceiveInstruction(){//从WIFI模块接收数据
  String instruction = "";
  if(softSerial.available()){
    size_t counti = softSerial.available();
    byte buf[counti];
    softSerial.readBytes(buf, counti);
    for(int i = 0; i < counti; i++){
      instruction += String((char)buf[i]);
    }
  }
  delay(2000);
  return instruction;
}

void DetectAirCondition(float &humd, float &temp){//检测空气温湿度
  humd = sht20.readHumidity();
  temp = sht20.readTemperature();
  delay(1000);
}

int DetectSoilMoisture(){//检测土壤湿度
  return analogRead(ASignal);
}

void PumpWork(int second){//控制水泵工作
  digitalWrite(Relay, HIGH);
  delay(second * 1000);
  digitalWrite(Relay, LOW);
}

String GetData(float AT, float AM, int SM){//发送前形成数据字符串
  return String("{\"clientID\":" + String(clientID) + ",\"AT\":" + String(AT) + ",\"AM\":" + String(AM) + ",\"SM\":" + String(SM) + "}");
}



