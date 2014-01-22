#!/usr/bin/python
# Please see for more info:
# http://mylatestmoneysink.blogspot.com/2014/01/driving-1100-motors-with-tb6612fng.html

import RPi.GPIO as GPIO
import sys
import time

class MotorDriver:
    # these use BCM pin layout
    _pins = [[2, 3], [4, 17]]

    def __init__(self):
        GPIO.setmode(GPIO.BCM)
        GPIO.setup(self._pins[0][0], GPIO.OUT)
        GPIO.setup(self._pins[0][1], GPIO.OUT)
        GPIO.setup(self._pins[1][0], GPIO.OUT)
        GPIO.setup(self._pins[1][1], GPIO.OUT)

    def __del__(self):
        GPIO.cleanup()

    def _outputToMotor(self, motor, bits):
        GPIO.output(self._pins[motor][0], bits[0])
        GPIO.output(self._pins[motor][1], bits[1])
 
    # period is in seconds, if < 0, will run until a new command is issued
    def forward(self, period):
        self._outputToMotor(0, [GPIO.LOW, GPIO.HIGH])
        self._outputToMotor(1, [GPIO.LOW, GPIO.HIGH])
        if time >= 0:
            time.sleep(period)
            self.stop()
        
    def stop(self):
        self._outputToMotor(0, [GPIO.LOW, GPIO.LOW])
        self._outputToMotor(1, [GPIO.LOW, GPIO.LOW])

    def reverse(self, period):
        self._outputToMotor(0, [GPIO.HIGH, GPIO.LOW])
        self._outputToMotor(1, [GPIO.HIGH, GPIO.LOW])
        if time >= 0:
            time.sleep(period)
            self.stop()
    
    def rotateCW(self, period):
        self._outputToMotor(0, [GPIO.HIGH, GPIO.LOW])
        self._outputToMotor(1, [GPIO.LOW,  GPIO.HIGH])        
        if time >= 0:
            time.sleep(period)
            self.stop()
    
    def rotateCCW(self, period):
        self._outputToMotor(0, [GPIO.LOW, GPIO.HIGH])
        self._outputToMotor(1, [GPIO.HIGH,GPIO.LOW])        
        if time >= 0:
            time.sleep(period)
            self.stop()
                
if __name__ == "__main__":
    motorDriver = MotorDriver()
    while 1:
       cmd = raw_input('cmd (f=forward r=reverse o=off c=cw w=ccw q=quit): ')
       if cmd == 'f':
           motorDriver.forward(0.5)
       if cmd == 'r':
           motorDriver.reverse(0.5)
       if cmd == 'o':
           motorDriver.stop()
       if cmd == 'c':
           motorDriver.rotateCW(0.5)
       if cmd == 'w':
           motorDriver.rotateCCW(0.5)
       if cmd == 'q':
           sys.exit()

