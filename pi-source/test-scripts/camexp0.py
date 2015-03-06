# http://www.raspberrypi.org/documentation/usage/camera/python/README.md

import picamera
from time import sleep

camera = picamera.PiCamera()
camera.resolution = (320,240)
sleep(1)
camera.capture('image2.jpg')
