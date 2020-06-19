#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Servo.h> 
#include <time.h>

// 아래의 6개설정은 사용자 환경에 맞게 수정하세요.
const char *ssid = "i2r"; // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char *pass = "00000000";  // 와이파이 AP, 또는 스마트폰의 핫스판 이름
const char* outTopic = "/kdi/inTopic"; // 이름이 중복되지 않게 설정 기록
const char* inTopic = "/kdi/outTopic"; // 이름이 중복되지 않게 설정 기록
const char *thingId = "";          // 사물 이름 (thing ID) 
const char *host = "***.iot.us-west-2.amazonaws.com"; // AWS IoT Core 주소

String sChipID;
char cChipID[20];
long lastMsg = 0;
char msg[50];
int value = 0;

Servo servoLeft1;
Servo servoLeft2;
Servo servoRight1;
Servo servoRight2;

//json을 위한 설정
StaticJsonDocument<200> doc;
DeserializationError error;
JsonObject root;

int Duty,mode=0; //imode 0=초기치 1,2,3,4=서보모터 회전 5=동작속도 9=DoArray
int data[100],volume;  //volume : data[]의 개수
int iTime=500;  // 로봇 동작속도

// 사물 인증서 (파일 이름: xxxxxxxxxx-certificate.pem.crt)
const char cert_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
2C72J9fylSWjz8TwJb4PVgv77gs9QraTCZWt+Lo3IftgJJHOM0R9Cv8+/BMo/yVu
uhJYX28+7PORM0FHMFESTGYiccyii1Z6Cj2otg59a770RU+GlDJ0t80xM3THavPa
Wa69k6H+F6gGOz9EUMKsGHvR97ZfX3gUHDkV0MsrOx78u7KO6ASTyLHq2caDoA==
-----END CERTIFICATE-----
)EOF";
// 사물 인증서 프라이빗 키 (파일 이름: xxxxxxxxxx-private.pem.key)
const char key_str[] PROGMEM = R"EOF(
-----BEGIN RSA PRIVATE KEY-----
34s5fcCMKeVH3+xzDbloQHDMQKPr8OMQkEwTE4ZEIQUQTS6M+C6XU80CgYEA2tTs
9C2OkV4x59/x5r/YPPKZzc2dXRLrvWef+LuU0t+r7m/KgtDqcDVFLHFsa+eHp1YJ
gt3TQp/s+rUQLmneEW1Qg3kNGic/D2+aCmcTZ6KFsY1b18nfmBcoCXnK4IpRRZC5
vPiO/4+yAwRP3p18csRiAhxMA5aV/jRI4n3x2XcCgYBEwYjYPliC4RPdtCvRA6PF
tSKRMRIN0XEGj1q06kTRJzBtz0ef/hLxPAf7JYi36a0e4PY9CUPOkaaRSpICGmmj
84oyQS7FHuc4/xvy019JCzPB0DNbPmh3AVmyM4bUsn59zd3m9dPWSpkfxV8dcXeG
nR9xBsDoTprsZC9M1/YaEQKBgQCFDgIlVa3h5YwimJ/U1cD9DeVGpUaRWPfNajxy
WFvc+/LmP0K615w0La+pni12TojEziNqVsYlNGg65+y2y9gicH0L84Zr6IEHOaxH
tvSthrwTgQfe5Pf22d2WJV+dt3xC+AR3SgpoiU6LVwvZJ5iLj6yn69ysMdtxfQjz
7fAncQKBgQC2I45sOS6xVx1+KOhQ+3QvST4O2hrw3sedA3vlorS7Wg0vajSZFELo
qjKDU8njRc8Lgdx/K71XIlo+CoBoLflxUHvmYpCN1gycyWJMvgKbnApVwttJm30z
zJQkfoMU4uPy/L2JJRyIpLKKxlVAqhYpw0koEMwTI7+cJsFe5BmNEw==
-----END RSA PRIVATE KEY-----

)EOF";
// Amazon Trust Services(ATS) 엔드포인트 CA 인증서 (서버인증 > "RSA 2048비트 키: Amazon Root CA 1" 다운로드)
const char ca_str[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// 통신에서 문자가 들어오면 이 함수의 payload 배열에 저장된다.
void callback(char* topic, byte* payload, unsigned int length) {
  int i;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
  int premode=mode;
  deserializeJson(doc,payload);
  root = doc.as<JsonObject>();
  mode = root["mode"];
  Duty = root["duty"];
  if(mode==0)
    InitSet();
  else if(mode==1)
    servoLeft1.write(Duty);
  else if(mode==2)
    servoLeft2.write(Duty);
  else if(mode==3)
    servoRight1.write(Duty);
  else if(mode==4)
    servoRight2.write(Duty);
  else if(mode==5) {
    iTime = root["valocity"];
    mode=premode;
  }
  else if(mode==9) { 
      volume = root["volume"]; 
      Serial.println(volume);
      for(i=0; i<volume;i++)
        data[i]= root["data"][i];     
    Serial.println("=======");
    for(i=0;i<volume;i++)
      Serial.println(data[i]);
  }
  else
    Serial.print("No choice");
  Serial.print(mode);
}

X509List ca(ca_str);
X509List cert(cert_str);
PrivateKey key(key_str);
WiFiClientSecure wifiClient;
PubSubClient client(host, 8883, callback, wifiClient); //set  MQTT port number to 8883 as per //standard

// mqtt 통신에 지속적으로 접속한다.
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(thingId)) {
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

void setup() {
  Serial.begin(115200);
  delay(1000);

  //이름 자동으로 생성
  uint8_t chipid[6]="";
  WiFi.macAddress(chipid);
  sprintf(cChipID,"%02x%02x%02x%02x%02x%02x%c",chipid[5], chipid[4], chipid[3], chipid[2], chipid[1], chipid[0],0);
  sChipID=String(cChipID);
  thingId=&cChipID[0];
  Serial.println(sChipID);

  servoLeft1.attach(5);  // attaches the servo on GIO5 to the servo object 
  servoLeft2.attach(4);  
  servoRight1.attach(0); 
  servoRight2.attach(2); 

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


void DoArray() {
  int i,j,k;
  k=volume/4;
  for(i=0 ; i<volume ; i++) {
    j=i%4;
    Serial.println(j);
    Serial.println(data[i]);
    if(j==0)
       servoLeft1.write(data[i]);
    else if(j==1)
       servoLeft2.write(data[i]);
    else if(j==2)
       servoRight1.write(data[i]);
    else if(j==3)
       servoRight2.write(data[i]);

    delay(iTime);
  }
}

void InitSet() {
  servoLeft1.write(90);
  delay(iTime);
  servoLeft2.write(90);
  delay(iTime);
  servoRight1.write(90);
  delay(iTime);  
  servoRight2.write(90);
  delay(iTime);   
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  /*
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 75, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(outTopic, msg);
    Serial.print("Heap: "); Serial.println(ESP.getFreeHeap()); //Low heap can cause problems
  }
  */
  if(mode==9)
    DoArray();
}
