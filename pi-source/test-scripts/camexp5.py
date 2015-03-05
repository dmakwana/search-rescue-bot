import picamera
import time
import cv2
import io
import numpy as np

def angle_cos(p0, p1, p2):
    d1, d2 = (p0-p1).astype('float'), (p2-p1).astype('float')
    return abs( np.dot(d1, d2) / np.sqrt( np.dot(d1, d1)*np.dot(d2, d2) ) )

def find_squares(img):
    img = cv2.GaussianBlur(img, (5, 5), 0)
    squares = []
    for gray in cv2.split(img):
        for thrs in xrange(0, 255, 26):
            if thrs == 0:
                bin = cv2.Canny(gray, 0, 50, apertureSize=5)
                bin = cv2.dilate(bin, None)
            else:
                retval, bin = cv2.threshold(gray, thrs, 255, cv2.THRESH_BINARY)
            contours, hierarchy = cv2.findContours(bin, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                cnt_len = cv2.arcLength(cnt, True)
                cnt = cv2.approxPolyDP(cnt, 0.02*cnt_len, True)
                if len(cnt) == 4 and cv2.contourArea(cnt) > 1000 and cv2.isContourConvex(cnt):
                    cnt = cnt.reshape(-1, 2)
                    max_cos = np.max([angle_cos( cnt[i], cnt[(i+1) % 4], cnt[(i+2) % 4] ) for i in xrange(4)])
                    if max_cos < 0.1:
                        squares.append(cnt)
    return squares

def check_img(camera):
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
    camera.resolution = (320,240)
    for i in range(5):
        t1 = time.time()
	img = check_img(camera)
	t2 = time.time()
	squares = find_squares(img)
	t3 = time.time()
	if len(squares) > 0:
	    print "found"
	else:
	    print "not found"
	image_name = "image" + str(i) + ".png"
	cv2.imwrite(image_name, img)
	print "image capturing time: " + str(t2-t1)
	print "image processing time: " + str(t3-t2)

    
