# http://www.raspberrypi.org/documentation/usage/camera/python/README.md

import picamera
from time import sleep

camera = picamera.PiCamera()

camera.capture('image1.jpg')
sleep(.5)
camera.capture('image2.jpg')
sleep(.5)
camera.capture('image3.jpg')
sleep(.5)
camera.capture('image4.jpg')
sleep(.5)
camera.capture('image5.jpg')
sleep(.5)
camera.capture('image6.jpg')
sleep(.5)
camera.capture('image7.jpg')
sleep(.5)
camera.capture('image8.jpg')
sleep(.5)
camera.capture('image9.jpg')

