from SimpleCV import Image
from SimpleCV import Color
from SimpleCV import Camera
import serial
import time

cam = Camera(prop_set={"width":320,"height":240})
ser = serial.Serial('/dev/ttyACM0', 9600)

print "Camera Initialized!"

while(1):
    img = cam.getImage()
    #img.save("tempimage.jpg")
    #img1 = Image(img)
    print "."
    blobs = img.findBlobs()
    if blobs:
	for blob in blobs:
	    if blob.isSquare(0.15,0.15):
		blob.draw()
		print blob.centroid()
		print blob.centroid()[0]
    img.show()
    time.sleep(1)
