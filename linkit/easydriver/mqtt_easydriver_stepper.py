import paho.mqtt.client as mqtt
import json, time
import mraa

pin19 = mraa.Pwm(19)
pin0 = mraa.Gpio(0)   
pin0.dir(mraa.DIR_OUT)

# ----- CHANGE THESE FOR YOUR SETUP -----
MQTT_HOST = "190.97.169.126" #Our free test MQTT server , please use with moderation
MQTT_PORT = 1883
#---------------------------------------

def on_connect(client, userdata, rc):
    print("\nConnected with result code " + str(rc) + "\n")
    client.subscribe("/redlibre/test/motor/control/")  
    print("Subscribed to homecontrol")

def on_message_iotrl(client, userdata, msg):
    print("\n\t* Linkit UPDATED ("+msg.topic+"): " + str(msg.payload))
    if msg.payload == "m2":
	pin0.write(0)
	pin1 = mraa.Gpio(1)
	pin1.dir(mraa.DIR_OUT)
	pin1.write(0)
	pin19.period_us(300)      
	pin19.enable(True)
	pin19.write(0.1)  
	time.sleep(2) 
        client.publish("/redlibre/test/motor/control/status/", "RIGHT", 2)
    if msg.payload == "m1":
    	pin0.write(0)
	pin1 = mraa.Gpio(1)
	pin1.dir(mraa.DIR_OUT)
	pin1.write(1)
	pin19.period_us(300)      
	pin19.enable(True)
	pin19.write(0.1)  
	time.sleep(2) 
        client.publish("/redlibre/test/motor/control/status/", "LEFT", 2)
    if msg.payload == "m0": 
	pin0.write(1)                                                      
	client.publish("/redlibre/test/motor/control/status", "STOP", 2)                                                
def command_error():
    print("Error: Unknown command")

client = mqtt.Client(client_id="linkit7688-stepper-motor")

client.on_connect = on_connect
client.message_callback_add("/redlibre/test/motor/control/", on_message_iotrl)

client.connect(MQTT_HOST, MQTT_PORT, 60)
client.loop_start() 

# Main program loop
while True:
    time.sleep(10) 
