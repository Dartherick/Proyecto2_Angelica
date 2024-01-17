import cv2
import threading
import numpy as np
from PyQt5.QtCore import QObject, pyqtSignal

class Camera(QObject):
    frameReady = pyqtSignal(object)
    detect_triangle = pyqtSignal()

    def __init__(self,id):
        super().__init__()
        self.Cam = cv2.VideoCapture(id)
        self._is_running = False

        self.redBajo1 = np.array([0, 100, 20], np.uint8)
        self.redAlto1 = np.array([8, 255, 255], np.uint8)
        self.redBajo2 = np.array([175, 100, 20], np.uint8)
        self.redAlto2 = np.array([179, 255, 255], np.uint8)

        self.SendMessage = False
        self.StartCam = False

    def start(self):
        self._is_running = True
        self._capture_thread = threading.Thread(target=self._execute)
        self._capture_thread.start()

    def stop(self):
        self._is_running = False
        self._capture_thread.join()
        self.Cam.release()
        cv2.destroyAllWindows()

    def _execute(self):
        while self._is_running:

            ret, self.cap = self.Cam.read() 

            if ret:
                frameHSV = cv2.cvtColor(self.cap,cv2.COLOR_BGR2HSV)

                maskRed1 = cv2.inRange(frameHSV, self.redBajo1, self.redAlto1)
                maskRed2 = cv2.inRange(frameHSV, self.redBajo2, self.redAlto2)
                self.maskRed = cv2.add(maskRed1, maskRed2)
                
                self.maskredvis = cv2.bitwise_and(self.cap,self.cap, mask= self.maskRed)

                self.image2 = self.maskRed.copy()

                gray_image = cv2.cvtColor(self.maskredvis, cv2.COLOR_BGR2GRAY) # Converting to gray image

                '''this function checks every pixel, and depending on how
                dark the pixel is, the threshold value will convert the pixel to either black or white (0 or 1))'''
                _, self.thresh_image = cv2.threshold(gray_image, 0, 50, cv2.THRESH_BINARY)

                #Retrieving outer-edge coordinates in the new threshold image
                contours, _ = cv2.findContours(self.thresh_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

                # Iterating through each contour to retrieve coordinates of each shape
                for i, contour in enumerate(contours):
                    if i == 0:
                        continue

                    '''The 2 lines below this comment will approximate the shape we want. The reason being that in certain cases the
                    shape we want might have flaws or might be imperfect, and so, for example, if we have a rectangle with a
                    small piece missing, the program will still count it as a rectangle. The epsilon value will specify the
                    precision in which we approximate our shape.'''
                    epsilon = 0.1*cv2.arcLength(contour, True)
                    approx = cv2.approxPolyDP(contour, epsilon, True)

                    # Retrieving coordinates of the contour so that we can put text over the shape.
                    x, y, w, h= cv2.boundingRect(approx)
                    x_mid = int(x + (w/3)) # This is an estimation of where the middle of the shape is in terms of the x-axis.
                    y_mid = int(y + (h/1.5)) # This is an estimation of where the middle of the shape is in terms of the y-axis.

                    # Setting some variables which will be used to display text on the final image
                    coords = (x_mid, y_mid)
                    colour = (255, 255, 255)
                    font = cv2.FONT_HERSHEY_DUPLEX

                    # This is the part where we actually guess which shape we have detected. The program will look at the amount of edges
                    # the contour/shape has, and then based on that result the program will guess the shape (for example, if it has 3 edges
                    # then the chances that the shape is a triangle are very good.)
                    #Previous area value = 7000
                    if (cv2.contourArea(contour)>= 8000):
                        #print(f'area = {cv2.contourArea(contour)} & lados = {len(approx)}')
                        cv2.putText(self.cap,f'area = {cv2.contourArea(contour)} & lados = {len(approx)}',(0,0),cv2.FONT_HERSHEY_SIMPLEX,1,(255,0,0),2,cv2.LINE_AA)
                        if len(approx) == 3:
                            cv2.drawContours(self.cap, [contour], 0, colour, 3)  #Drawing the outer-edges onto the image
                            cv2.putText(self.cap, "Triangle", coords, font, 1, colour, 1) # Text on the image
                            self.detect_triangle.emit()

                self.frameReady.emit(self.cap)

if __name__ == '__main__':

    Cam = Camera(0)

    while(Cam.Cam.isOpened()):
        Cam.Exec()

        # Displaying the image with the detected shapes onto the screen
        cv2.imshow("Modified Image", Cam.image2)
        cv2.imshow('self.maskredvis',Cam.maskredvis)
        cv2.imshow('self.maskRed',Cam.maskRed)
        cv2.imshow('original',Cam.cap)
        cv2.imshow('tresh image',Cam.thresh_image)

        if cv2.waitKey(1) & 0xFF == ord('q'): 
            break

    # After the loop release the self.cap object 
    Cam.Cam.release() 
    # Destroy all the windows 
    cv2.destroyAllWindows() 