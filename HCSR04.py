#!/usr/bin/python

import RPi.GPIO as GPIO
import time

class HCSR04:

    def __init__(self, triggerPin, echoPin):
        GPIO.setmode(GPIO.BCM)
        self._triggerPin = triggerPin;
        self._echoPin = echoPin
        GPIO.setup(self._triggerPin, GPIO.OUT)
        GPIO.setup(self._echoPin, GPIO.IN)
	GPIO.output(self._triggerPin, GPIO.LOW)

    def getRangeInCentimeters(self):
        # issue a 10uS pulse
        GPIO.output(self._triggerPin, GPIO.HIGH)
	time.sleep(0.00001)
        GPIO.output(self._triggerPin, GPIO.LOW)
        
        # wait for the echo
        while GPIO.input(self._echoPin) == 0:
            start = time.time()

        while GPIO.input(self._echoPin) == 1:
            stop = time.time()
        
        return (stop - start)  * 17241.37931034482759 # 1000000 / 58
         
    def __del__(self):
        GPIO.cleanup()
    
	
if __name__ == "__main__":
    hcsr04 = HCSR04(17, 4)
    raw_input("waiting to start")
    while 1:
        print "range: " + str(hcsr04.getRangeInCentimeters())
        time.sleep(1)
