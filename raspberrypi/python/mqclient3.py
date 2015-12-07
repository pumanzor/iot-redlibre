import paho.mqtt.client as paho
import RPi.GPIO as GPIO
import json, time

# device credentials
device_id        = 't1'      # * set your device id (will be the MQTT client username)
device_secret    = '34'  # * set your device secret (will be the MQTT client password)
random_client_id = 't1'      # * set a random client_id (max 23 char)

####PIR
sensor = 27

GPIO.setmode(GPIO.BCM)
GPIO.setup(sensor, GPIO.IN, GPIO.PUD_DOWN)

previous_state = False
current_state = False
#######

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


##############PIR
#def run():
while True:
    time.sleep(0.1) 
    previous_state = current_state
    current_state = GPIO.input(sensor)
    if current_state != previous_state:
          new_state = "HIGH" if current_state else "LOW"
          print("GPIO pin %s is %s" % (sensor, new_state))
          client.publish(out_topic, json.dumps(new_state))          
#run()
#################fin
