import mraa
pin = mraa.Pwm(18)
#pin2 = mraa.Pwm(19)

pin1 = mraa.Gpio(1)
pin1.dir(mraa.DIR_OUT)


while True:
	pin1.write(0)
	pin2.period_us(200)      
	pin2.enable(True)
	pin2.write(0.1)  
