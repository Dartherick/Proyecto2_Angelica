from PyQt5.QtWidgets import QApplication
import sys
import cv2
import threading
from PyQt5.QtCore import QObject, pyqtSignal

class Camera(QObject):
    frameReady = pyqtSignal(object)

    def __init__(self, id):
        super().__init__()
        self.vid = cv2.VideoCapture(id)
        self._is_running = False

    def start(self):
        self._is_running = True
        self._capture_thread = threading.Thread(target=self._execute)
        self._capture_thread.start()

    def stop(self):
        self._is_running = False
        self._capture_thread.join()
        self.vid.release()
        cv2.destroyAllWindows()

    def _execute(self):
        while self._is_running:
            ret, frame = self.vid.read()
            if ret:
                self.frameReady.emit(frame)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    CAM = Camera(0)
    CAM.start()
    sys.exit(app.exec_())
