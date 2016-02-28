import paho.mqtt.client as mqtt
import json, time
import mraa

pin19 = mraa.Pwm(19)
pin0 = mraa.Gpio(0)   
pin0.dir(mraa.DIR_OUT)

# ----- CHANGE THESE FOR YOUR SETUP -----
MQTT_HOST = "190.97.168.236"
MQTT_PORT = 1883

def on_connect(client, userdata, rc):
    print("\nConnected with result code " + str(rc) + "\n")

    #Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    #client.subscribe("/pyxo/xyusers/{USERNAME}/{APIKEY}/iot/control/".format(**vars()), 2)  # Connect to everything in /mcu topic
    client.subscribe("/pryxo/yxusers/motor/control/")  
    print("Subscribed to homecontrol")


def on_message_iotrl(client, userdata, msg):
    print("\n\t* Linkit UPDATED ("+msg.topic+"): " + str(msg.payload))
    if msg.payload == "m1":
	pin0.write(0)
	pin1 = mraa.Gpio(1)
	pin1.dir(mraa.DIR_OUT)
	pin1.write(0)
	pin19.period_us(300)      
	pin19.enable(True)
	pin19.write(0.1)  
	time.sleep(2) 
        client.publish("/pryxo/yxusers/iot/status/", "derecha", 2)
    if msg.payload == "m0":
	pin1 = mraa.Gpio(1)
	pin1.dir(mraa.DIR_OUT)
	pin1.write(1)
	pin19.period_us(300)      
	pin19.enable(True)
	pin19.write(0.1)  
	time.sleep(2) 
        client.publish("/pryxo/yxusers/iot/status/", "izquierda", 2)
    if msg.payload == "m2": 
	pin0.write(1)                                                      
	client.publish("/pryxo/yxusers/iot/status/", "STOP", 2)                                                
def command_error():
    print("Error: Unknown command")

client = mqtt.Client(client_id="linkit7688-patio")

# Callback declarations (functions run based on certain messages)
client.on_connect = on_connect
client.message_callback_add("/pryxo/yxusers/motor/control/", on_message_iotrl)

# This is where the MQTT service connects and starts listening for messages
client.connect(MQTT_HOST, MQTT_PORT, 60)
client.loop_start()  # Background thread to call loop() automatically

# Main program loop
while True:
    time.sleep(10) 
