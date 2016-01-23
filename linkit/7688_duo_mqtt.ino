#include "MQTTclient.h"
#include <OneWire.h> 
#include <Bridge.h>
#include <DallasTemperature.h>
#include <SHT1x.h>
#define dataPin  2
#define clockPin 3
SHT1x sht1x(dataPin, clockPin);


#define ONE_WIRE_BUS_PIN A5
float  tempC2 = 0;
float  tempC3 = 0;
unsigned long time;

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);


#define MQTT_HOST "190.97.168.236" 


char message_buff[64];
char message_buff2[64];
char message[128];
String id_sensor = "linkitrs1";


float t2;



void setup() {
 
  Serial.begin(9600);
  Bridge.begin();
  mqtt.begin(MQTT_HOST, 1883);
  sensors.begin(); 
  //sensors.setResolution(probe01, 10);
  
  mqtt.subscribe("/rr2/temp", someEvent);


}


void loop() {
 delay(2000);
  float h = sht1x.readHumidity() * 100;
  float t = sht1x.readTemperatureC() * 100;

 sensors.requestTemperatures();
 
 tempC2 = sensors.getTempCByIndex(0),4;
 tempC3 = tempC2 * 100;
 
 //tempC2 = sensors.getTempC(probe01);
 
 if (isnan(t) || isnan(h)) {
    Serial.println("Failed to from DHT");
    mqtt.publish("/error/temp/SHT/linkitrs1","error de lectura NAN SHT" );
  }
  
  else if (t == 0.00 && h == 0.00)
  
  {
    Serial.println("Failed to from DHT");
    mqtt.publish("/error/temp/SHT/linkitrs1","error de lectura SHT" );
  }
  
    else if (tempC2 == -127.00)
  
  {
    Serial.println("Failed to from DHT");
    mqtt.publish("/error/temp/ds18b20/linkitrs1","error de lectura ds18b20 -127.00" );
  }
  
  
  else {
 
 
   if (millis() > (time + 15000)) {
       time = millis();
  
 String pubString = "" + String(t) + "," + String(tempC3) + "," + String(h) + "," + id_sensor + "";
   pubString.toCharArray(message_buff, pubString.length()+1);
   mqtt.publish("/rr2/temp",message_buff );

   }
   else{
   
  mqtt.monitor();
   }
  }

}

void someEvent(const String& topic, const String& subtopic, const String& message) {

}
