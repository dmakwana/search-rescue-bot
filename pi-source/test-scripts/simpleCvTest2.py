from SimpleCV import Image
from SimpleCV import Color
import time
print "STARTING NOW!"
img = Image("image2.jpg")
t1 = time.time()
blobs = img.findBlobs()
t2 = time.time()
if blobs:
    for blob in blobs:
	if blob.isSquare(0.1,0.1):
	    blob.draw()
	    print blob.centroid()
t3 = time.time()
print "showing image"
#print "time taken: " + str(t1-t3)
#img.show()
img.save("image1-edited.jpg")
print "done"
print "time taken to findBlobs: " + str(t2-t1)
print "time taken to check square: " + str(t3-t2)
time.sleep(25)
#img = img.show()
#time.sleep(5)
#img = img.binarize()
#img = img.show()
#time.sleep(5)
#img = img.findBlobs()
#for blob in blobs:
#    blob.draw()
#img.show()
#time.sleep(5)
