#include <PubSubClient.h>
#include <ESP8266WiFi.h>

String clientID = "2";//Arduino 客户端的ID
// Create a random client ID
//String clientId = "ESP8266Client-";
//clientId += String(random(0xffff), HEX);
const char *ssid     = "TP410";//这里写入网络的ssid
const char *password = "8littleindianboys";//wifi密码
const char *mqtt_server = "47.93.55.173";
String data_topic = String("MyPlant/Arduino/data/" + clientID);
String instruction_topic = String("MyPlant/Arduino/instruction/" + clientID);

WiFiClient espClient;
PubSubClient client(espClient);
//long lastMsg = 0;//时间间隔的参照
//int value = 0;//消息编号

void setup_wifi() {//连接wifi
  delay(10);
  // We start by connecting to a WiFi network
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  randomSeed(micros());
  //Serial.println("");
  //Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());//获取IP
}

void callback(char* topic, byte* payload, unsigned int length) {//回调函数，处理接收到的来自服务器的消息  
  //读取的数据转发到串口*************************************************************
  Serial.write(payload, length);
  /*
    while (client.available())//读取到的数据转发到到串口
    {
      uint8_t c = client.read();
      Serial.write(c);
    }
*/
}

void reconnect() {// Loop until we're reconnected
  while (!client.connected()) {
    //Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientID.c_str())) {
      //Serial.println("connected");
      client.subscribe(instruction_topic.c_str());//订阅指令
    } else {
      //Serial.print("failed, rc=");
      //Serial.print(client.state());
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  delay(10);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //从串口读数据************************************************************
  if (Serial.available()) {
    String msg = "";
    size_t counti = Serial.available();
    byte sbuf[counti];
    Serial.readBytes(sbuf, counti);
    delay(10);
    for(int i = 0; i < sizeof(sbuf); i++){
      msg = msg + String((char)sbuf[i]);
     // msg = msg + String(sbuf[i]) + " ";
  }
    client.publish(data_topic.c_str(), msg.c_str());//发布数据
  }
  delay(3000);
  }
 
 
  

  /*
    if (Serial.available())//串口读取到的转发到wifi，因为串口是一位一位的发送所以在这里缓存完再发送
    {
      size_t counti = Serial.available();
      uint8_t sbuf[counti];
      Serial.readBytes(sbuf, counti);
      client.write(sbuf, counti);

    }
  */
  /*//固定间隔发布消息
    long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      snprintf (msg, 75, "hello world #%ld", value);
      Serial.print("Publish message: ");
      Serial.println(msg);
      client.publish("test", msg);
    }
  */


