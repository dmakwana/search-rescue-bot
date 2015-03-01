import cv2
import numpy as np
import time
import picamera
import io

def angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return abs( np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) ) )

def highlight_squares(data):
    img = cv2.imdecode(data)
    gray = cv2.imread(data,0)

    ret,thresh = cv2.threshold(gray,127,255,1)

    contours,h = cv2.findContours(thresh,1,2)

    for cnt in contours:
        approx = cv2.approxPolyDP(cnt,0.01*cv2.arcLength(cnt,True),True)
        if len(approx)==4:
            print "square"
            cv2.drawContours(img,[cnt],0,(0,0,255),-1)

    cv2.imshow('img',img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()



## Return a cv2 image
def image_data(camera):
    # saving the picture to an in-program stream rather than a file
    stream = io.BytesIO()

    #scale_down = 6
    red = False

    # capture into stream
    camera.capture(stream, format='jpeg', use_video_port=True)
    # convert image into numpy array
    data = np.fromstring(stream.getvalue(), dtype=np.uint8)
    # turn the array into a cv2 image
    img = cv2.imdecode(data, 1)

    return img


with picamera.PiCamera() as camera:
    # to speed things up, lower the resolution of the camera
    camera.resolution = (1024, 768)
    for i in range(5):
	print str(i) + "1"
        data = image_data(camera)
	print str(i) + "2"
        squares = show_squares(data)


