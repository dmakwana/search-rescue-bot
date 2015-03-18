# This script logs height data and passes direction to Arduino

from SimpleCV import Image
from SimpleCV import Color
from SimpleCV import Camera
import time
import picamera
import io
import smbus

# for RPI version 1, use "bus = smbus.SMBus(0)"
bus = smbus.SMBus(1)
time_str = str(time.strftime("%Y%m%d-%H%M%S"))
dir_name = "log-data/"
file_name = dir_name+time_str
f = open(file_name, "w")
# This is the address we setup in the Arduino Program

address = 0x04

def writeNumber(value):
    bus.write_byte(address,value)
    return -1

def readNumber():
    number = bus.read_byte(address)
    return number

cam = Camera(prop_set={"width":320,"height":240})
time.sleep(1)

print "Camera Initialized!"

xdist = 0
ydist = 0
xcenter = 160
ycenter = 120
thres = 5
foundSquare = False
square = 0
dontoutput = False

while True:
    img = cam.getImage()
    height = readNumber()
    print "Height Read: ", height
    f.write(str(height))
    f.write('\n')
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
            writeNumber(4)
        else:
            writeNumber(3)
            print "left"
        elif abs(ydist) > 20 and abs(ydist) > abs(xdist):
        if ydist > 0:
            writeNumber(2)
            print "down"
        else:
            writeNumber(1)
            print "up"
            else:
        writeNumber(0)
        # print "X: " + str(xdist) + ", Y: " + str(ydist)
    dontoutput = False
    foundSquare = False
