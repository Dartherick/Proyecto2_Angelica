import cv2
import numpy as np

cap = cv2.imread(".\Pics\Front1.jpeg")
# cap = cv2.imread(".\Pics\Back.jpeg")
#cap = cv2.imread(".\Pics\Lateral.jpeg")

redBajo1 = np.array([0, 100, 20], np.uint8)
redAlto1 = np.array([8, 255, 255], np.uint8)
redBajo2=np.array([175, 100, 20], np.uint8)
redAlto2=np.array([179, 255, 255], np.uint8)

frameHSV = cv2.cvtColor(cap,cv2.COLOR_BGR2HSV)

maskRed1 = cv2.inRange(frameHSV, redBajo1, redAlto1)
maskRed2 = cv2.inRange(frameHSV, redBajo2, redAlto2)
maskRed = cv2.add(maskRed1, maskRed2)
 
maskredvis = cv2.bitwise_and(cap,cap, mask= maskRed)

cv2.imshow('maskredvis',maskredvis)
cv2.imshow('maskred',maskRed)
cv2.imshow('original',cap)

cv2.waitKey(0)