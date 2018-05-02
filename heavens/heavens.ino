#include <Milkcocoa.h>
#include <WiFi.h>

#define MILKCOCOA_APP_ID      "xxx"
#define MILKCOCOA_SERVERPORT  1883
#define MILKCOCOA_DATASTORE   "2ftoilet"

const char MQTT_SERVER[] PROGMEM    = MILKCOCOA_APP_ID ".mlkcca.com";
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ MILKCOCOA_APP_ID;
const char* ssid     = "ssid";
const char* password = "pass";
const int lead_swich_pin = 12;

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

}

void loop() {
  // put your main code here, to run repeatedly:
  milkcocoa.loop();
  DataElement elem = DataElement();
  int door1_sensor_value = digitalRead(lead_swich_pin);
  String msg = "lead_swich_pin :";
  msg.concat(door1_sensor_value);
  Serial.println(msg);
  if (door1_sensor_value == 1)
  {
    elem.setValue("state", "空席");
  } else {
    elem.setValue("state", "使用中");
  }
  // データストア名を指定して、データをpushåします
  milkcocoa.push(MILKCOCOA_DATASTORE, &elem);
  delay(1000);
}
