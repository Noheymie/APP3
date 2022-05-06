float Vout;

void setup() {

  Serial.begin(38400);

}

void loop() {
  
  Vout = analogRead(A2);
  Serial.print("analog A2 is: ");
  Serial.println(Vout*0.005);
  delay(250);
  
}
