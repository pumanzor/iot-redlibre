#include <Process.h>
#include "MQTTclient.h"
#include "DHT.h"
#include <OneWire.h> 
#include <DallasTemperature.h>


#define ONE_WIRE_BUS_PINC3 A3
#define ONE_WIRE_BUS_PINC2 A2
float  tempC3 = 0;
float  tempC2 = 0;
unsigned long time;

OneWire oneWireC3(ONE_WIRE_BUS_PINC3);
OneWire oneWireC2(ONE_WIRE_BUS_PINC2);

DallasTemperature sensorsC3(&oneWireC3);
DallasTemperature sensorsC2(&oneWireC2);


#define MQTT_HOST "190.97.168.236" 
#define DHTPIN A0     
//#define DHTPIN2 A1

#define DHTTYPE DHT11   // DHT 11  (
DHT dht(DHTPIN, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);


char message_buff[100];
char message_buff2[100];
char message[256];
String id_sensor = "scgnas1";


float t2;



void setup() {
 
  Serial.begin(9600);
  Bridge.begin();
  mqtt.begin(MQTT_HOST, 1883);
  dht.begin();
  sensorsC3.begin(); 
  sensorsC2.begin();
  //sensors.setResolution(probe01, 10);
  
  mqtt.subscribe("scapq/control", someEvent);


}


void loop() {
 delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

 sensorsC3.requestTemperatures();
  sensorsC2.requestTemperatures();
 tempC3 = sensorsC3.getTempCByIndex(0),4;
  tempC2 = sensorsC2.getTempCByIndex(0),4;
 //tempC2 = sensors.getTempC(probe01);
 


// if (isnan(t) || isnan(h)) {
//    Serial.println("Failed to from DHT");
//    mqtt.publish("test/temp","error de lectura NAN temp o humi" );
 // }
  
 // else if (t == 0.00 && h == 0.00)
  
 // {
   // Serial.println("Failed to from DHT");
   // mqtt.publish("test/temp","error de lectura, en cero Temp y humi" );
 // }
  
   // else 
  // if (tempC3 == -127.00)
  
  //{
   // Serial.println("Failed to from DHT");
    //mqtt.publish("temp/scgnb/sensores/error","error de lectura ds18b20 -127.00" );
 // }
  
  
  //else {
 
 
   if (millis() > (time + 15000)) {
       time = millis();
  
 String pubString = "" + String(tempC3) + "," + String(tempC2) + "," + String(t) + "," + String(h) + "," + id_sensor + "";
   pubString.toCharArray(message_buff, pubString.length()+1);
   mqtt.publish("temp/scgnb/pldt",message_buff );

   }
   else{
   
  mqtt.monitor();
   }
 // }

}

void someEvent(const String& topic, const String& subtopic, const String& message) {

}
