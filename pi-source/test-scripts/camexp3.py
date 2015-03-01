import cv2
import numpy as np
import time

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
            bin, contours, hierarchy = cv2.findContours(bin, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
            for cnt in contours:
                cnt_len = cv2.arcLength(cnt, True)
                cnt = cv2.approxPolyDP(cnt, 0.02*cnt_len, True)
                if len(cnt) == 4 and cv2.contourArea(cnt) > 1000 and cv2.isContourConvex(cnt):
                    cnt = cnt.reshape(-1, 2)
                    max_cos = np.max([angle_cos( cnt[i], cnt[(i+1) % 4], cnt[(i+2) % 4] ) for i in xrange(4)])
                    if max_cos < 0.1:
                        squares.append(cnt)
    return squares


## Return a cv2 image
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
    # # Resizing the image, blur the image and convert it to HSV values for better recognition
    # # img = cv2.resize(img, (len(img[0]) / scale_down, len(img) / scale_down))
    # # img = cv2.GaussianBlur(img, (5,5), 0)
    # img = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

    # #Defining the red color range and calculating if these values lie in the range
    # red_lower = np.array([0, 150, 0], np.uint8)
    # red_upper = np.array([5, 255, 255], np.uint8)
    # red_binary = cv2.inRange(img, red_lower, red_upper)

    # # Dilates the red space, making it larger
    # dilation = np.ones((15, 15), "uint8")
    # red_binary = cv2.dilate(red_binary, dilation)

    # contours, _ = cv2.findContours(red_binary, cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)

    # if not contours == []:
    #     if not red:
    #         red = True
    #         print "Red surface detected!"
    # else:
    #     print "No red surface  detected."

    # return red


with picamera.PiCamera() as camera:
    # to speed things up, lower the resolution of the camera
    camera.resolution = (1024, 768)
    for i in range(5):
        img = check_img(camera)
        squares = find_squares(img)
        cv2.drawContours( img, squares, -1, (0, 255, 0), 3 )
        cv2.imshow('squares', img)
        time.sleep(3)

