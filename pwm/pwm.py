#!/usr/bin/python

import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(25, GPIO.OUT)

# pin 25, 4KHz frequency
p = GPIO.PWM(25, 4000.0)
# 50% duty cycle
p.start(50.0)

raw_input("press enter to end")
p.stop()

GPIO.cleanup()