
/*

*/

int ldrPin=A0;
int ldrVal = 0;
void setup() {
 
  Serial.begin(9600);
}

void loop() {
   
  ldrVal = analogRead(ldrPin);
  Serial.println(ldrVal);

  delay(100);
}
