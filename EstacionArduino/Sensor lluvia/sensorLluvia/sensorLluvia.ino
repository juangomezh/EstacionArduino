
int analogValue;
bool digitalValue;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Entrada analogica
  analogValue = analogRead(1);
  if (analogValue < 300)
    Serial.println("Lluvia intensa");
  else if (analogValue < 500)
    Serial.println("Lluvia moderada");
  else 
    Serial.println("Lluvia no detectada");

 //Entrada digintal pin 2 arduino
 digitalValue = digitalRead(3);
 if (digitalValue == HIGH)
    Serial.println (">>> NO LLUVIA");
 else 
    Serial.println(">>>>> LLUVIA");


 delay(1000);
}
