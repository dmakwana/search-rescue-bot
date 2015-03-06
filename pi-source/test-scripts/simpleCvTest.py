from SimpleCV import Image
import time
t3 = time.time()
img = Image("image2.png")
t1 = time.time()
blobs = img.findBlobs()
t2 = time.time()
if blobs:
    blobs.draw()
print "time taken: " + str(t1-t3)
img.show()
time.sleep(5)
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
