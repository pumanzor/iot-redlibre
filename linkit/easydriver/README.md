![alt tag]
==================================================================================
==================================================================================

Hallo, this folder contain a programs to take control of stepper motor by Internet using MQTT broker and a
linkit Smart 7688 dev board.

Materials

- 1 x Linkit Smart 7688  (http://store.redlibre.cl/index.php?id_product=65&controller=product)
- 1 x 4-wires stepping motor
- 1 x Easydriver v44 (http://store.redlibre.cl/index.php?id_product=71&controller=product)

First of all, you need pre-configure the Linkit Smart 7688 with this

- Enter by ssh to IP of Linkit Smart 7688 ,please read this link to achieve this

http://labs.mediatek.com/site/global/developer_tools/mediatek_linkit_smart_7688/get_started/7688/sign_in/index.gsp

or this link in spanish

http://wiki2.redlibre.cl/index.php/Como_usar_el_MPU_Linkit_Smart_7688_conectado_a_Internet_mediante_MQTT_para_control_de_artefactos_el%C3%A9ctricos_por_Internet#Configurar_el_Linkit_Smart_por_primera_vez

Next , enter by ssh to ip assigned by your router

ssh IP_Assigned_by_your_router -lroot

Next, execute this commands

opkg update
opkg install python-pip
pip install paho-mqtt

Ok, now download the file from github to /root 

https://github.com/pumanzor/iot-redlibre/blob/master/linkit/easydriver/mqtt_easydriver_stepper.py

then, go to this web site

http://test.redlibre.cl

click on "Connect" button, then put this string into "Topic" box

/redlibre/test/motor/control/

Finally on box "Message" , we have three options command 

m1 : turn left motor
m2 : turn right motor
m0 : stop motor

then put 

m1

on Message box and voila..

to stop motor punt into "Message" box:

m0

=======================================================================================





