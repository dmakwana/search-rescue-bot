# http://www.raspberrypi.org/documentation/usage/camera/python/README.md

import picamera
from time import sleep

camera = picamera.PiCamera()
camera.resolution = (320,240)
sleep(0.1)
camera.capture('imageTest.jpg')
#sleep(0.2)
#camera.capture('image3.jpg')
#sleep(0.2)
#camera.capture('image4.jpg')
#sleep(0.2)
#camera.capture('image5.jpg')
#sleep(0.2)
#camera.capture('image6.jpg')
#sleep(0.2)
#camera.capture('image7.jpg')
#sleep(0.2)
#camera.capture('image8.jpg')
