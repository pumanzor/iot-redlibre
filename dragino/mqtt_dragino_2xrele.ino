#include <Process.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MQTTclient.h"

#define I2C_ADDR  0x27
#define En_pin    2
#define Rw_pin    1
#define Rs_pin    0
#define D4_pin    4
#define D5_pin    5
#define D6_pin    6
#define D7_pin    7
#define BACKLIGHT_PIN   3

int rele1 = 4;
int rele2 = 6; 

#define MQTT_HOST "190.97.168.236" 
char message_buff[100];
char message_buff2[100];
char message[256];
String id_sensor = "prueba12";

LiquidCrystal_I2C lcd(I2C_ADDR, En_pin, Rw_pin, Rs_pin,
                      D4_pin, D5_pin, D6_pin, D7_pin);

void setup() {
    Serial.begin(9600);
  Bridge.begin();
  mqtt.begin(MQTT_HOST, 1883);
    mqtt.subscribe("/rr/temp", someEvent);
  lcd.begin (20, 4);
  pinMode(rele1, OUTPUT);
  pinMode(rele2, OUTPUT);
  digitalWrite(rele1, HIGH);
  digitalWrite(rele2, HIGH);

  // Switch on the backlight
  lcd.setBacklightPin(BACKLIGHT_PIN, POSITIVE);
  lcd.setBacklight(HIGH);

  // Position cursor and write some text
  lcd.home();
  lcd.print("RELE 1: OFF, RELE 2: OFF");
    mqtt.publish("test/dos", "conectado!");
}

void loop() {
  
  mqtt.monitor();

}


void someEvent(const String& topic, const String& subtopic, const String& message) {





  mqtt.publish("test/dos", "Recibido_Control_D1");
   if (message == "rele10") {
    digitalWrite(rele1, HIGH);
    lcd.clear();
    lcd.print("RELE 1: ON");
   
  } 
  else if (message == "rele11"){
    digitalWrite(rele1, LOW);
        lcd.clear();
    lcd.print("RELE 1: OFF");
    
  } 
  
 else if (message == "rele20"){
    digitalWrite(rele2, HIGH);
            lcd.clear();
    lcd.print("RELE 2: ON");
    
  }  else if (message == "rele21"){
    digitalWrite(rele2, LOW);
            lcd.clear();
    lcd.print("RELE 2: OFF");
    
  } 
