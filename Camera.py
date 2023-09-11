import cv2

class Camera():
    def __init__(self,index=0):
        self.Cap = cv2.VideoCapture(index)
        self.i = 0

    def TakePic(self):
        if self.Cap.isOpened():
            ret,self.frame = self.Cap.read()
            if ret and self.frame is not None:
                cv2.imwrite('Original.jpg', self.frame)
