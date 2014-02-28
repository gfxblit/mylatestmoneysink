#!/usr/bin/python3
# Please see for more info:
# http://mylatestmoneysink.blogspot.com/2014/01/driving-1100-motors-with-tb6612fng.html

import RPi.GPIO as GPIO
import sys
import time

class MotorDriver:        
    # these use BCM pin layout
    _pins = [[27, 22], [4, 17]]
    
    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self._pins[0][0], GPIO.OUT)
        GPIO.setup(self._pins[0][1], GPIO.OUT)
        GPIO.setup(self._pins[1][0], GPIO.OUT)
        GPIO.setup(self._pins[1][1], GPIO.OUT)
	    # start with everything stopped, in case something was driving the pins before
        self.stop()

    def __del__(self):
        self.stop()
        GPIO.cleanup()

    def _outputToMotor(self, motor, bits):
        GPIO.output(self._pins[motor][0], bits[0])
        GPIO.output(self._pins[motor][1], bits[1])
 
    # period is in seconds, if < 0, will run until a new command is issued
    def forward(self, period):
        self._outputToMotor(0, [GPIO.LOW, GPIO.HIGH])
        self._outputToMotor(1, [GPIO.HIGH, GPIO.LOW])
        if period > 0:
            time.sleep(period)
            self.stop()
        
    def stop(self):
        self._outputToMotor(0, [GPIO.LOW, GPIO.LOW])
        self._outputToMotor(1, [GPIO.LOW, GPIO.LOW])

    def reverse(self, period):
        self._outputToMotor(0, [GPIO.HIGH, GPIO.LOW])
        self._outputToMotor(1, [GPIO.LOW, GPIO.HIGH])

        if period > 0:
            time.sleep(period)
            self.stop()
    
    def turnLeft(self, period):
        self._outputToMotor(0, [GPIO.LOW, GPIO.HIGH])
        self._outputToMotor(1, [GPIO.LOW,  GPIO.HIGH])

        if period > 0:
            time.sleep(period)
            self.stop()
    
    def turnRight(self, period):
        self._outputToMotor(0, [GPIO.HIGH, GPIO.LOW])
        self._outputToMotor(1, [GPIO.HIGH,GPIO.LOW])    

        if period > 0:
            time.sleep(period)
            self.stop()

if __name__ == "__main__":
    motorDriver = MotorDriver()
    
    while 1:
       cmd = input('cmd (w=forward s=reverse a=turn left d=turn right o=off q=quit): ')
       if cmd == 'w':
           motorDriver.forward(0)
       if cmd == 's':
           motorDriver.reverse(0)
       if cmd == 'o':
           motorDriver.stop()
       if cmd == 'd':
           motorDriver.turnRight(0.2)
       if cmd == 'a':
           motorDriver.turnLeft(0.2)
       if cmd == 'q':
           sys.exit()

