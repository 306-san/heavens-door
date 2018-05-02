#include <Milkcocoa.h>
#include <WiFi.h>
#include <HTTPClient.h>

//Milkcocoa Settings
#define MILKCOCOA_APP_ID      "xxx"
#define MILKCOCOA_SERVERPORT  1883
#define MILKCOCOA_DATASTORE   "2ftoilet"

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;
//Wi-Fi Settings
const char* ssid     = "ssid";
const char* password = "pass";
//リードスイッチのピン設定
const int lead_swich_pin = 12;
// Slack incoming web hook URL
const char* slackUrl         = "https://hooks.slack.com/services/hoge/foo/bar";
const char* slackUserAgent   = "IoT_DoorSensor/1.0";
//flagの設定
bool is_posted = false; //投稿したか
bool is_door_open = false; //センサー読み取る前の状態を保持

WiFiClient client;

// 'client' is Ethernet/WiFi Client you defined
Milkcocoa milkcocoa = Milkcocoa(&client, MQTT_SERVER, MILKCOCOA_SERVERPORT, MILKCOCOA_APP_ID, MQTT_CLIENTID);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200) ;
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(lead_swich_pin, INPUT_PULLUP);
  slack("hello, world");
}

void loop() {
  milkcocoa.loop();
  DataElement elem = DataElement();
  int door1_sensor_value = digitalRead(lead_swich_pin);
  String msg = "lead_swich_pin :";
  msg.concat(door1_sensor_value);
  Serial.println(msg);
  if (is_door_open != door1_sensor_value) {
    is_posted = false;
  }
  is_door_open = door1_sensor_value;
  if (door1_sensor_value == 1)
  {
    elem.setValue("state", "空席");
    if ( is_posted == false ) {
      slack("空席になりました。");
      is_posted = true;
    }
  } else {
    elem.setValue("state", "使用中");
    if ( is_posted == false ) {
      slack("トイレは使用中です。");
      is_posted = true;
    }
  }
  // データストア名を指定して、データをpushします
  milkcocoa.push(MILKCOCOA_DATASTORE, &elem);
  delay(1000);
}

void slack(String msg) {
  HTTPClient http;
  http.begin(slackUrl);
  http.setUserAgent(slackUserAgent);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded;");
  //メッセージを動的に送るため、bodyをばらして文字列連結で無理やり連結してJSONにしています。
  String Payload_body = "payload={\"text\":\"";
  Payload_body.concat(msg);
  Payload_body.concat("\"}");
  int httpCode = http.POST(Payload_body);
  if (httpCode > 0) {
    Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  } else {
    Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  //http.end();
  Serial.println ("Slack done");
}
