String request = "01 03 00 00 00 02 C4 0B";
int led = 13;    //定义LED所在灯管脚
unsigned char req[8] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x02, 0xC4, 0x0B};
unsigned char rec[10];
int EN = 2;    //定义RS485扩展板的使能端（2号数字IO口），高电平为发送状态，低电平为接收状态
void setup()
{
  Serial.begin(9600);     //初始化串口
  pinMode(led,OUTPUT);
  pinMode(EN,OUTPUT);
}
void loop()
{
  read_command();
  if(Serial.available() > 0){
    data_return();
  }
  delay(8000);
}

void read_command()
{
  int i;
  digitalWrite(EN,HIGH);//发送
  delay(10);
  for(i = 0; i < 8; i++){
    Serial.write(req[i]);
    Serial.flush();
    delay(10);
  }
  digitalWrite(EN,LOW);//接收
  digitalWrite(led,HIGH);
  delay(5000);
  digitalWrite(led,LOW);
}
void data_return()
{
  int i;
  digitalWrite(EN,LOW);//接收
  delay(10);
  for(i = 0; i < 10; i++){
    if(Serial.available() > 0){
      rec[i] = Serial.read();
      delay(20);
      Serial.print(rec[i],HEX);
      Serial.print(" ");
    }
  }
  Serial.println();
  Serial.println("One detection finished");
  digitalWrite(led,HIGH);
  delay(3000);
  digitalWrite(led,LOW);
  digitalWrite(EN,HIGH);//发送
}

