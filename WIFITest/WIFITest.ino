#include <SoftwareSerial.h>
String comdata = "";    // 字符串缓冲区
SoftwareSerial softSerial(7, 8);
unsigned long lastDebounceTime = 0;
String sendMsg = "this is some data";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  softSerial.begin(9600);
  softSerial.listen();
}

void loop() {
  Serial.println("Sending some data: " + sendMsg);
  softSerial.print(sendMsg);
  if(softSerial.available()){
    while (softSerial.available() > 0)
      {
        size_t counti = softSerial.available();
        byte buf[counti];
        softSerial.readBytes(buf, counti);
        for(int i = 0; i < counti; i++){
          comdata += String((char)buf[i]);
        }
      }
      Serial.println("Receiving some data*****************: " + comdata);
  }
  comdata = "";
  delay(10000);
}

