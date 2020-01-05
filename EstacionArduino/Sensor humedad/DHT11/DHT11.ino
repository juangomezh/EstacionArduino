#include <DHT11.h> //cargamos la librería DHT

int pinHumedad=2; //PIN SENSOR HUMEDAD
DHT11 dht11(pinHumedad); 


int analogLluvia;
bool digitalLluvia, conexion;
 
void setup() {
  Serial.begin(9600);

 
}

void loop() {
//----------------------------------------------------------------
//Codigo DHT11 - S.Temperatura
float temp, hum;   
int err;
if((err=dht11.read(hum, temp))==0)
  {
    Serial.print("Sensor de temperatura >>   ");
    Serial.print("Temperatura: ");
    Serial.print(temp);
    Serial.print(" Humedad: ");
    Serial.print(hum);
    Serial.println();
  }
       else
          {
             Serial.println();
             Serial.print("Error Num :");
             Serial.print(err);
             Serial.println();
          }

//----------------------------------------------------------------

//----------------------------------------------------------------
//Sensor Humedad del Suelo
int lectura = analogRead(A0);

  Serial.print("Sensor del suelo      >>   ");

// lecturas entre 1000 - 1023
  if (lectura >= 1000){
    Serial.print("Sensor desconectado o fuera del suelo");
  }
   else if (lectura <1000 && lectura >= 303){
    Serial.print("Suelo seco");
   }
   else if (lectura < 303 && lectura >= 277){
    Serial.print("Suelo humedo");
   }
   else if (lectura < 235){
    Serial.print("Suelo mojado");
   }

   //Convirtiendo a Porcentaje
   int lecturaPorcentaje = map(lectura, 1023, 0, 0, 100);

   Serial.print(" - Humedad: ");
   Serial.print(lecturaPorcentaje);
   Serial.print("%");
   Serial.println();


//----------------------------------------------------------------

//----------------------------------------------------------------
//Sensor de LLuvia
  analogLluvia= analogRead(1);
  Serial.print("Sensor de Lluvia      >>   ");
  if (analogLluvia < 300)
    Serial.print("Lluvia intensa  ");
  else if (analogLluvia < 500)
    Serial.print("Lluvia moderada  ");
  else 
    Serial.print("Lluvia no detectada  ");

 //Entrada digintal pin 2 arduino
 digitalLluvia = digitalRead(3);
 if (digitalLluvia == HIGH)
    Serial.println (" - NO LLUVIA");
 else 
    Serial.println(" - LLUVIA");

//----------------------------------------------------------------
   
   
//----------------------------------------------------------------
//Retardo
  delay(5000); 
//----------------------------------------------------------------
}
