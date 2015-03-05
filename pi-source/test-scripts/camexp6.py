import cv2
import numpy as np
 
# Read image
im = cv2.imread("image2.jpg")
 
# Set up the detector with default parameters.
detector = cv2.SimpleBlobDetector()
 
# Detect blobs.
keypoints = detector.detect(im)
print keypoints
# Draw detected blobs as red circles.
# cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS ensures the size of the circle corresponds to the size of blob
#im_with_keypoints = cv2.drawContours(im, keypoints, 0, (0,0,255), -1)
 
# Show keypoints
#cv2.imshow("Keypoints", im_with_keypoints)
#cv2.imwrite("editedImage.jpg",im_with_keypoints)
