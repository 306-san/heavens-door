const int lead_swich_pin = 12;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200) ;
  pinMode(lead_swich_pin, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(lead_swich_pin));
  delay(500);
}
