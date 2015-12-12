const int gasPin = A0; //GAS sensor output pin to Arduino analog A0 pin
unsigned long time;

void setup()
{
	Serial.begin(9600); //Initialize serial port - 9600 bps
 pinMode(7, OUTPUT);
}

void loop()
{
  int valorgas = analogRead(gasPin);
	
	delay(1000); // Print value every 1 sec.

if (valorgas > 200 )

{digitalWrite(7, HIGH);}

else
{digitalWrite(7, LOW);}


if (millis() > (time + 5000)) {
       time = millis();
Serial.println(valorgas);
}
else {}

}
