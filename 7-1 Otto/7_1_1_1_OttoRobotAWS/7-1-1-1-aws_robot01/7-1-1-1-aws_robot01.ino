// Example of the different modes of the X.509 validation options
// in the WiFiClientBearSSL object
//
// Jul 2019 by Taejun Kim at www.kist.ac.kr
// arduino version 1.8.15

#include <ESP8266WiFi.h>
#include <PubSubClient.h> // version 2.7.0
#include <time.h>
#include <ArduinoJson.h>
#include "credentials.h"
#include <Servo.h>

Servo servoLeft1;
Servo servoLeft2;
Servo servoRight1;
Servo servoRight2;

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

const char *ssid = "***";  // 와이파이 이름
const char *pass = "***";      // 와이파이 비밀번호
const char *clientName = "abc34566";          // 사물 이름 (thing ID) 
const char *host = "***-ats.iot.us-west-2.amazonaws.com"; // AWS IoT Core 주소
const char* outTopic = "outTopic"; 
const char* inTopic = "inTopic"; 

String sChipID; // mac address를 문자로 기기를 구분하는 기호로 사용
char cChipID[40];
char msg[100];
unsigned int countTick=0;

unsigned long previousMillis = 0;     
const long interval = 1000;  

int duty[4]={0};

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  int duty0 = root["duty0"];
  int duty1 = root["duty1"];
  int duty2 = root["duty2"];
  int duty3 = root["duty3"];
  duty[0]=duty0;
  duty[1]=duty1;
  duty[2]=duty2;
  duty[3]=duty3;
  Serial.print(duty[0]);
  Serial.print(" : ");
  Serial.print(duty[1]);
  Serial.print(" : ");
  Serial.print(duty[2]);
  Serial.print(" : ");
  Serial.println(duty[3]);

  servoLeft1.write(duty[0]);
  servoLeft2.write(duty[1]);
  servoRight1.write(duty[2]);
  servoRight2.write(duty[3]);
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

void tick()
{
    /*
  countTick++;
  snprintf (msg, 75, "hello world #%ld", countTick);
  Serial.print("Publish message: ");
  Serial.println(msg);
  client.publish(outTopic, msg);
  */
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientName)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "hello world");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");

      char buf[256];
      wifiClient.getLastSSLError(buf,256);
      Serial.print("WiFiClientSecure SSL error: ");
      Serial.println(buf);

      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// Set time via NTP, as required for x.509 validation
void setClock() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));
}

void setup() {
  Serial.begin(115200);
  
  servoLeft1.attach(5);  // attaches the servo on GIO5 to the servo object 
  servoLeft2.attach(4);  // attaches the servo on GIO5 to the servo object 
  servoRight1.attach(0); 
  servoRight2.attach(2); 

  //이름 자동으로 생성
  uint8_t chipid[6]="";
  WiFi.macAddress(chipid);
  sprintf(cChipID,"%02x%02x%02x%02x%02x%02x%c",chipid[5], chipid[4], chipid[3], chipid[2], chipid[1], chipid[0],0);
  sChipID=String(cChipID);
  clientName=cChipID;

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  wifiClient.setTrustAnchors(&ca);
  wifiClient.setClientRSACert(&cert, &key);
  Serial.println("Certifications and key are set");

  setClock();
  //client.setServer(host, 8883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  doTick();
}

//1초 마다 실행되는 시간함수
void doTick() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    tick();
  }  
}
