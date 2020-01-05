#include <TimeLib.h>
                                                                      // Virtuino KEY, only the requests that include this key will be acceptable
#include <SoftwareSerial.h>
#include <DHT11.h> //cargamos la librería DHT

const DHT11 dht11(8); 
byte temp, hum;   

SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin
const char* serverIP = "10.20.10.210";
const char* ssid = "ITAdisic";                                        // WIFI network SSID or AP SSID
const char* password = "4D1D1S1C200106";    
void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);       // You have to set the baud rate of the ESP8266 to 9600 before 
  connectToWiFiNetwork();      //enable this line to connect the module your wifi network 
  empty();
  setSyncProvider(requestSync);
}
void loop() { 
    if(Serial.available())
    {
      processSync();
    }
    if(timeStatus() != timeNotSet)
    {
      sensores();
      delay(1000);
      httpGet();
      delay(3000);
    }
}
String DTime()
{
  return String(year())+"-"+String(month())+"-"+String(day())+" "+String(hour())+":"+String(minute())+":"+String(second());
}
void processSync()
{
  if(Serial.find("T"))
  {
    setTime(Serial.parseInt());
  }
}

time_t requestSync()
{
  Serial.print("--");
  return 0;
}
void sensores()
{
  //Codigo DHT11 - S.Temperatura 
    float h, t; 
    dht11.read(h, t);
    hum=byte(h);
    temp=byte(t);
    //lecturaPorcentaje = map(analogRead(A0), 1023, 0, 0, 100);
}

//============================================================== connectToWiFiNetwork
void connectToWiFiNetwork(){
    Serial.println("Connecting to "+String(ssid));
    empty();

    
    espSerial.println(F("AT+CWMODE=1"));  // configure as client
    waitForResponse("OK",1000);
    //--- connect
    espSerial.print("AT+CWJAP=\"");
    espSerial.print(ssid);
    espSerial.print("\",\"");
    espSerial.print(password);
    espSerial.println("\"");
    waitForResponse("OK",10000);

    espSerial.print(F("AT+CIPSTA=\""));
    espSerial.print(serverIP);
    espSerial.println("\"");   
    waitForResponse("OK",5000);

    espSerial.println(F("AT+CIPSTA?"));
    waitForResponse("OK",3000); 
    
    espSerial.println(F("AT+CIFSR"));           // get ip address
    waitForResponse("OK",1000);
 
}

   
boolean waitForResponse(String target1,  int timeout){
    String data;
    char a;
    unsigned long start = millis();
    boolean rValue=false;
    while (millis() - start < timeout) {
        while(espSerial.available() > 0) {
            a = espSerial.read();
            Serial.print(a);
            if(a == '\0') continue;
            data += a;
        }
        if (data.indexOf(target1) != -1) {
            rValue=true;
            break;
        } 
    }
    return rValue;
}
void httpGet()
{
   sendMultiple();
}

