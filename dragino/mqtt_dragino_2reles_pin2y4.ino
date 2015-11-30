#include <Process.h>
#include "MQTTclient.h"
#include "DHT.h"
#include <OneWire.h> 
#include <DallasTemperature.h>

#define ONE_WIRE_BUS_PIN 3
float  tempC2 = 0;
float  tempC3 = 0;

OneWire oneWire(ONE_WIRE_BUS_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress probe01 =  {0x28, 0xC7, 0xEB, 0xF0, 0x05, 0x00, 0x00, 0xEA};
DeviceAddress probe02 =  {0x28, 0x82, 0x2F, 0xF1, 0x05, 0x00, 0x00, 0xC6};

#define MQTT_HOST "190.97.168.236" 
#define DHTPIN A0     

#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
//DHT dht2(DHTPIN2, DHTTYPE);


char message_buff[100];
char message_buff2[100];
String id_sensor = "sensor3";
String id_sensor2 = "sensor4";
int rele = 2;
//int led2 = 3 ;
int led = 4; 
unsigned long time;
float t2;

void setup() {
    
  Serial.begin(9600);
  Bridge.begin();
  mqtt.begin(MQTT_HOST, 1883);
  dht.begin();
  //dht2.begin();
  sensors.begin();
  sensors.setResolution(probe01, 10);
    sensors.setResolution(probe02, 10);
  pinMode(led, OUTPUT);
 // pinMode(led2, OUTPUT);
  pinMode(rele, OUTPUT);
  
  mqtt.subscribe("test/uno", someEvent);


}

/*void printTemperature(DeviceAddress deviceAddress)
{

float tempC = sensors.getTempC(deviceAddress);

   if (tempC == -127.00) 
   {
   Serial.print("Error getting temperature  ");
   } 
   else
   {
   Serial.print("C: ");
   Serial.print(tempC);
   Serial.print(" F: ");
   Serial.print(DallasTemperature::toFahrenheit(tempC));
   }
}

*/

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

 // float h2 = dht2.readHumidity();
  //float t2 = dht2.readTemperature();

 sensors.requestTemperatures();
 tempC2 = sensors.getTempC(probe01);
  tempC3 = sensors.getTempC(probe02);
 
   if (millis() > (time + 15000)) {
       time = millis();
  
  String pubString = "" + String(t) + " ," + String(h) + " ," + id_sensor + "" ;
   pubString.toCharArray(message_buff, pubString.length()+1);
        
   String pubString2= "" + String(tempC2) + " ," + String(tempC3) + " ," + id_sensor2 + "" ;
    pubString2.toCharArray(message_buff2, pubString2.length()+1);    
        
       mqtt.publish("test/temp",message_buff );
       mqtt.publish("test/temp",message_buff2 );

   }
   else{
   
  mqtt.monitor();
   }
}

void someEvent(const String& topic, const String& subtopic, const String& message) {
  
  
  Serial.print("topic: ");
  Serial.println(topic);
  Serial.print("message: "); 
  Serial.println(message); 

  mqtt.publish("test/dos", "Recibido_Control_D1");
   if (message == "led41") {
    digitalWrite(led, HIGH);
   
  } 
  else if (message == "led40"){
    digitalWrite(led, LOW);
    
  } 
  
 else if (message == "rele21"){
    digitalWrite(rele, HIGH);
    
  }  else if (message == "rele20"){
    digitalWrite(rele, LOW);
    
  } 
  
}
