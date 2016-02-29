import mraa
#pin = mraa.Pwm(18)
pin19 = mraa.Pwm(19)

pin1 = mraa.Gpio(1)
pin1.dir(mraa.DIR_OUT)


while True:
	pin1.write(0)
	pin19.period_us(300)      
	pin19.enable(True)
	pin19.write(0.1)  
