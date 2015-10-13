
#include <Process.h>
#include "MQTTclient.h"
#include "DHT.h"
#include <OneWire.h> 
#include <DallasTemperature.h>


#define ONE_WIRE_BUS_PIN A3
float  tempC2 = 0;
float  tempC3 = 0;
unsigned long time;

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
//DeviceAddress probe01 =  {0x28, 0xC7, 0xEB, 0xF0, 0x05, 0x00, 0x00, 0xEA};

#define MQTT_HOST "190.97.168.236" 
#define DHTPIN A0     
//#define DHTPIN2 A1

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);


char message_buff[100];
char message_buff2[100];
char message[256];
String id_sensor = "prueba5";


float t2;



void setup() {
 
  Serial.begin(9600);
  Bridge.begin();
  mqtt.begin(MQTT_HOST, 1883);
  dht.begin();
  sensors.begin(); 
  //sensors.setResolution(probe01, 10);
  
  mqtt.subscribe("rr/temp", someEvent);


}


void loop() {
 delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

 sensors.requestTemperatures();
 
 tempC2 = sensors.getTempCByIndex(0),4;
 //tempC2 = sensors.getTempC(probe01);
 
 if (isnan(t) || isnan(h)) {
    Serial.println("Failed to from DHT");
    mqtt.publish("test/temp","error de lectura NAN temp o humi" );
  }
  
  else if (t == 0.00 && h == 0.00)
  
  {
    Serial.println("Failed to from DHT");
    mqtt.publish("test/temp","error de lectura, en cero Temp y humi" );
  }
  
    else if (tempC2 == -127.00)
  
  {
    Serial.println("Failed to from DHT");
    mqtt.publish("test/temp","error de lectura ds18b20 -127.00" );
  }
  
  
  else {
 
 
   if (millis() > (time + 5000)) {
       time = millis();
  
 String pubString = "" + String(t) + "," + String(tempC2) + "," + String(h) + "," + id_sensor + "";
   pubString.toCharArray(message_buff, pubString.length()+1);
   mqtt.publish("test/temp",message_buff );

   }
   else{
   
  mqtt.monitor();
   }
  }

}

void someEvent(const String& topic, const String& subtopic, const String& message) {

}
