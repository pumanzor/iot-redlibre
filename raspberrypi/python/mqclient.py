import paho.mqtt.client as paho
import RPi.GPIO as GPIO
import json, time
import picamera

# device credentials
device_id        = 'test1'      # * set your device id (will be the MQTT client username)
device_secret    = '1234'  # * set your device secret (will be the MQTT client password)
random_client_id = 'test1'      # * set a random client_id (max 23 char)


# -------------- #
# Board settings #
# -------------- #
buttonPin = 7
ledPin = 12

GPIO.setmode(GPIO.BOARD)  # use P1 header pin numbering convention
GPIO.cleanup()            # clean up resources
GPIO.setup(ledPin, GPIO.OUT)   # led pin setup
GPIO.setup(buttonPin, GPIO.IN)   # button pin setup


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


# ------------ #
# Button logic #
# ------------ #

prev_status = GPIO.LOW
led_status  = GPIO.LOW
updated_at  = 0  # the last time the output pin was toggled
debounce    = 0.2  # the debounce time, increase if the output flickers

# Continue the network loop, exit when an error occurs
rc = 0
while rc == 0:
    rc = client.loop()
    button = GPIO.input(buttonPin)

    if button != prev_status and time.time() - updated_at > debounce:
        prev_status = button
        updated_at  = time.time()

        if button:
            led_status = not led_status
            button_payload = 'off'
            if led_status == GPIO.HIGH:
                button_payload = 'on'
            # effectively update the light status
            GPIO.output(ledPin, led_status)
            payload = { 'properties': [{ 'id': '518be5a700045e1521000001', 'value': button_payload }] }
            client.publish(out_topic, json.dumps(payload))
	########camera
        with picamera.PiCamera() as camera:
                        camera.resolution = (1024, 768)
                        camera.start_preview()
                        # Camera warm-up time
                        time.sleep(2)
                        camera.capture('foo2.jpg')
        ###################



print('rc: ' + str(rc))
