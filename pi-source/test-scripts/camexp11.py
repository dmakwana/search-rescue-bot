
from SimpleCV import Image
from SimpleCV import Color
from SimpleCV import Camera
import time
import picamera
import io

cam = Camera(prop_set={"width":320,"height":240})
#camera = picamera.PiCamera()
#camera.resolution = (320,240)
time.sleep(2)
#my_stream = io.BytesIO()

print "Camera Initialized!"
xdist = 0
ydist = 0
xcenter = 160
ycenter = 120
thres = 20
foundSquare = False
square = 0
dontoutput = False
while(1):
    #camera.capture('workingImage.jpg')
    img = cam.getImage()
    #img = Image('workingImage.jpg')    
    blobs = img.findBlobs()
    if blobs:
        for blob in blobs:
            if blob.isSquare(0.1,0.1):
		if foundSquare:
		    dontoutput = True
		    break    
		foundSquare = True
		square = blob
	if not dontoutput:
	    xdist = xcenter - blob.centroid()[0]
   	    ydist = ycenter - blob.centroid()[1]
	    if abs(xdist) > 20 and abs(xdist) > abs(ydist):
		if xdist > 0:
		    print "right"
		else:
		    print "left"
	    elif abs(ydist) > 20 and abs(ydist) > abs(xdist):
		if ydist > 0:
		    print "down"
		else:
		    print "up"
	    print "X: " + str(xdist) + ", Y: " + str(ydist)
	dontoutput = False
	foundSquare = False
