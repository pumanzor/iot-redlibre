import paho.mqtt.client as mqtt
import json, time
import os

# ----- CHANGE THESE FOR YOUR SETUP -----
MQTT_HOST = "190.97.168.236"
MQTT_PORT = 1883
# ---------------------------------------


# The callback function for when the client connects to broker
def on_connect(client, userdata, rc):
    print("\nConnected with result code " + str(rc) + "\n")

    #Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/mcu/#")  # Connect to everything in /mcu topic
    print("Subscibed to /mcu/#")


# The callback function for when a message on /mcu/rgbled_status/ is published
def on_message_rgbled(client, userdata, msg):
    print("\n\t* LED UPDATED ("+msg.topic+"): " + str(msg.payload))
import paho.mqtt.client as mqtt
import json, time
import os

# ----- CHANGE THESE FOR YOUR SETUP -----
MQTT_HOST = "190.97.168.236"
MQTT_PORT = 1883
# ---------------------------------------


# The callback function for when the client connects to broker
def on_connect(client, userdata, rc):
    print("\nConnected with result code " + str(rc) + "\n")

    #Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("/mcu/#")  # Connect to everything in /mcu topic
    print("Subscibed to /mcu/#")


# The callback function for when a message on /mcu/rgbled_status/ is published
def on_message_rgbled(client, userdata, msg):
    print("\n\t* LED UPDATED ("+msg.topic+"): " + str(msg.payload))
    if msg.payload == "1": 
	os.system("/root/on.sh")
        client.publish("mcu/status", "ENCENDIDO")
    else:
       os.system("/root/off.sh")
       client.publish("mcu/status", "APAGADO")
# Call this if input is invalid
def command_error():
    print("Error: Unknown command")


# Create an MQTT client instance
client = mqtt.Client(client_id="python-commander")

# Callback declarations (functions run based on certain messages)
client.on_connect = on_connect
client.message_callback_add("/mcu/rgbled_status/", on_message_rgbled)

# This is where the MQTT service connects and starts listening for messages
client.connect(MQTT_HOST, MQTT_PORT, 60)
client.loop_start()  # Background thread to call loop() automatically

# Main program loop
while True:
    time.sleep(10) 
