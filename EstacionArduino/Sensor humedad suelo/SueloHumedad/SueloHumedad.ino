void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int lectura = analogRead(A0);

  Serial.print("La lectura es: ");
  Serial.println(lectura);

// lecturas entre 1000 - 1023
  if (lectura >= 1000){
    Serial.println(">> EL SENSOR ESTA DECONECTADO O FUERA DEL SUELO <<");
  }
   else if (lectura <1000 && lectura >= 600){
    Serial.println(">> EL SUELO ESTA SECO <<");
   }
   else if (lectura < 600 && lectura >= 370){
    Serial.println(">> EL SUELO ESTA HUMEDO <<");
   }
   else if (lectura < 370){
    Serial.println(">> EL SENSOR ESTA PRACTICAMENTE EN AGUA <<");
   }
   delay(1000);

   //Convirtiendo a Porcentaje
   int lecturaPorcentaje = map(lectura, 1023, 0, 0, 100);

   Serial.print("La Humedad es del: ");
   Serial.print(lecturaPorcentaje);
   Serial.println("%");
}