void sendMultiple()
{
  String post, request;
  boolean sent=false;
      while(!sent)
      {
          Serial.println(F("POST TEMPERATURA"));
          TCP("TCP", "10.20.10.77", 80);
          post="{\r\n  \"TimeStamp\": \""+String(DTime())+"\",\r\n  \"MAC\": \"1\",\r\n  \"temperatura\":"+String(temp)+",\r\n  \"unidad\": \"Celsius\"\r\n}";
          request =  "POST /api/Temperatura/PostTemperatura HTTP/1.1\r\nHost: 10.20.10.77:80\r\nContent-Type: application/json\r\nContent-Length: "+String(post.length())+"\r\n\r\n";
          request+=post;
          empty();
          espSerial.print("AT+CIPSEND=");
          espSerial.println(request.length());
          empty();
          espSerial.print(request);
          sent=waitForResponse("SEND OK", 500);
          espSerial.println("AT+CIPCLOSE");
          delay(1000);
      }
      delay(3000);
      sent=false;
      while(!sent)
      {
        Serial.println(F("POST HUMEDAD"));
        TCP("TCP", "10.20.10.77", 80);
        post="{\r\n  \"TimeStamp\": \""+String(DTime())+"\",\r\n  \"MAC\": \"1\",\r\n  \"humedad\":"+String(hum)+",\r\n  \"unidad\": \"%\"\r\n}";
        request =  "POST /api/Humedad/PostHumedad HTTP/1.1\r\nHost: 10.20.10.77:80\r\nContent-Type: application/json\r\nContent-Length: "+String(post.length())+"\r\n\r\n";
        request+=post;
        empty();
        espSerial.print("AT+CIPSEND=");
        espSerial.println(request.length());
        empty();
        espSerial.print(request);
        sent=waitForResponse("SEND OK", 500);
        espSerial.println("AT+CIPCLOSE");
        delay(1000);  
      }
      delay(3000);
      sent=false;
      while(!sent)
      {
        Serial.println(F("POST HUMEDAD DEL SUELO"));
        TCP("TCP", "10.20.10.77", 80);
        post="{\r\n  \"TimeStamp\": \""+String(DTime())+"\",\r\n  \"MAC\": \"1\",\r\n  \"humedadSuelo\":"+String(map(analogRead(A0), 1023, 0, 0, 100))+",\r\n  \"unidad\": \"%\"\r\n}";
        request =  "POST /api/HumedadSuelo/PostHumedad HTTP/1.1\r\nHost: 10.20.10.77:80\r\nContent-Type: application/json\r\nContent-Length: "+String(post.length())+"\r\n\r\n";
        request+=post;
        empty();
        espSerial.print("AT+CIPSEND=");
        espSerial.println(request.length());
        empty();
        espSerial.print(request);
        sent=waitForResponse("SEND OK", 500);
        espSerial.println("AT+CIPCLOSE");
        delay(1000);
      }
      delay(3000);
      sent=false;
      while(!sent)
      {
        Serial.println(F("POST LLUVIA"));
        TCP("TCP", "10.20.10.77", 80);
        post="{\r\n  \"TimeStamp\": \""+String(DTime())+"\",\r\n  \"MAC\": \"1\",\r\n  \"estado\":\""+String(digitalRead(3))+"\"\r\n}";
        request =  "POST /api/lluvia/PostLluvia HTTP/1.1\r\nHost: 10.20.10.77:80\r\nContent-Type: application/json\r\nContent-Length: "+String(post.length())+"\r\n\r\n";
        request+=post;
        empty();
        espSerial.print(F("AT+CIPSEND="));
        espSerial.println(request.length());
        empty();
        espSerial.print(request);
        sent=waitForResponse("SEND OK", 500);
        espSerial.println("AT+CIPCLOSE");
        delay(1000);
      }
      delay(3000);
}
bool TCP(String type, String addr, uint32_t port)
{
  String data;
  empty();
  espSerial.print(F("AT+CIPSTART=\""));
  espSerial.print(type);
  espSerial.print(F("\",\""));
  espSerial.print(addr);
  espSerial.print(F("\","));
  espSerial.println(port);
  data = recvString("OK", "ERROR", "ALREADY CONNECT", 500);
  if (data.indexOf("OK") != -1 || data.indexOf("ALREADY CONNECT") != -1) {
    return true;
  }
  return false;
}

String recvString(String target1, String target2, String target3, uint32_t timeout)
{
  String data;
  char a;
  int start = millis();
  while (millis() - start < timeout) {
    while (espSerial.available() > 0) {
      a = espSerial.read();
      if (a == '\0') continue;
      data += a;
    }
    if (data.indexOf(target1) != -1) {
      break;
    } else if (data.indexOf(target2) != -1) {
      break;
    } else if (data.indexOf(target3) != -1) {
      break;
    }
  }
  return data;
}

void empty()
{
   while(espSerial.available())
  {
      espSerial.read();
  }
}
