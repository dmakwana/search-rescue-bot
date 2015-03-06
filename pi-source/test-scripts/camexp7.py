from SimpleCV import Image
from SimpleCV import Color
from SimpleCV import Camera
import time

cam = Camera(prop_set={"width":320,"height":240})

print "Camera Initialized!"

while(1):
    img = cam.getImage()
    #img.save("tempimage.jpg")
    #img1 = Image(img)
    print "."
    blobs = img.findBlobs()
    if blobs:
	for blob in blobs:
	    if blob.isSquare(0.1,0.1):
		print blob.centroid()
