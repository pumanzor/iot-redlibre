#include <Process.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MQTTclient.h"

unsigned long time;
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
  lcd.setCursor(0,0);
  lcd.print("RELE 1: OFF");
  lcd.setCursor(0,1);
  lcd.print("RELE 2: OFF");
  
    mqtt.publish("test/dos", "conectado!");
}

void loop() {



 int estadorele1 = bitRead(PORTD,4);
 int estadorele2 = bitRead(PORTD,6);

   if (millis() > (time + 10000)) {
       time = millis();


   
   mqtt.publish("/pryxo/yxusers/lrojas/control/ui98h23d09h2d/rele/1",estadorele1);
   mqtt.publish("/pryxo/yxusers/lrojas/control/ui98h23d09h2d/rele/2",estadorele2);
/*lcd.home();
  lcd.setCursor(0,2);
  lcd.print("Status RELE 1X:");
  lcd.setCursor(17,2);
  lcd.print(estadorele1);
  
  lcd.setCursor(0,3);
  lcd.print("Status RELE 2X:");
  lcd.setCursor(17,3);
  lcd.print(estadorele2);
  */ 

   }
 
  else {
  mqtt.monitor();
  }

}


void someEvent(const String& topic, const String& subtopic, const String& message) {





  mqtt.publish("test/dos", "Recibido_Control_D1");
   if (message == "rele10") {
    digitalWrite(rele1, HIGH);
    lcd.setCursor(0,0);
   // lcd.clear();
    lcd.print("RELE 1: OFF");
   
  } 
  else if (message == "rele11"){
    digitalWrite(rele1, LOW);
       lcd.setCursor(0,0);
        //lcd.clear();
    lcd.print("RELE 1:  ON");
    
  } 
  
 else if (message == "rele20"){
    digitalWrite(rele2, HIGH);
lcd.setCursor(0,1);
           // lcd.clear();
    lcd.print("RELE 2: OFF");
    
  }  else if (message == "rele21"){
    digitalWrite(rele2, LOW);
    lcd.setCursor(0,1);
         //   lcd.clear();
    lcd.print("RELE 2:  ON");
    
  } 

// lcd.setCursor(2,1);
}
