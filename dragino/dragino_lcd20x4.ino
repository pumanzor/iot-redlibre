#include <Process.h>
#include "MQTTclient.h"
#include "DHT.h"
#include <OneWire.h> //Se importan las librerías
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

#define ONE_WIRE_BUS_PIN A1
float  tempC2 = 0;
float  tempC3 = 0;

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress probe01 =  {0x28, 0xC7, 0xEB, 0xF0, 0x05, 0x00, 0x00, 0xEA};

#define MQTT_HOST "190.97.168.236" 
#define DHTPIN A0     
//#define DHTPIN2 A1

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);


char message_buff[100];
char message_buff2[100];
char message[256];
String id_sensor = "sensor7";
unsigned long time;
float t2;
LiquidCrystal lcd(10, 9, 6, 4, 3, 2);

void setup() {
    lcd.begin(20, 4);
  Serial.begin(9600);
  Bridge.begin();
  mqtt.begin(MQTT_HOST, 1883);
  dht.begin();
  sensors.begin(); //Se inician los sensores
  sensors.setResolution(probe01, 10);
  
  mqtt.subscribe("test/temp", someEvent);


}


void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

 sensors.requestTemperatures();
 tempC2 = sensors.getTempC(probe01);
 
   if (millis() > (time + 30000)) {
       time = millis();
  
  String pubString = "" + String(t) + "," + String(tempC2) + "," + String(h) + "," + id_sensor + "" ;
   pubString.toCharArray(message_buff, pubString.length()+1);
   mqtt.publish("test/temp",message_buff );

   }
   else{
   
  mqtt.monitor();
   }
}


void someEvent(const String& topic, const String& subtopic, const String& message) {

int commaIndex = message.indexOf(',');
int secondCommaIndex = message.indexOf(',',commaIndex+1);
int thirdCommaIndex = message.indexOf(',',secondCommaIndex+1);

String temps1 = message.substring(0,commaIndex);
String tempa1 = message.substring(commaIndex+1,secondCommaIndex);
String humi1 = message.substring(secondCommaIndex+1,thirdCommaIndex);
String idsec = message.substring(thirdCommaIndex);


if (idsec == ",sensor8")
{
  lcd.setCursor(0,0);
String valores =  "S1:" + temps1 + "C " + tempa1  + "C " + humi1 + "%";
lcd.print(valores);
}

if (idsec == ",sensor7")
{
  lcd.setCursor(0,1);
String valores =  "S2:" + temps1 + "C " + tempa1  + "C " + humi1 + "%";
lcd.print(valores);
}


if (idsec == ",sensor6")
{
  lcd.setCursor(0,2);
String valores =  "S3:" + temps1 + "C " + tempa1  + "C " + humi1 + "%";
lcd.print(valores);
}

if (idsec == ",sensor5")
{
  lcd.setCursor(0,3);
String valores =  "S4:" + temps1 + "C " + tempa1  + "C " + humi1 + "%";
lcd.print(valores);
}
 
}
