#!/usr/bin/python

import RPi.GPIO as GPIO
import time
from HCSR04 import HCSR04
from MotorDriver import MotorDriver
	
if __name__ == "__main__":
    triggerPin = 18
    echoPin = 23
        
    if raw_input("Running pre-flight tests. Proceed? [Y/n] ") == "n" : exit()
    
    hcsr04 = HCSR04(triggerPin, echoPin)
    motorDriver = MotorDriver()

    print "Testing sensor..."
    for i in range(0, 19) :
        print "Range: " + str(hcsr04.getRangeInCentimeters())
        time.sleep(0.5)
        
    if raw_input("Success? [Y/n] ") == "n" : exit() 
    
    print "Testing motor driver..."
    if raw_input("Moving forward...success? [Y/n] ") == "n" : exit()
    motorDriver.forward(0.1)

    if raw_input("Moving backward...success? [Y/n] ") == "n" : exit()
    motorDriver.reverse(0.1)

    if raw_input("Rotating to the right...success? [Y/n] ") == "n" : exit()
    motorDriver.rotateCW(0.1)

    if raw_input("Rotating to the left...success? [Y/n] ") == "n" : exit()
    motorDriver.rotateCCW(0.1)
    
    if raw_input("All tests succeeded. Proceed? [Y/n] ") == "n" : exit()
    
    while 1:
        print "sensing..."
        range = hcsr04.getRangeInCentimeters()
        print "range is " + str(range)

        if range > 50:
            print "moving forward 0.5 secs"
            motorDriver.forward(0.5)
        else:
            print "checking to the right"
            motorDriver.rotateCW(0.2)
        time.sleep(1)
