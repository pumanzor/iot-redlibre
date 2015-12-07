import paho.mqtt.client as paho
import RPi.GPIO as GPIO
import json, time
import Adafruit_DHT

# device credentials
device_id        = 'tst1'      # * set your device id (will be the MQTT client username)
device_secret    = '134'  # * set your device secret (will be the MQTT client password)
random_client_id = 'tst1'      # * set a random client_id (max 23 char)


####dht11
period = 10 ## Sensor data reporting period (1 minute)
pin = 17 ## Assuming the DHT11 sensor is connected to GPIO pin number 4
#########


# --------------- #
# Callback events #
# --------------- #

# connection event
def on_connect(client, data, flags, rc):
    print('Connected, rc: ' + str(rc))

# subscription event
def on_subscribe(client, userdata, mid, gqos):
    print('Subscribed: ' + str(mid))

# received message event
def on_message(client, obj, msg):
    # get the JSON message
    json_data = msg.payload
    # check the status property value
    print(json_data)
    value = json.loads(json_data)['properties'][0]['value']
    if value == 'on':
        led_status = GPIO.HIGH
        GPIO.output(ledPin, GPIO.HIGH)
    else:
        led_status = GPIO.LOW
        GPIO.output(ledPin, GPIO.LOW)

    # confirm changes to Leylan
    client.publish(out_topic, json_data)


# ------------- #
# MQTT settings #
# ------------- #

# create the MQTT client
client = paho.Client(client_id=random_client_id, protocol=paho.MQTTv31)  # * set a random string (max 23 chars)

# assign event callbacks
client.on_message = on_message
client.on_connect = on_connect
client.on_subscribe = on_subscribe


# device topics
in_topic  = 'devices/' + device_id + '/get'  # receiving messages
out_topic = 'devices/' + device_id + '/set'  # publishing messages

# client connection
client.username_pw_set(device_id, device_secret)  # MQTT server credentials
client.connect("190.97.168.236")                   # MQTT server address
client.subscribe(in_topic, 0)                     # MQTT subscribtion (with QoS level 0)


##############LOOP sensor temp humi dht11
def run():
 while True:
    humidity, temperature = Adafruit_DHT.read_retry( Adafruit_DHT.DHT11, pin )
    if humidity is not None and temperature is not None:
        print "Temp={0:f}*C  Humidity={1:f}%".format(temperature, humidity)
        try:
	  #payload2 = { 'properties': [{ 'id': temperature, 'value': humidity }] }
          payload2 = temperature, humidity 
	  client.publish(out_topic, json.dumps(payload2))
        except Exception:
          ## Process exception here
          print "Error while writing to Beebotte"
    else:
        print "Failed to get reading. Try again!"

    #Sleep some time
    time.sleep( period )

run()
#################fin
