import cv2
import sys
from PyQt5 import QtGui
from PyQt5.QtGui import *
from PyQt5.QtCore import *
from PyQt5.uic import loadUi
from PyQt5.QtWidgets import QMainWindow

class MainHMI(QMainWindow):
    def __init__(self,file):
        super(MainHMI, self).__init__()
        loadUi(file,self)
        self.TabWidget.setCurrentIndex(3)
        self.EnableTabs(False)
        
        #camera
        self.EnableButton.clicked.connect(self.disconnectCamera)  # Connect the button click to disconnectCamera
        self.cameraPaused = False  # Flag to track if the camera feed is paused

        
        self.pushButton.clicked.connect(self.pushButton_action)
        self.pushButton_2.clicked.connect(self.pushButton2_action)
        self.pushButton_3.clicked.connect(self.pushButton3_action)
        self.pushButton_4.clicked.connect(self.pushButton4_action)
        self.pushButton_5.clicked.connect(self.pushButton5_action)
        self.pushButton_6.clicked.connect(self.pushButton6_action)
        self.pushButton_7.clicked.connect(self.pushButton7_action)
        self.ClearButton.clicked.connect(self.ClearB)
        
        BaudRate_List = ("9600","38400","57600","115200")
        self.BaudRate_ComboBox.addItems(BaudRate_List)

        self.Rutas_ComboBox.currentTextChanged.connect(self.TriggerComboBox)

        #delete later
        #self.Map.setStyleSheet("background-color: white")
        self.Map.setStyleSheet("border: 1px solid black; background-color: white")
        self.FactibilidadLabel.setStyleSheet("border: 1px solid black")

        # Load and set the image to the QLabel
        self.ImagePaths = ("Icons\Battery\Empty.png",
                           "Icons\Battery\Bar_1.png",
                           "Icons\Battery\Bar_2.png",
                           "Icons\Battery\Bar_3.png",
                           "Icons\Battery\Bar_4.png",
                           "Icons\Battery\Bar_5.png",
                           "Icons\Battery\Bar_6.png"
                           )

        self.Batteries = {1:self.Battery1,
                          2:self.Battery2,
                          3:self.Battery3,
                          4:self.Battery4,
                          }

        print(self.ImagePaths[0])
        pixmap = QtGui.QPixmap(self.ImagePaths[0])
        self.Batteries[1].setPixmap(pixmap)
        self.Batteries[2].setPixmap(pixmap)
        self.Batteries[3].setPixmap(pixmap)
        self.Batteries[4].setPixmap(pixmap)

    def ChangeBatteryImage(self,Battery,Percentage):
        BarPercentage = 100/5

        if Percentage == 0:
            pixmap = QtGui.QPixmap(self.ImagePaths[0])
        elif Percentage <= BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[1])
        elif Percentage <= 2*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[2])
        elif Percentage <= 3*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[3])
        elif Percentage <= 4*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[4])
        elif Percentage <= 5*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[5])
        elif Percentage <= 6*BarPercentage:
            pixmap = QtGui.QPixmap(self.ImagePaths[6])

        self.Batteries[Battery].setPixmap(pixmap)

    def closeEvent(self, event):
        #serial_connection.close_port()
        print("Closing...")
    
    def pushButton_action(self):
        self.ChangeBatteryImage(1,0)
        self.ChangeBatteryImage(2,0)
        self.ChangeBatteryImage(3,0)
        self.ChangeBatteryImage(4,0)

    def pushButton2_action(self):
        self.ChangeBatteryImage(1,17)
        self.ChangeBatteryImage(2,17)
        self.ChangeBatteryImage(3,17)
        self.ChangeBatteryImage(4,17)

    def pushButton3_action(self):
        self.ChangeBatteryImage(1,34)
        self.ChangeBatteryImage(2,34)
        self.ChangeBatteryImage(3,34)
        self.ChangeBatteryImage(4,34)

    def pushButton4_action(self):
        self.ChangeBatteryImage(1,51)
        self.ChangeBatteryImage(2,51)
        self.ChangeBatteryImage(3,51)
        self.ChangeBatteryImage(4,51)

    def pushButton5_action(self):
        self.ChangeBatteryImage(1,67)
        self.ChangeBatteryImage(2,67)
        self.ChangeBatteryImage(3,67)
        self.ChangeBatteryImage(4,67)
    
    def pushButton6_action(self):
        self.ChangeBatteryImage(1,84)
        self.ChangeBatteryImage(2,84)
        self.ChangeBatteryImage(3,84)
        self.ChangeBatteryImage(4,84)

    def pushButton7_action(self):
        self.ChangeBatteryImage(1,101)
        self.ChangeBatteryImage(2,101)
        self.ChangeBatteryImage(3,101)
        self.ChangeBatteryImage(4,101)
        
    def TriggerComboBox(self):
        Distancia_total = (0,40182.6, 44571.4, 42821.5)
        Puntos = ('Punto 1', 'Punto 2', 'Punto 3', 'Punto 4', 'Punto 5', 'Punto 6', 'Punto 7')

        # Load and set the image to the QLabel
        MapPaths = ("Icons\Maps\Map1.png",
                    "Icons\Maps\Map2.png",
                    "Icons\Maps\Map3.png")

        i = self.Rutas_ComboBox.currentIndex() 
        self.DistanceTotal.setText(str(Distancia_total[i])+"m")

        if i > 0:
            pixmap = QtGui.QPixmap(MapPaths[i-1])
            self.Map.setPixmap(pixmap)

            self.Puntos_ComboBox.clear()
            if i == 1:
                self.Puntos_ComboBox.addItems(Puntos[0:6])
            elif i == 2:
                self.Puntos_ComboBox.addItems(Puntos[0:5])
            elif i == 3:
                self.Puntos_ComboBox.addItems(Puntos)
        else:
            self.Map.clear()

    def ClearB(self):
        self.TimeRest.setText("")
        self.TimeTotal.setText("")
        self.Map.clear()
        self.TimeTotal.setText("")
        self.FactibilidadLabel.setStyleSheet("border: 1px solid black")
        self.FactibilidadLabel.setText("")
        self.Rutas_ComboBox.setCurrentIndex(0)
        self.Puntos_ComboBox.clear()
    
    def ProgressBar_Progression(self,ProgressBar, Start, End, Time):
        self.animation = QPropertyAnimation(ProgressBar, b"value")  # Create a QPropertyAnimation object for the progress bar value
        self.animation.setDuration(Time)  # Set the duration of the animation in milliseconds
        self.animation.setStartValue(Start)  # Set the start value of the animation
        self.animation.setEndValue(End)  # Set the end value of the animation
        self.animation.start()  # Start the animation

    def updateFrame(self, frame):
        rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        h, w, ch = rgb_image.shape
        bytes_per_line = ch * w
        image = QImage(rgb_image.data, w, h, bytes_per_line, QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(image)
        self.Camera_Window.setPixmap(pixmap)

    def disconnectCamera(self):
        if not self.cameraPaused:
            self.cameraPaused = True  # Pause the camera feed
            black_image = QPixmap(1, 1)  # Create a 1x1 black image
            black_image.fill(Qt.black)  # Fill the image with black color
            self.Camera_Window.setPixmap(black_image)  # Set the <link>QLabel</link> to display the black image
            self.EnableButton.setText('Enable Camera')
    
    def SaveImage(self,frame):
        pass

    def EnableTabs(self,State):
        self.TabWidget.setTabEnabled(0, State)
        self.TabWidget.setTabEnabled(1, State)
        self.TabWidget.setTabEnabled(2, State)
        self.TabWidget.setTabEnabled(3, State)
        self.TabWidget.setTabEnabled(5, State)
        self.TabWidget.setTabEnabled(6, State)