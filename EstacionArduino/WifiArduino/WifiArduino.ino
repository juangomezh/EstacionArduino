


//--------------- USER SETTINGS ---------------------------------------
const char* ssid = "ITAdisic";          // WIFI network SSID or AP SSID
const char* password = "4D1D1S1C200106";          // WIFI network PASSWORD

#define VIRTUINO_KEY  "1234"                // Virtuino KEY, only the requests that include this key will be acceptable
#define MAX_BUFFER_SIZE  300                                       // disable the line if you don't want to use a key                          

#include <SoftwareSerial.h>

SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin

uint8_t m_responseBuffer[MAX_BUFFER_SIZE] = {0};
int port = 8000;
long puerto= 53505;
const char* serverIP = "10.20.10.210";  
const char* api = "10.20.10.77"; // The three first numbers have to be the same with the router IP
const char* protocolo = "TCP";
unsigned long timeStrored=0;
boolean debug=true;

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200);       // You have to set the baud rate of the ESP8266 to 9600 before 
  connectToWiFiNetwork();      //enable this line to connect the module your wifi network
  //initAccessPoint();         //enable this line to use the esp as AP  
  clearESP_buffer(1000);
}
void loop() { 
    enviar();
    delay(4000);
}


//============================================================== connectToWiFiNetwork
void connectToWiFiNetwork(){
    Serial.println("Connecting to "+String(ssid));
    clearESP_buffer(1000);
    espSerial.println("AT+GMR");
    waitForResponse("OK",1000);
    
    espSerial.println("AT+CWMODE=1");  // configure as client
    waitForResponse("OK",1000);
    
    //--- connect
    espSerial.print("AT+CWJAP=\"");
    espSerial.print(ssid);
    espSerial.print("\",\"");
    espSerial.print(password);
    espSerial.println("\"");
    waitForResponse("OK",10000);

    espSerial.print("AT+CIPSTA=\"");
    espSerial.print(serverIP);
    espSerial.println("\"");   
    waitForResponse("OK",5000);

    espSerial.println("AT+CIPSTA?");
    waitForResponse("OK",3000); 
    
    espSerial.println("AT+CIFSR");           // get ip address
    waitForResponse("OK",1000);


 
}

void enviar()
{
    httpGet();
    delay(2000);
}

void clearESP_buffer(int timeout){
    long t=millis();
    char c;
    while (t+timeout>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        Serial.print("");
        if (debug) Serial.print(c);
      }
    }
   }
   
boolean waitForResponse(String target1,  int timeout){
    String data;
    char a;
    unsigned long start = millis();
    boolean rValue=false;
    while (millis() - start < timeout) {
        while(espSerial.available() > 0) {
            a = espSerial.read();
     
            Serial.print("");
            if (debug) Serial.print(a);
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
  String post="{\r\n  \"TimeStamp\": \"2020-4-1 23:18:33\",\r\n  \"MAC\": \"1\",\r\n  \"temperatura\": 26,\r\n  \"unidad\": \"sample string 4\"\r\n}";
  int len=post.length();
  
  String request =  "POST /api/Temperatura/PostTemperatura HTTP/1.1\r\nHost: 10.20.10.77:80\r\nContent-Type: application/json\r\nContent-Length: "+String(len)+"\r\n\r\n";
  request+=post;
  if (TCP("TCP", "10.20.10.77", 80))
  {
    Serial.println(F("create tcp - OK"));
  }
  else
  {
    Serial.println(F("create tcp - ERROR"));
    return "";
  }
  if (!sendSingle(request))
  {
    Serial.print(F("not sent"));
    
    //return "";
  }
  else
  {
    int len = (m_responseBuffer, MAX_BUFFER_SIZE);
    Serial.println((char*)m_responseBuffer);
    delay(2000);
    
  }
  espSerial.println("AT+CIPCLOSE");
}
bool sendSingle(String url)
{
  empty();
  espSerial.print("AT+CIPSEND=");
  espSerial.println(url.length());
  empty();
  espSerial.print(url);
  return waitForResponse("SEND OK", 500);
}
bool TCP(String type, String addr, uint32_t port)
{
  String data;
  empty();
  espSerial.print("AT+CIPSTART=\"");
  espSerial.print(type);
  espSerial.print("\",\"");
  espSerial.print(addr);
  espSerial.print("\",");
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
  unsigned long start = millis();
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

 
