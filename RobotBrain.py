#!/usr/bin/python3

import RPi.GPIO as GPIO
import time
from enum import Enum
from HCSR04 import HCSR04
from MotorDriver import MotorDriver
	
class State(Enum) :
    Search = 0
    Forward = 1
    
if __name__ == "__main__":
    triggerPin = 18
    echoPin = 23
    state = State.Search
        
    if input("Running pre-flight tests. Proceed? [Y/n] ") == "n" : exit()
    
    hcsr04 = HCSR04(triggerPin, echoPin)
    motorDriver = MotorDriver()

    print("Testing sensor...")
    
    for i in range(0, 19) :
        print ("Range: " + str(hcsr04.getRangeInCentimeters()))
        time.sleep(0.5)
        
    if input("Success? [Y/n] ") == "n" : exit() 
    
    print ("Testing motor driver...")

    motorDriver.forward(0.1)
    if input("Moving forward...success? [Y/n] ") == "n" : exit()

    motorDriver.reverse(0.1)
    if input("Moving backward...success? [Y/n] ") == "n" : exit()

    motorDriver.turnRight(0.1)
    if input("Rotating to the right...success? [Y/n] ") == "n" : exit()

    motorDriver.turnLeft(0.1)
    if input("Rotating to the left...success? [Y/n] ") == "n" : exit()
    
    if input("All tests succeeded. Proceed? [Y/n] ") == "n" : exit()
    
    while 1:
        startState = state
        
        range = hcsr04.getRangeInCentimeters()

        if range < 50:
            state = State.Search
        else:
            state = State.Forward
        
        if startState != state:
            print("New state: " + str(state))
        
        # do something based on the state
        if state == State.Search:
            motorDriver.turnRight(0)
        elif state == State.Forward:
            motorDriver.forward(0)

        time.sleep(.1)
